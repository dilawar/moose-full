#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>

#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgUtil/SmoothingVisitor>
#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/io_utils>
#include <osg/ShapeDrawable>
#include <osgDB/WriteFile>
#include <osgText/Text>

#include <sstream>
#include <tuple>

#include <iostream>

std::tuple<osg::Vec3f, osg::Vec3f, float>
interpolate( const osg::Vec3f & p0
           , const osg::Vec3f & p1
           , const osg::Vec3f & control
           , float radius0
           , float radius1
           , float t
           )
{
    float u  = 1.0f - t;
    float uu = u * u;
    osg::Vec3f position( p0 * uu
                       + control * 2 * u * t
                       + p1 * t * t
                       );
    osg::Vec3f direction( (control - p0) * 2.0f * u
                        + (p1 - control) * 2.0f * t
                        );
    float radius = radius0 + t * (radius1 - radius0);
    return std::tuple<osg::Vec3f, osg::Vec3f, float>( position
                                                         , direction
                                                         , radius
                                                         );
}

osg::Vec3f
quadratic_spline( const osg::Vec3f & p0
                , const osg::Vec3f & p1
                , const osg::Vec3f & control
                , float t
                )
{
    float u  = 1.0f - t;
    float uu = u * u;
    return ( p0 * uu
           + control * 2 * u * t
           + p1 * t * t
           );
}

osg::Vec3f
cubic_bezier_interpolate( const osg::Vec3f & p0
                        , const osg::Vec3f & p1
                        , const osg::Vec3f & p2
                        , const osg::Vec3f & p3
                        , float t
                        )
{
    float s = 1.0f - t;
    float ss = s * s;
    float sss = ss * s;
    float tt = t * t;
    float ttt = t * t * t;

    return ( p0 * sss
           + p1 * 3 * ss * t
           + p2 * 3 * s * tt
           + p3 * ttt
           );
}

void
generate_ring( const osg::Vec3Array * p0s
             , const osg::Vec3Array * p1s
             , const osg::Vec3Array * p2s
             , const osg::Vec3Array * p3s
             , float t
             , osg::Vec3Array * vertices
             , osg::Vec3Array * normals
             , size_t index
             )
{
    size_t size = p0s -> size();
    for(size_t i = 0; i < size; ++i)
    {
        osg::Vec3f mid(((*p1s)[i] + (*p2s)[i]) / 2.0f);
        (*vertices)[index + i] = cubic_bezier_interpolate( (*p0s)[i]
                                                         , (*p1s)[i] //mid //(*p1s)[i]
                                                         , (*p2s)[i] //mid // (*p2s)[i]
                                                         , (*p3s)[i]
                                                         , t
                                                         );
    }
    // std::cerr << "Here";
}

void
generate_normals( osg::Vec3Array * vertices
                , osg::Vec3Array * normals
                , size_t axial_segments
                , size_t radial_segments
                )
{
    size_t rows = axial_segments / 2 + 2;
    size_t index = 0;
    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < radial_segments / 2; ++j)
        {
            size_t current_index    = i * radial_segments + j;
            size_t opposite_index   = current_index + radial_segments / 2;
            (*normals)[current_index] = ((*vertices)[current_index] - (*vertices)[opposite_index]);
            (*normals)[current_index].normalize();
            (*normals)[opposite_index] = (*normals)[current_index] * -1.0f;
            ++index;
        }
    }

}

void
transform_ring( osg::Vec3Array *    ring
              , float               scale
              , osg::Vec3f          direction
              , osg::Vec3f          center
              , osg::Vec3Array *    vertices
              , size_t              index
              )
{
    osg::Quat rotate;
    rotate.makeRotate(osg::Vec3f(0.0f, 0.0f, 1.0f), direction);
    for(size_t i = 0; i < ring -> size(); ++i)
    {
        (*vertices)[index + i] = rotate * ((*ring)[i] * scale) + center;
    }
}


void
hide_vertices( osg::Vec3Array * vertices
             , size_t axial_segments
             , size_t radial_segments
             , size_t index
             )
{
    size_t row, col;
    size_t rows = axial_segments / 2 + 2;
    size_t cols = radial_segments;
    for(row = 0; row < rows; ++row)
    {
        for(col = 0; col < cols; ++col)
        {
            if(col == index)
            {
                (*vertices)[row * radial_segments + col].set(0.0f, 0.0f, 0.0f);
            }
        }
    }
}

