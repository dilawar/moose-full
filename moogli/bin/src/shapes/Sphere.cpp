#include "shapes/Sphere.hpp"

Sphere::Sphere(const string & id): Shape(id)
{
    set( osg::Vec3f(0.0f, 0.0f, 0.0f)
       , 1.0f
       , 20
       , osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f)
       );
    allocate();
    construct_indices();
    construct_vertices();
    this -> color();
}

Sphere::Sphere( const string & id
                      , const osg::Vec3f & center
                      , const float radius
                      , unsigned int vertices
                      , const osg::Vec4f & color
                      ) : Shape(id)
{
    set(center, radius, vertices, color);
    allocate();
    construct_indices();
    construct_vertices();
    this -> color();
}

void
Sphere::allocate()
{
    unsigned int ring_count = _vertices / 2 + 1;
    unsigned int vertex_count = _vertices * ring_count;
    _geometry -> setVertexArray(new osg::Vec3Array(vertex_count + 2));
    _geometry -> setNormalArray( new osg::Vec3Array(vertex_count + 2)
                              , osg::Array::BIND_PER_VERTEX
                              );
    _geometry -> setColorArray( new osg::Vec4Array(vertex_count)
                             , osg::Array::BIND_OVERALL
                             );
    _geometry -> addPrimitiveSet( new osg::DrawElementsUShort( GL_TRIANGLES
                                                            , (ring_count - 1) * 2 * _vertices * 3
                                                            )
                               );
    _allocation_required = false;

}

void
Sphere::construct_vertices()
{
    unsigned int i, j;
    unsigned int ring_count = _vertices / 2 + 1;
    unsigned int vertex_count = _vertices * ring_count;
    float phi, delta_phi = osg::PI / (ring_count - 1);
    float radius, height;
    osg::Vec3f vertex;
    const osg::Vec3Array * polygon = Shape::regular_polygon(_vertices);
    osg::Vec3Array * V = static_cast<osg::Vec3Array *>(_geometry -> getVertexArray());
    osg::Vec3Array * N = static_cast<osg::Vec3Array *>(_geometry -> getNormalArray());
    unsigned int vertex_offset = (ring_count - 1) * _vertices;

    (*V)[vertex_count] = _center + osg::Z_AXIS * _radius;
    (*V)[vertex_count + 1] = _center - osg::Z_AXIS * _radius;
    (*N)[vertex_count] = osg::Z_AXIS;
    (*N)[vertex_count + 1] = -osg::Z_AXIS;

    for(i = 0; i < _vertices; ++i)
    {
        (*V)[i] = _center + osg::Z_AXIS * _radius;
        (*V)[i + vertex_offset] = _center - osg::Z_AXIS * _radius;
    }
    for(i = 0; i < ring_count - 2; ++i)
    {
        phi = (i + 1) * delta_phi;
        radius = _radius * sin(phi);
        height = _radius * cos(phi);
        for(j = 0; j < _vertices; ++j)
        {
            vertex = (*polygon)[j];
            (*V)[_vertices + i * _vertices + j] = (vertex * radius) + _center + osg::Vec3f(0.0f, 0.0f, height);
            (*N)[_vertices + i * _vertices + j] = osg::Vec3f( vertex.x()
                                                    , vertex.y()
                                                    , height
                                                    ) / sqrt(1 + height * height);
        }
    }
    for(i = 0; i < _vertices; ++i)
    {
        (*N)[i] = (*N)[i + _vertices];
        (*N)[i + vertex_offset] = (*N)[i + vertex_offset - _vertices];
    }
    _construction_required = false;
}

void
Sphere::construct_indices()
{
    unsigned int i, j, k, l, m, n, o, p;
    unsigned int ring_count = _vertices / 2 + 1;
    unsigned int vertex_count = _vertices * ring_count;
    unsigned int ring_triangle_count = 2 * _vertices;
    unsigned int vertex_offset;
    unsigned int index_offset;
    osg::DrawElementsUShort * I = static_cast<osg::DrawElementsUShort *>(_geometry -> getPrimitiveSet(0));

    const osg::DrawElementsUShort * axial_triangle_indices = Shape::axial_triangle(_vertices);
    // for(i = 0; i < _vertices; ++i)
    // {
    //     j = i * 3;
    //     k = j + 1;
    //     l = k + 1;
    //     (*I)[j] = 0;
    //     (*I)[k] = i + 1;
    //     (*I)[l] = i + 2;
    // }
    // (*I)[l] = 1;
    for(i = 0; i < ring_count - 1; ++i)
    {
        index_offset = 3 * i * ring_triangle_count;
        vertex_offset = i * _vertices;
        for(j = 0; j < _vertices; ++j)
        {
            k = j * 6;
            l = k + 1;
            m = l + 1;
            n = m + 1;
            o = n + 1;
            p = o + 1;
            (*I)[index_offset + k] = (*axial_triangle_indices)[k] + vertex_offset;
            (*I)[index_offset + l] = (*axial_triangle_indices)[l] + vertex_offset;
            (*I)[index_offset + m] = (*axial_triangle_indices)[m] + vertex_offset;
            (*I)[index_offset + n] = (*axial_triangle_indices)[n] + vertex_offset;
            (*I)[index_offset + o] = (*axial_triangle_indices)[o] + vertex_offset;
            (*I)[index_offset + p] = (*axial_triangle_indices)[p] + vertex_offset;
        }
    }
    // index_offset = (_vertices + (ring_count - 1) * 2 * _vertices) * 3;
    // vertex_offset = 1 + (ring_count - 1) * _vertices;
    // for(i = 0; i < _vertices; ++i)
    // {
    //     j = index_offset + i * 3;
    //     k = j + 1;
    //     l = k + 1;
    //     (*I)[j] = vertex_count - 1;
    //     (*I)[k] = vertex_offset + i + 1;
    //     (*I)[l] = vertex_offset + i;
    // }
    // (*I)[k] = vertex_offset;

    I -> dirty();
}

void
Sphere::color()
{
    osg::Vec4Array * colors = static_cast<osg::Vec4Array *>(_geometry -> getColorArray());
    (*colors)[0] = _color;
    colors -> dirty();
    _coloring_required = false;
}

void
Sphere::set_geometry( const osg::Vec3f & center
                        , float radius
                        , unsigned int vertices
                        )
{
    _center = center;
    _radius = radius;
    _vertices = vertices;
    _allocation_required = true;
    _construction_required = true;
}

void
Sphere::set( const osg::Vec3f & center
               , float radius
               , unsigned int vertices
               , const osg::Vec4f & color
               )
{
    set_geometry(center, radius, vertices);
    set_color(color);
}

void
Sphere::set_vertices(unsigned int vertices)
{
    _vertices = vertices;
    _allocation_required = true;
    _construction_required = true;
}

unsigned int
Sphere::get_vertices()
{
    return _vertices;
}

void
Sphere::set_center(const osg::Vec3f & center)
{
    _center = center;
    _construction_required = true;
}

osg::Vec3f
Sphere::get_center() const
{
    return _center;
}

void
Sphere::set_radius(const float radius)
{
    _radius = radius;
    _construction_required = true;
}

float
Sphere::get_radius() const
{
    return _radius;
}

void
Sphere::set_color(const osg::Vec4f & color)
{
    _color = color;
    _coloring_required = true;
}

const osg::Vec4f &
Sphere::get_color() const
{
    return _color;
}

void
Sphere::move_center_by(const osg::Vec3f & displacement)
{
    set_center(_center + displacement);
}
