#include "mesh/CylinderMesh.hpp"
#include <osgUtil/SmoothingVisitor>
// http://www.bencloward.com/tutorials_normal_maps2.shtml
CylinderMesh::CylinderMesh()
{ }

const tuple< const Vec3Array * const
           , const DrawElementsUShort * const
           , const Vec3Array * const
           >
CylinderMesh::unit(unsigned int points)
{
    //points should always be even

    // First check if cylinder with the required number of points already exists.
    auto result = cylinders.find(points);
    if(result != cylinders.end()) { return result -> second; }

    // If a cylinder is not found then create, cache and return it.

    unsigned int vertex_count   = 2 * points;
    unsigned int triangle_count = 12 * points - 12;

    unsigned int triangle_index = 0;

    unsigned int i, j;
    float theta;

    float theta_delta = 2.0 * M_PI / points;
    float z;
    float radius;

    Vec3Array          * vertices = new Vec3Array(2 * vertex_count);
    Vec3Array          * normals  = new Vec3Array(2 * vertex_count);
    DrawElementsUShort * indices  = new DrawElementsUShort(GL_TRIANGLES, triangle_count);

    float x, y;
    // const float COS_DELTA_THETA         = cos(theta_delta);
    // const float COS_DELTA_THETA_HALF    = cos(theta_delta / 2);
    // const float NORMAL_MAGNITUDE        = sqrt(COS_DELTA_THETA + 3);
    // float x, y, nx, ny;
    // const float NZ = M_SQRT2 / NORMAL_MAGNITUDE;
    // const float NC = M_SQRT2 * COS_DELTA_THETA_HALF / NORMAL_MAGNITUDE;

    for(i = 0; i < points; ++i)
    {
        theta = i * theta_delta;
        x = cos(theta);
        y = sin(theta);

        // nx = NC * x;
        // ny = NC * y;

        (*vertices)[i]          = Vec3f(x, y,  0.5f);
        (*vertices)[i + points] = Vec3f(x, y, -0.5f);
        (*vertices)[i + 2 * points] = Vec3f(x, y,  0.5f);
        (*vertices)[i + 3 * points] = Vec3f(x, y, -0.5f);

        // (*normals)[i]           = Vec3f(nx, ny,  NZ);
        // (*normals)[i + points]  = Vec3f(nx, ny, -NZ);
        (*normals)[i]           = Vec3f(x,y,0.0f);
        (*normals)[i + points]  = Vec3f(x,y,0.0f);
        (*normals)[i + 2 * points] = Vec3f(0.0f, 0.0f, 1.0f);
        (*normals)[i + 3 * points]  = Vec3f( 0.0f, 0.0f, -1.0f);

        // (*normals)[i + points] -> normalize();
        // (*normals)[i]           = Vec3f(nx, ny,  0);
        // (*normals)[i + points]  = Vec3f(nx, ny,  0);
    }

    for(i = 0; i < points; ++i)
    {
        (*indices)[triangle_index    ] = i;
        (*indices)[triangle_index + 1] = i + points;
        (*indices)[triangle_index + 2] = i + 1;

        (*indices)[triangle_index + 3] = i + points;
        (*indices)[triangle_index + 4] = i + 1 + points;
        (*indices)[triangle_index + 5] = i + 1;

        triangle_index += 6;
    }

    (*indices)[triangle_index - 1]     = 0;
    (*indices)[triangle_index - 2]     = points;
    (*indices)[triangle_index - 4]     = 0;

    for(i = 2 * points + 1; i < 3 * points - 1; ++i)
    {
        (*indices)[triangle_index    ] = 2 * points;
        (*indices)[triangle_index + 1] = i;
        (*indices)[triangle_index + 2] = i + 1;
        triangle_index += 3;
    }

    for(i = 3 * points + 1; i < 4 * points - 1; ++i)
    {
        (*indices)[triangle_index    ] = i + 1;
        (*indices)[triangle_index + 1] = i;
        (*indices)[triangle_index + 2] = 3 * points;
        triangle_index += 3;
    }

    auto insert_position =
        cylinders.insert( make_pair( points
                                   , make_tuple( vertices
                                               , indices
                                               , normals
                                               )
                                   )
                        );
    return insert_position.first -> second;
}

float
CylinderMesh::angle(Vec3f& vector1, Vec3f& vector2)
{
    float dot = vector1 * vector2;
    float det = vector1.x() * vector2.y() - vector1.y() * vector2.x();
    RECORD_INFO(string("angle =>") + to_string(atan2(det, dot)));
    return atan2(det, dot);
}