void
connect_vertices( osg::Vec3Array * vertices
                , osg::DrawElementsUShort * indices
                , size_t axial_segments
                , size_t radial_segments
                )
{
    size_t triangle_index = 0;
    size_t row, col;
    size_t rows = axial_segments / 2 + 2;
    size_t cols = radial_segments;
    std::cerr << std::endl;
    for(row = 0; row < rows - 1; ++row)
    {

        for(col = 0 ; col < cols; ++col)
        {
            /*************************************

            d---------------c
            |               |
            |               |
            |               |
            |               |
            |               |
            |               |
            a---------------b

            ***************************************/
            ushort a = row * radial_segments + col;
            ushort b = a + 1;
            ushort d = a + radial_segments;
            ushort c = d + 1;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*indices)[triangle_index    ] = a;
            (*indices)[triangle_index + 1] = b;
            (*indices)[triangle_index + 2] = d;

            (*indices)[triangle_index + 3] = d;
            (*indices)[triangle_index + 4] = b;
            (*indices)[triangle_index + 5] = c;

            // (*indices)[triangle_index    ] = a;
            // (*indices)[triangle_index + 1] = b;
            // // (*indices)[triangle_index + 2] = b; //
            // // (*indices)[triangle_index + 3] = b + 1; //
            // (*indices)[triangle_index + 2] = d;
            // (*indices)[triangle_index + 3] = c;
            // (*indices)[triangle_index + 4] = b;
            // (*indices)[triangle_index + 5] = c;

            triangle_index += 6;
        }

        (*indices)[triangle_index - 1]     = row * radial_segments + radial_segments;
        (*indices)[triangle_index - 2]     = row * radial_segments;
        (*indices)[triangle_index - 5]     = row * radial_segments;
        // (*indices)[triangle_index - 1]     = row * radial_segments + radial_segments ;
        // (*indices)[triangle_index - 3]     = row * radial_segments;

    }
    std::cerr << "Triangle Index => " << triangle_index << std::endl;
}

