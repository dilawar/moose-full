#include "shapes/Shape.hpp"

std::unordered_map<unsigned int, osg::ref_ptr<osg::Vec3Array> >MoogliShape::_polygons;
std::unordered_map<unsigned int, osg::ref_ptr<osg::DrawElementsUShort> >MoogliShape::_basal_triangles;
std::unordered_map<unsigned int, osg::ref_ptr<osg::DrawElementsUShort> >MoogliShape::_axial_triangles;
//std

const osg::Vec3Array *
MoogliShape::regular_polygon(unsigned int vertices)
{
    auto result =MoogliShape::_polygons.find(vertices);
    if(result !=MoogliShape::_polygons.end()) { return result -> second.get(); }

    osg::Vec3Array * polygon = new osg::Vec3Array(vertices);
    float theta = 0.0f;
    float theta_delta = osg::PI * 2.0f / vertices;
    unsigned int i;
    for(i = 0; i < vertices; ++i)
    {
        theta = i * theta_delta;
        (*polygon)[i].set(cos(theta), sin(theta), 0.0f);
    }
   MoogliShape::_polygons[vertices] = polygon;
    return polygon;
}

const osg::DrawElementsUShort *
MoogliShape::basal_triangle(unsigned int vertices)
{
    auto result =MoogliShape::_basal_triangles.find(vertices);
    if(result !=MoogliShape::_basal_triangles.end()) { return result -> second.get(); }

    unsigned int basal_triangle_count = vertices - 2;
    unsigned int basal_triangle_vertex_count = basal_triangle_count * 3;

    osg::DrawElementsUShort * basal_triangle = new osg::DrawElementsUShort(GL_TRIANGLES, basal_triangle_vertex_count);
    unsigned int i, j, k, l;
    for(i = 0; i < basal_triangle_count; ++i)
    {
        j = i * 3;
        k = j + 1;
        l = k + 1;
        (*basal_triangle)[j] = 0;
        (*basal_triangle)[k] = i + 2;
        (*basal_triangle)[l] = i + 1;
    }
   MoogliShape::_basal_triangles[vertices] = basal_triangle;
    return basal_triangle;
}

const osg::DrawElementsUShort *
MoogliShape::axial_triangle(unsigned int vertices)
{
    auto result =MoogliShape::_axial_triangles.find(vertices);
    if(result !=MoogliShape::_axial_triangles.end()) { return result -> second.get(); }

    unsigned int axial_triangle_count = 2 * vertices;
    unsigned int axial_triangle_vertex_count = axial_triangle_count * 3;
    unsigned int i, j, k, l, m, n, o;
    osg::DrawElementsUShort * axial_triangle = new osg::DrawElementsUShort(GL_TRIANGLES, axial_triangle_vertex_count);

    for(i = 0; i < vertices; ++i)
    {
        j = i * 6;
        k = j + 1;
        l = k + 1;
        m = l + 1;
        n = m + 1;
        o = n + 1;

        (*axial_triangle)[j] = i;
        (*axial_triangle)[k] = i + vertices;
        (*axial_triangle)[l] = i + 1;
        (*axial_triangle)[m] = i + vertices;
        (*axial_triangle)[n] = i + vertices + 1;
        (*axial_triangle)[o] = i + 1;
    }

    (*axial_triangle)[o] = 0;
    (*axial_triangle)[n] = vertices;
    (*axial_triangle)[l] = 0;

   MoogliShape::_axial_triangles[vertices] = axial_triangle;
    return axial_triangle;
}

MoogliShape::MoogliShape(const string & id): _outline(new osgFX::Outline())
                               , _cartoon(new osgFX::Cartoon())
                               , _wireframe(new osgFX::Scribe())
                               , _specular_highlights(new osgFX::SpecularHighlights())
                               , _geode(new osg::Geode())
                               , _geometry(new osg::Geometry())
                               , _allocation_required(true)
                               , _construction_required(true)
                               , _coloring_required(true)
{
    _outline -> setName(id);
    _outline -> addChild(_cartoon.get());
    _cartoon -> setName(id);
    _cartoon -> addChild(_wireframe.get());
    _wireframe -> setName(id);
    _wireframe -> addChild(_specular_highlights.get());
    _specular_highlights -> setName(id);
    _specular_highlights -> addChild(_geode.get());
    _geode -> setName(id);
    _geode -> addDrawable(_geometry.get());
    _geode -> getOrCreateStateSet() -> setMode(GL_BLEND,
                                               osg::StateAttribute::ON);
    _geode -> getOrCreateStateSet() -> setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    _geometry -> setName(id);
    _geometry -> setUpdateCallback(new MoogliShapeUpdateCallback(this));
    //setUpdateCallback(callback);
    _geometry -> setUseDisplayList(false);
    _geometry -> setUseVertexBufferObjects(true);
    disable_effects();
    root = _outline;
}

