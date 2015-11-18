#include "meshes/CylinderMesh.cpp"

inline unsigned int
get_vertex_count()
{
  return _radial_segments * (_axial_segments + 1) * 2;
}

inline unsigned int
get_normal_count()
{
  return get_vertex_count();
}

inline unsigned int
get_color_count()
{
  return _axial_segments + 1;
}

inline unsigned int
get_triangle_count()
{
  return (_radial_segments * 2) * _axial_segments + 2 * (_radial_segments - 2);
}

inline unsigned int
get_index_count()
{
  return get_triangle_count() * 3;
}

void
CylinderMesh::allocate()
{
  _allocation_required = false;
  osg::Vec3Array * V = static_cast<osg::Vec3Array *>(getVertexArray());
  unsigned int vertex_count = V -> getNumElements();
  if (vertex_count == radial_segments * (axial_segments + 1) { return; }
  setVertexArray(new osg::Vec3Array(radial_segments * (axial_segments + 1)));
  setNormalArray(new osg::Vec3Array(radial_segments * (axial_segments + 3)));
  setColorArray(new osg::Vec4Array(axial_segments + 1));
  addPrimitiveSet(new DrawElementsUShort(GL_TRIANGLES, (axial_segments + 1) * radial_segments));
}

void
CylinderMesh::construct()
{
  osg::Vec3Array * V = static_cast<osg::Vec3Array *>(getVertexArray());
  osg::Vec3Array * N = static_cast<osg::Vec3Array *>(getNormalArray());
  osg::DrawElementsUShort * I = static_cast<osg::DrawElementsUShort *>(getPrimitiveSet(0));

  for(unsigned int i = 0; i <= _axial_segments; ++i)
  {
     fill_regular_polygon( V
                         , i * 2 * _radial_segments
                         , _radial_segments
                         , get_radius()
                         , get_direction()
                         , get_base() + get_direction() * get_length() * i / _axial_segments
                         );
     copy(V, i * 2 * _radial_segments, i * 3 * radial_segments, radial_segments);
  }

  V -> dirty();
  N -> dirty();
  I -> dirty();
  dirtyBound();
}

void
  CylinderMesh::color()
{
  C -> dirty();
}

void
set_radial_segments(unsigned int radial_segments)
{
  if _radial_segments == radial_segments && _axial_segments 
}

void
set_axial_segments(unsigned int axial_segments)
{
  if(_axial_segments == axial_segments) { return; }
  _axial_segments = axial_segments;
  _allocation_required = true;
  _construction_required = true;
  _colorint_required = true;
}

void
set_radial_segments(unsigned int radial_segments)
{
   if(_radial_segments == axial_segments) { return; }
   _axial_segments = axial_segments;
   _allocation_required = true;
   _construction_required = true;
   _coloring_required = true;  
}

void
set_color(osg::Vec4f color)
{
    
}

void
set_colors( const osg::Vec4 & base_color
          , const osg::Vec4 & apex_color
          )
{
    
}