void
smoothen( const osg::Vec3f & parent_proximal
        , const osg::Vec3f & joint
        , const osg::Vec3f & child_distal
        , const float parent_radius
        , const float child_radius
        , osg::Geometry * parent_geometry
        , osg::Geometry * child_geometry
        , float parent_fraction
        , float child_fraction
        , size_t axial_segments
        , size_t radial_segments
        )
{
    // axial_segments = axial_segments;
    size_t rows           = 2 + axial_segments / 2;
    size_t vertex_count   = rows * radial_segments;
    size_t triangles      = (rows - 1) * 2 * radial_segments;
    size_t triangle_count = triangles;

    osg::ref_ptr<osg::Vec3Array> parent_vertices         = new osg::Vec3Array(vertex_count);
    osg::ref_ptr<osg::Vec3Array> parent_normals          = new osg::Vec3Array(vertex_count);
    osg::ref_ptr<osg::DrawElementsUShort> parent_indices = new osg::DrawElementsUShort(GL_TRIANGLES, triangle_count * 3);
    std::cerr << "Original Triangle Index => " << triangle_count * 3 << std::endl;
    osg::ref_ptr<osg::Vec4Array> parent_colors                = new osg::Vec4Array();
    parent_colors -> push_back(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

    osg::ref_ptr<osg::Vec3Array> child_vertices          = new osg::Vec3Array(vertex_count);
    osg::ref_ptr<osg::Vec3Array> child_normals           = new osg::Vec3Array(vertex_count);
    osg::ref_ptr<osg::DrawElementsUShort> child_indices  = new osg::DrawElementsUShort(GL_TRIANGLES, triangle_count * 3);
    osg::ref_ptr<osg::Vec4Array> child_colors            = new osg::Vec4Array();
    child_colors -> push_back(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    std::cerr << "a";
    osg::Vec3f child_direction(child_distal - joint);
    osg::Vec3f parent_direction(joint - parent_proximal);
    float child_height  = child_direction.normalize();
    float parent_height = parent_direction.normalize();
    std::cout << "Child Height => "     << child_height     << std::endl;
    std::cout << "Parent Height => "    << parent_height    << std::endl;
    osg::Vec3f start_point = joint - parent_direction * parent_fraction * parent_height;
    osg::Vec3f end_point   = joint + child_direction  * child_fraction * child_height;
    std::cerr << "b";
    osg::ref_ptr<osg::Vec3Array> unit_ring(new osg::Vec3Array(radial_segments));
    for(size_t i = 0; i < radial_segments; ++i)
    {
        double theta = (i * 2 * osg::PI) / static_cast<double>(radial_segments);
        (*(unit_ring.get()))[i].set( float(cos(theta))
                                   , float(sin(theta))
                                   , 0.0f
                                   );
    }
    std::cerr << "c";
    osg::ref_ptr<osg::Vec3Array> p0s(new osg::Vec3Array(radial_segments));
    transform_ring( unit_ring.get()
                  , parent_radius
                  , parent_direction
                  , start_point
                  , p0s
                  , 0
                  );
    std::cerr << "d";
    osg::ref_ptr<osg::Vec3Array> p1s(new osg::Vec3Array(radial_segments));
    transform_ring( unit_ring.get()
                  , parent_radius
                  , parent_direction
                  , joint
                  , p1s
                  , 0
                  );
    std::cerr << "e";

    osg::ref_ptr<osg::Vec3Array> p2s(new osg::Vec3Array(radial_segments));
    transform_ring( unit_ring.get()
                  , child_radius
                  , child_direction
                  , joint
                  , p2s
                  , 0
                  );
    std::cerr << "f";

    osg::ref_ptr<osg::Vec3Array> p3s(new osg::Vec3Array(radial_segments));
    transform_ring( unit_ring.get()
                  , child_radius
                  , child_direction
                  , end_point
                  , p3s
                  , 0
                  );
    std::cerr << "g";



    transform_ring( unit_ring.get()
                  , parent_radius
                  , parent_direction
                  , parent_proximal
                  , parent_vertices.get()
                  , 0
                  );


    // transform_ring( unit_ring.get()
    //               , 1.0f
    //               , parent_direction
    //               , parent_proximal
    //               , parent_vertices
    //               , 0
    //               );
    // transform_ring( unit_ring.get()
    //               , parent_radius
    //               , parent_direction
    //               , joint
    //               , parent_vertices
    //               , unit_ring -> size()
    //               );

    // for(size_t i = 0; i < radial_segments; ++i)
    // {
    //     (*parent_vertices)[i]                   = (*p0s.get())[i];
    //     (*parent_vertices)[i + radial_segments] = (*p1s.get())[i];
    //     (*parent_vertices)[i + 2 * radial_segments] = (*p2s.get())[i];
    //     (*parent_vertices)[i + 3 * radial_segments] = (*p3s.get())[i];

    // }
    // connect_vertices( parent_vertices.get()
    //                 , parent_indices.get()
    //                 , 2
    //                 , radial_segments
    //                 );


    std::cerr << "h";
    float t;
    size_t i;
    size_t j;
    for(i = 0; i <= axial_segments / 2; ++i)
    {
        t = float(i) / float(axial_segments);
        generate_ring( p0s.get()
                     , p1s.get()
                     , p2s.get()
                     , p3s.get()
                     , t
                     , parent_vertices.get()
                     , parent_normals.get()
                     , radial_segments + i * radial_segments
                     );
    }

    for(i = i - 1,j = 0; i <= axial_segments; ++i, ++j)
    {
        t = float(i) / float(axial_segments);
        generate_ring( p0s.get()
                     , p1s.get()
                     , p2s.get()
                     , p3s.get()
                     , t
                     , child_vertices.get()
                     , child_normals.get()
                     , j * radial_segments
                     );
    }
    std::cerr << "j" << std::endl;
    // std::cerr << radial_segments + j * radial_segments << std::endl;
    // std::cerr << child_vertices -> size() << std::endl;
    transform_ring( unit_ring.get()
                  , child_radius
                  , child_direction
                  , child_distal
                  , child_vertices.get()
                  , j * radial_segments
                  );
    std::cerr << "k";

    connect_vertices( parent_vertices.get()
                    , parent_indices.get()
                    , axial_segments
                    , radial_segments
                    );
    // std::cerr << "l";

    connect_vertices( child_vertices.get()
                    , child_indices.get()
                    , axial_segments
                    , radial_segments
                    );

    generate_normals( parent_vertices.get()
                    , parent_normals.get()
                    , axial_segments
                    , radial_segments
                    );
    // std::cerr << "l";

    generate_normals( child_vertices.get()
                    , child_normals.get()
                    , axial_segments
                    , radial_segments
                    );
    std::cerr << "m";

    // hide_vertices( parent_vertices.get()
    //              , axial_segments
    //              , radial_segments
    //              , 1
    //              );

    // hide_vertices( child_vertices.get()
    //              , axial_segments
    //              , radial_segments
    //              , 1
    //              );



    parent_geometry -> setVertexArray(parent_vertices.get());
    parent_geometry -> addPrimitiveSet(parent_indices.get());//   );

    parent_geometry -> setNormalArray( parent_normals.get(), osg::Array::BIND_PER_VERTEX);
    // parent_geometry -> addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, parent_vertices -> size()));//   parent_indices.get());

    parent_geometry -> setColorArray(parent_colors);
    parent_geometry -> setColorBinding( osg::Geometry::BIND_OVERALL );

    child_geometry -> setVertexArray(child_vertices.get());
    child_geometry -> addPrimitiveSet(child_indices.get());

    // child_geometry -> addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, child_vertices -> size()));//   parent_indices.get());
    child_geometry -> setNormalArray( child_normals.get(), osg::Array::BIND_PER_VERTEX);

    child_geometry -> setColorArray(child_colors);
    child_geometry -> setColorBinding( osg::Geometry::BIND_OVERALL );

    // osgUtil::SmoothingVisitor::smooth(*parent_geometry);
    // osgUtil::SmoothingVisitor::smooth(*child_geometry);
}








// osg::Geode *
// common_axes()
// {
//     osg::Geode * geode = new osg::Geode();
//     osg::Geometry * cylinder1 = new osg::Geometry();
//     osg::Geometry * cylinder2 = new osg::Geometry();
//     smoothen( osg::Vec3f(0.0f, -10.0f, -10.0f)
//             , osg::Vec3f(0.0f, -10.0f, 10.0f)
//             , osg::Vec3f(0.0f, -10.0f, 10.0f)
//             , 5.0f
//             , 10.0f
//             , cylinder1
//             , cylinder2
//             );
//     geode -> addDrawable(cylinder1);
//     geode -> addDrawable(cylinder2);
//     return geode;
// }

int
main(int argc, char ** argv)
{
    osg::Group * group = new osg::Group();
    osg::Geode * geode = new osg::Geode();
    osg::Geometry * parent_geometry = new osg::Geometry();
    osg::Geometry * child_geometry = new osg::Geometry();

    smoothen( osg::Vec3f(0.0f, 0.0f, -20.0f)
            , osg::Vec3f(0.0f, 0.0f, 0.0f)
            , osg::Vec3f(0.0f, 0.0f, 10.0f)
            , 2.0f
            , 20.0f
            , parent_geometry
            , child_geometry
            , 0.4
            , 0.4
            , 8
            , 20
            );

    // The fraction has to be based upon the angle and radius difference
    // if the angle is 0.0, then fraction can be quite small
    // if the angle is 90.0 then fraction has to be big
    // plus it can be different for different

    std::cerr << "1" << std::endl;
    geode -> addDrawable(parent_geometry);
    std::cerr << "2" << std::endl;
    geode -> addDrawable(child_geometry);
    group -> addChild(geode);
    osgViewer::Viewer viewer;
    std::cerr << "3" << std::endl;
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    std::cerr << "4" << std::endl;
    viewer.setSceneData(group);
    std::cerr << "5" << std::endl;
    viewer.setCameraManipulator( new osgGA::TrackballManipulator() );
    viewer.getCamera() -> setClearColor(osg::Vec4f(0.0f, 0.0f, 0.0f, 1.0f));



    std::cerr << "6" << std::endl;
    viewer.run();
    return 0;
}

/**

The Idea

Generate a basic circular ring.

parent_ring => rotate and translate in accordance with parent axis and distal position
child_ring  => circular ring rotated and translated in accordance with the child axis and proximal position

Generate intermediate rings -

Generate intermediate points on the axis using quadratic bezier
Generate intermediate direction vectors
Generate intermediate radii values using quadratic bezier curve
scale translate and rotate the basic circular ring using the above values
Join the rings above
rinse and repeat
*/