const std::string &
MoogliShape::id() const
{
    return _geometry -> getName();
}
// void
//MoogliShape::allocate()
// { }

// void
//MoogliShape::construct_indices()
// { }

// void
//MoogliShape::construct_vertices()
// { }

// void
//MoogliShape::color()
// { }

bool
MoogliShape::allocation_required() const
{
  return _allocation_required;
}

bool
MoogliShape::construction_required() const
{
  return _construction_required;
}

bool
MoogliShape::coloring_required() const
{
  return _coloring_required;
}

void
MoogliShape::hide()
{
    _geode -> setNodeMask(0x0);
}

void
MoogliShape::show()
{
    _geode -> setNodeMask(0xffffffff);
}

bool
MoogliShape::visible()
{
    return _geode -> getNodeMask();
}

bool
MoogliShape::export_geometry(const std::string & filename, bool with_effects) const
{
  return with_effects ? osgDB::writeNodeFile(*root.get(), filename)
                      : osgDB::writeNodeFile(*_geode.get(), filename);
}


void
MoogliShape::enable_effects()
{
    enable_cartoon_effect();
    enable_outline_effect();
    enable_wireframe_effect();
    enable_specular_highlights_effect();
}

void
MoogliShape::disable_effects()
{
    disable_cartoon_effect();
    disable_outline_effect();
    disable_wireframe_effect();
    disable_specular_highlights_effect();
}

void
MoogliShape::toggle_effects()
{
    toggle_cartoon_effect();
    toggle_outline_effect();
    toggle_wireframe_effect();
    toggle_specular_highlights_effect();
}

void
MoogliShape::enable_outline_effect()
{
    _outline -> setEnabled(true);
}

void
MoogliShape::disable_outline_effect()
{
    _outline -> setEnabled(false);
}

void
MoogliShape::toggle_outline_effect()
{
    _outline -> setEnabled(! _outline -> getEnabled());
}

bool
MoogliShape::outline_effect_enabled() const
{
    return _outline -> getEnabled();
}

void
MoogliShape::set_outline_effect_line_width(float width)
{
  _outline -> setWidth(width);
}

float
MoogliShape::get_outline_effect_line_width() const
{
    return _outline -> getWidth();
}

void
MoogliShape::set_outline_effect_line_color(const osg::Vec4f & color)
{
    _outline -> setColor(color);
}

const osg::Vec4f &
MoogliShape::get_outline_effect_line_color() const
{
    return _outline -> getColor();
}

void
MoogliShape::enable_cartoon_effect()
{
    _cartoon -> setEnabled(true);
}

void
MoogliShape::disable_cartoon_effect()
{
    _cartoon -> setEnabled(false);
}

void
MoogliShape::toggle_cartoon_effect()
{
    _cartoon -> setEnabled(! _cartoon -> getEnabled());
}

bool
MoogliShape::cartoon_effect_enabled() const
{
    return _cartoon -> getEnabled();
}

void
MoogliShape::set_cartoon_effect_outline_width(float width)
{
    _cartoon -> setOutlineLineWidth(width);
}

float
MoogliShape::get_cartoon_effect_outline_width() const
{
    return _cartoon -> getOutlineLineWidth();
}

void
MoogliShape::set_cartoon_effect_outline_color(const osg::Vec4f & color)
{
  _cartoon -> setOutlineColor(color);
}

const osg::Vec4f &
MoogliShape::get_cartoon_effect_outline_color() const
{
  return _cartoon -> getOutlineColor();
}


void
MoogliShape::enable_wireframe_effect()
{
    _wireframe -> setEnabled(true);
}

void
MoogliShape::disable_wireframe_effect()
{
    _wireframe -> setEnabled(false);
}

