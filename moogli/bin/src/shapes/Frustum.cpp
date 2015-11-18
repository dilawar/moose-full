#include "shapes/Frustum.hpp"

Frustum::Frustum(const string & id) : MoogliShape(id)
{
    set( osg::Vec3f(0.0f, 0.0f, 0.0f),
         osg::Z_AXIS,
         10.0f,
         5.0f,
         2.0f,
         10,
         osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f),
         osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f)
       );
    allocate();
    construct_indices();
    construct_vertices();
    this -> color();
}


Frustum::Frustum( const string & id
            , const osg::Vec3f & center
            , const osg::Vec3f & axis
            , float length
            , float base_radius
            , float apex_radius
            , unsigned int vertices
            , const osg::Vec4f & base_color
            , const osg::Vec4f & apex_color
            ) : MoogliShape(id)
{
    set(center, axis, length, base_radius, apex_radius, vertices, base_color, apex_color);
    allocate();
    construct_indices();
    construct_vertices();
    this -> color();
}

Frustum::Frustum( const string & id
             , const osg::Vec3f & base
             , const osg::Vec3f & apex
             , float base_radius
             , float apex_radius
             , unsigned int vertices
             , const osg::Vec4f & base_color
             , const osg::Vec4f & apex_color
             ) : MoogliShape(id)
{
    set(base, apex, base_radius, apex_radius, vertices, base_color, apex_color);
    allocate();
    construct_indices();
    construct_vertices();
    this -> color();
}


void
Frustum::set_geometry( const osg::Vec3f & center
              , const osg::Vec3f & axis
              , float length
              , float base_radius
              , float apex_radius
              , unsigned int vertices
              )
{
    _center = center;
    _axis   = axis;
    _length = length;
    _base = _center - _axis * _length / 2.0f;
    _apex = _center + _axis * _length / 2.0f;
    _base_radius = base_radius;
    _apex_radius = apex_radius;
    _vertices = vertices;
    _allocation_required = true;
    _construction_required = true;
}

void
Frustum::set_geometry( const osg::Vec3f & base
            , const osg::Vec3f & apex
            , float base_radius
            , float apex_radius
            , unsigned int vertices
            )
{
  _base = base;
  _apex = apex;
  _axis = _apex - _base;
  _length = _axis.normalize();
  _center = (_apex + _base) / 2.0;
  _base_radius = base_radius;
  _apex_radius = apex_radius;
  _vertices = vertices;
  _allocation_required = true;
  _construction_required = true;
}

void
Frustum::set( const osg::Vec3f & center
     , const osg::Vec3f & axis
     , float length
     , float base_radius
     , float apex_radius
     , unsigned int vertices
     , const osg::Vec4f & base_color
     , const osg::Vec4f & apex_color
     )
{
    set_geometry(center, axis, length, base_radius, apex_radius, vertices);
    set_colors(base_color, apex_color);
}

void
Frustum::set( const osg::Vec3f & base
   , const osg::Vec3f & apex
     , float base_radius
     , float apex_radius
     , unsigned int vertices
     , const osg::Vec4f & base_color
     , const osg::Vec4f & apex_color
     )
{
    set_geometry(base, apex, base_radius, apex_radius, vertices);
    set_colors(base_color, apex_color);
}

void
Frustum::set_vertices(unsigned int vertices)
{
    _vertices = vertices;
    _allocation_required = true;
    _construction_required = true;
}

unsigned int
Frustum::get_vertices()
{
    return _vertices;
}

void
Frustum::set_axis(const osg::Vec3f & axis)
{
    _axis = axis;
    _base = _center - _axis * _length / 2.0f;
    _apex = _center + _axis * _length / 2.0f;
    _construction_required = true;
}

osg::Vec3f
Frustum::get_axis() const
{
    return _axis;
}

void
Frustum::set_length(const float & length)
{
    _length = length;
    _base = _center - _axis * _length / 2.0f;
    _apex = _center + _axis * _length / 2.0f;
    _construction_required = true;
}

float
Frustum::get_length() const
{
    return _length;
}

void
Frustum::set_center(const osg::Vec3f & center)
{
    _center = center;
    _base = _center - _axis * _length / 2.0f;
    _apex = _center + _axis * _length / 2.0f;
    _construction_required = true;
}

osg::Vec3f
Frustum::get_center() const
{
    return _center;
}

void
Frustum::set_apex(const osg::Vec3f & apex)
{
    _apex = apex;
    _axis = _apex - _base;
    _length = _axis.normalize();
    _center = (_apex + _base) / 2.0f;
    _construction_required = true;
}

osg::Vec3f
Frustum::get_apex() const
{
  return _apex;
}

void
Frustum::set_base(const osg::Vec3f & base)
{
    _base = base;
    _axis = _apex - _base;
    _length = _axis.normalize();
    _center = (_apex + _base) / 2.0f;
    _construction_required = true;
}

osg::Vec3f
Frustum::get_base() const
{
    return _base;
}

void
Frustum::set_base_radius(const float base_radius)
{
    _base_radius = base_radius;
    _construction_required = true;
}

float
Frustum::get_base_radius() const
{
    return _base_radius;
}

void
Frustum::set_apex_radius(const float apex_radius)
{
    _apex_radius = apex_radius;
    _construction_required = true;
}

float
Frustum::get_apex_radius() const
{
    return _apex_radius;
}

void
Frustum::set_radii(float base_radius, float apex_radius)
{
    set_base_radius(base_radius);
    set_apex_radius(apex_radius);
}

void
Frustum::set_radius(float radius)
{
    set_radii(radius, radius);
}

void
Frustum::set_base_color(const osg::Vec4f & base_color)
{
    _base_color = base_color;
    _coloring_required = true;
}