void
CylinderMesh::operator()( Vec3f        center
                        , float        upper_radius
                        , float        lower_radius
                        , float        height
                        , Vec3f        direction
                        , Geometry  *  geometry
                        , unsigned int points
                        , const Vec4&  color
                        , Vec3f        parent
                        )
{
    const auto arrays = unit(points);

    const auto unit_vertices = get<0>(arrays);
    const auto unit_indices  = get<1>(arrays);
    const auto unit_normals  = get<2>(arrays);

    ref_ptr<Vec3Array> vertices(
        new Vec3Array(unit_vertices -> size())
                               );
    ref_ptr<Vec3Array> normals(
        new Vec3Array(unit_normals -> size())
                              );
    ref_ptr<DrawElementsUShort> indices(
        new DrawElementsUShort(*unit_indices)
                                       );

    unsigned int i, j;
    Vec3f temp_vertex;



    Quat rotate;
    rotate.makeRotate(parent, osg::Vec3f(0.0f, 0.0f, 1.0f));
    Vec3f side = osg::Vec3f(0.0f, 0.0f, 1.0f) ^ (rotate * direction);
    side.normalize();
    //* RECORD_INFO(string("side => ") + to_string(side[0]) + "," + to_string(side[1]) + "," + to_string(side[2]));

    float warp_angle = acos((parent * direction) / (parent.length() * direction.length()));

    float max_warp = lower_radius * tan(warp_angle);

    //* RECORD_INFO(to_string(warp_angle));
    //* RECORD_INFO();
    //* if(parent.length() != 0.0)
    // {
    //     RECORD_INFO("coming here!");
    //     for(i = 0; i < points; ++i)
    //     {
    //         temp_vertex.set( (*vertices)[i + points][0]
    //                        , (*vertices)[i + points][1]
    //                        , 0.0
    //                        );
    //         float proportion = angle(side, temp_vertex) / M_PI;
    //         RECORD_INFO( string("proportion => ") + to_string(proportion));
    //         if(proportion <= -0.5f)
    //         {
    //             (*vertices)[i + 3 * points][2] = (*vertices)[i + points][2] =
    //             -0.5 - 2 * (1 + proportion) * max_warp;
    //         }
    //         else if(proportion <= 0.0f)
    //         {
    //             (*vertices)[i + 3 * points][2] = (*vertices)[i + points][2] =
    //             -0.5 + 2 * proportion * max_warp;
    //         }
    //         else if(proportion <= 0.5f)
    //         {
    //             (*vertices)[i + 3 * points][2] = (*vertices)[i + points][2] =
    //             -0.5 + 2 * proportion * max_warp;
    //         }
    //         else if(proportion <= 1.0f)
    //         {
    //             (*vertices)[i + 3 * points][2] = (*vertices)[i + points][2] =
    //             -0.5 + 2 * (1.0 - proportion) * max_warp;
    //         }
    //     }
    //* }

    rotate.makeRotate(osg::Vec3f(0.0f, 0.0f, 1.0f), direction);

    for(i = 0; i < points; ++i)
    {
        temp_vertex.set( upper_radius * (*unit_vertices)[i][0]
                       , upper_radius * (*unit_vertices)[i][1]
                       , height/2.0f
                       );
        (*vertices)[i + 2 * points] = (*vertices)[i] = rotate * temp_vertex + center;
        (*normals)[i + points]  = (*normals)[i]  = rotate * (*unit_normals)[i];
        (*normals)[i + 2 * points] = direction;
        (*normals)[i + 3 * points] = -direction;
        j = i + points;
        temp_vertex.set( lower_radius * (*unit_vertices)[j][0]
                       , lower_radius * (*unit_vertices)[j][1]
                       , -height/2.0f
                       );

        (*vertices)[j + 2 * points] = (*vertices)[j] = rotate * temp_vertex + center;
    }


    // for(; i < unit_vertices -> size(); ++i)
    // {
    //     (*vertices)[i] = rotate * temp_vertex + center;

    //     (*normals)[i]  = rotate * (*unit_normals)[i];
    // }

    Vec4Array * colors = new Vec4Array();
    colors -> push_back(color); //color);
    // for(i = 0; i < vertices -> size(); ++i)
    // {
    // }
    geometry -> setNormalArray( normals.get()
                                       , Array::BIND_PER_VERTEX
                                       );
    // polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,6));
    geometry -> addPrimitiveSet(indices.get());
    geometry -> setVertexArray(vertices.get());
    geometry -> setColorArray(colors);
    geometry -> setColorBinding( osg::Geometry::BIND_OVERALL );
    // osgUtil::SmoothingVisitor::smooth(*geometry);
}

Geometry *
CylinderMesh::operator()( Vec3f        center
                        , float        upper_radius
                        , float        lower_radius
                        , float        height
                        , Vec3f        direction
                        , unsigned int points
                        , const Vec4&  color
                        , Vec3f        parent
                        )
{

    Geometry * geometry = new Geometry();
    operator()( center
              , upper_radius
              , lower_radius
              , height
              , direction
              , geometry
              , points
              , color
              , parent
              );
    return geometry;
}


void
CylinderMesh::operator()( Vec4f        upper
                        , Vec4f        lower
                        , Geometry  *  geometry
                        , unsigned int points
                        , const Vec4&  color
                        , Vec3f        parent
                        )
{
    Vec3f center( (upper[0] + lower[0]) / 2.0
                , (upper[1] + lower[1]) / 2.0
                , (upper[2] + lower[2]) / 2.0
                );

    Vec3f direction( upper[0] - lower[0]
                   , upper[1] - lower[1]
                   , upper[2] - lower[2]
                   );

    float height  = direction.normalize();
    float upper_radius = upper[3];
    float lower_radius = lower[3];

    operator()( center
              , upper_radius
              , lower_radius
              , height
              , direction
              , geometry
              , points
              , color
              , parent
              );
}

Geometry *
CylinderMesh::operator()( Vec4f        upper
                        , Vec4f        lower
                        , unsigned int points
                        , const Vec4&  color
                        , Vec3f        parent
                        )
{
    Geometry * geometry = new Geometry();
    operator()( upper
              , lower
              , geometry
              , points
              , color
              , parent
              );
    return geometry;
}