void
MoogliShape::toggle_wireframe_effect()
{
    _wireframe -> setEnabled(! _wireframe -> getEnabled());
}

bool
MoogliShape::wireframe_effect_enabled() const
{
  return _wireframe -> getEnabled();
}

void
MoogliShape::set_wireframe_effect_line_width(float width)
{
    _wireframe -> setWireframeLineWidth(width);
}

float
MoogliShape::get_wireframe_effect_line_width() const
{
  return _wireframe -> getWireframeLineWidth();
}

void
MoogliShape::set_wireframe_effect_line_color(const osg::Vec4f & color)
{
    _wireframe -> setWireframeColor(color);
}

const osg::Vec4f &
MoogliShape::get_wireframe_effect_line_color() const
{
    return _wireframe -> getWireframeColor();
}


void
MoogliShape::enable_specular_highlights_effect()
{
    _specular_highlights -> setEnabled(true);
}

void
MoogliShape::disable_specular_highlights_effect()
{
    _specular_highlights -> setEnabled(false);
}

void
MoogliShape::toggle_specular_highlights_effect()
{
    _specular_highlights -> setEnabled(! _specular_highlights -> getEnabled());
}

bool
MoogliShape::specular_highlights_effect_enabled() const
{
    return _specular_highlights -> getEnabled();
}

void
MoogliShape::set_specular_highlights_effect_exponent(float exponent)
{
    _specular_highlights -> setSpecularExponent(exponent);
}

float
MoogliShape::get_specular_highlights_effect_exponent() const
{
    return _specular_highlights -> getSpecularExponent();
}

void
MoogliShape::get_specular_highlights_effect_color(const osg::Vec4f & color)
{
    _specular_highlights -> setSpecularColor(color);
}

const osg::Vec4f &
MoogliShape::get_specular_highlights_effect_color() const
{
    return _specular_highlights -> getSpecularColor();
}


MoogliShape::MoogliShapeUpdateCallback::MoogliShapeUpdateCallback(MoogliShape * shape): UpdateCallback()
                                                              , shape(shape)
{ }

void
MoogliShape::MoogliShapeUpdateCallback::update( osg::NodeVisitor *nv
                                  , osg::Drawable *drawable
                                  )
{
    if(shape -> allocation_required())
    {
        shape -> allocate();
        shape -> construct_indices();
        shape -> construct_vertices();
        shape -> color();
    }
    if(shape -> construction_required())
    {
        shape -> construct_vertices();
    }
    if(shape -> coloring_required())
    {
        shape -> color();
    }
}



// void
//MoogliShape::fill_rotated_regular_polygon( osg::Vec3Array * V
//                             , const unsigned int index
//                             , const unsigned int vertices
//                             , const float radius
//                             , const osg::Vec3f & direction
//                             , const osg::Vec3f & center
//                             )
// {
//   osg::Vec3Array * polygon = regular_polygon(vertices);
//   osg::Quat rotation(osg::Z_AXIS, direction);
//   for(unsigned int i = 0; i < vertices; ++i)
//     {
//       (*V)[index + i] = ((*polygon)[i] * radius) * rotation + center;
//     }
// }

// void
//MoogliShape::transform_regular_polygon( osg::Vec3Array * V
//                                  , const unsigned int index
//                                  , const unsigned int vertices
//                                  , const float radius
//                                  , const osg::Vec3f & direction
//                                  , const osg::Vec3f & center
//                                  )
// {
//   osg::Vec3Array * polygon = regular_polygon(vertices);
//   osg::Quat rotation(osg::Z_AXIS, direction);
//   for(unsigned int i = 0; i < vertices; ++i)
//     {
//       (*V)[index + i] = ((*polygon)[i] * radius) * rotation + center;
//     }
// }


// void
//MoogliShape::transform_regular_polygon( osg::Vec3Array * V
//                                , const unsigned int index
//                                , const unsigned int vertices
//                                , const float radius
//                                , const osg::Vec3f & direction
//                                , const osg::Vec3f & center
//                                )
// {
//   osg::Vec3Array * polygon = regular_polygon(vertices);
//   osg::Quat rotation(osg::Z_AXIS, direction);
//   for(unsigned int i = 0; i < vertices; ++i)
//   {
//     (*V)[index + i] = ((*polygon)[i] * radius) * rotation + center;
//   }
// }