void
Frustum::set_apex_color(const osg::Vec4f & apex_color)
{
    _apex_color = apex_color;
    _coloring_required = true;
}

const osg::Vec4f &
Frustum::get_base_color() const
{
    return _base_color;
}

const osg::Vec4f &
Frustum::get_apex_color() const
{
    return _apex_color;
}

void
Frustum::set_color(const osg::Vec4f & color)
{
    set_colors(color, color);
}

void
Frustum::set_colors(const osg::Vec4f & base_color, const osg::Vec4f & apex_color)
{
    set_base_color(base_color);
    set_apex_color(apex_color);
}

void
Frustum::move_apex_by(float dl)
{
    move_apex_along(_axis, dl);
}

void
Frustum::move_base_by(float dl)
{
    move_base_along(_axis, dl);
}

void
Frustum::move_apex_along(const osg::Vec3f & direction, float dl)
{
    set_apex(_apex + direction * dl);
}

void
Frustum::move_base_along(const osg::Vec3f & direction, float dl)
{
    set_base(_base + direction * dl);
}

void
Frustum::move_center_by(const osg::Vec3f & displacement)
{
    set_center(_center + displacement);
}

void
Frustum::allocate()
{
    _geometry -> setVertexArray(new osg::Vec3Array(4 * _vertices));
    _geometry -> setNormalArray(new osg::Vec3Array(4 * _vertices), osg::Array::BIND_PER_VERTEX);
    _geometry -> setColorArray(new osg::Vec4Array(4 * _vertices), osg::Array::BIND_PER_VERTEX);
    _geometry -> addPrimitiveSet(new osg::DrawElementsUShort(GL_TRIANGLES, 12 * _vertices - 12));
    _allocation_required = false;
}


void
Frustum::construct_vertices()
{
     float z = (_base_radius - _apex_radius) / _length;
     float scale = 1.0f / sqrt(1 + z * z);
     osg::Quat rotation; rotation.makeRotate(osg::Z_AXIS, _axis);
     unsigned int i, j, k, l;
     osg::Vec3f vertex;
     const osg::Vec3Array * polygon = MoogliShape::regular_polygon(_vertices);
     osg::Vec3Array * V = static_cast<osg::Vec3Array *>(_geometry -> getVertexArray());
     osg::Vec3Array * N = static_cast<osg::Vec3Array *>(_geometry -> getNormalArray());
     for(i = 0; i < _vertices; ++i)
     {
        j = i + _vertices;
        k = j + _vertices;
        l = k + _vertices;
        vertex = (*polygon)[i];
        (*V)[k] = (*V)[i] = rotation * (vertex * _base_radius) + _base;
        (*V)[l] = (*V)[j] = rotation * (vertex * _apex_radius) + _apex;

        (*N)[i] = -_axis;
        (*N)[j] = _axis;
        (*N)[l] = (*N)[k] = rotation * (osg::Vec3f(vertex.x(), vertex.y(), z) * scale);
    }
    V -> dirty();
    N -> dirty();
    _geometry -> dirtyBound();
    _construction_required = false;
}


void
Frustum::construct_indices()
{
    unsigned int i, j, k, l, m, n, o;
    unsigned int basal_triangle_count = _vertices - 2;
    unsigned int basal_triangle_vertex_count = basal_triangle_count * 3;
    unsigned int axial_triangle_count = _vertices * 2;
    unsigned int axial_triangle_vertex_count = axial_triangle_count * 3;

    osg::DrawElementsUShort * I = static_cast<osg::DrawElementsUShort *>(_geometry -> getPrimitiveSet(0));

    const osg::DrawElementsUShort * basal_triangle_indices = MoogliShape::basal_triangle(_vertices);
    const osg::DrawElementsUShort * axial_triangle_indices = MoogliShape::axial_triangle(_vertices);
    for(i = 0; i < basal_triangle_count; ++i)
    {
        j = i * 3;
        k = j + 1;
        l = k + 1;
        m = basal_triangle_vertex_count + j;
        n = m + 1;
        o = n + 1;

        (*I)[j] = (*basal_triangle_indices)[j];
        (*I)[k] = (*basal_triangle_indices)[k];
        (*I)[l] = (*basal_triangle_indices)[l];
        (*I)[m] = (*basal_triangle_indices)[j] + _vertices;
        (*I)[n] = (*basal_triangle_indices)[k] + _vertices;
        (*I)[o] = (*basal_triangle_indices)[l] + _vertices;
    }

    for(i = 0; i < axial_triangle_count; ++i)
    {
        j = i * 3;
        k = j + 1;
        l = k + 1;
        m = j + 2 * basal_triangle_vertex_count;
        n = m + 1;
        o = n + 1;
        (*I)[m] = (*axial_triangle_indices)[j] + 2 * _vertices;
        (*I)[n] = (*axial_triangle_indices)[k] + 2 * _vertices;
        (*I)[o] = (*axial_triangle_indices)[l] + 2 * _vertices;
    }
    I -> dirty();
}

void
Frustum::color()
{
    unsigned int i, j, k, l;
    osg::Vec4Array * colors = static_cast<osg::Vec4Array *>(_geometry -> getColorArray());
    for(i = 0; i < _vertices; ++i)
    {
      j = i + _vertices;
      k = j + _vertices;
      l = k + _vertices;
      (*colors)[k] = (*colors)[i] = _base_color;
      (*colors)[l] = (*colors)[j] = _apex_color;
    }
    colors -> dirty();
    _coloring_required = false;
}

void
Frustum::rotate_axis_along(const osg::Vec3f & rotation_axis, float angle)
{
    set_axis(osg::Quat(angle, rotation_axis) * _axis);
}
