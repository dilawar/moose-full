#include "widgets/ColorBar.hpp"

ColorBar::LabelFormatter::LabelFormatter(unsigned int precision): _precision(precision)
{ }

std::string
ColorBar::LabelFormatter::printScalar(float scalar)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(_precision) << scalar;
    return ss.str();
}

unsigned int
ColorBar::LabelFormatter::get_precision() const
{
    return _precision;
}


ColorBar::ColorBar(const std::string & id): id(id)
                                          , scalar_bar( new osgSim::ScalarBar( 7
                                                                             , 7
                                                                             , new osgSim::ColorRange( 0.0
                                                                                                     , 1.0
                                                                                                     , { osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f)
                                                                                                       , osg::Vec4f(1.0f, 0.5f, 0.0f, 1.0f)
                                                                                                       , osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f)
                                                                                                       , osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f)
                                                                                                       , osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f)
                                                                                                       , osg::Vec4f(75.0f / 255.0f, 0.0f, 130.0f / 255.0f, 1.0f)
                                                                                                       , osg::Vec4f(139.0f / 255.0f, 0.0f, 1.0f, 1.0f)
                                                                                                       }
                                                                                                     )
                                                                             , "Rainbow Color Map"
                                                                             , osgSim::ScalarBar::HORIZONTAL
                                                                             , 1.0
                                                                             )
                                                        )
                                          , _position(osg::Vec2f(0.0, 0.0))
                                          , _size(osg::Vec2f(0.25, 0.25))
                                          , _orientation(0.0)
                                          , root(new osg::PositionAttitudeTransform())
                                          , _parent_viewport(osg::Vec4i(0, 0, 100, 100))

{
  scalar_bar -> setName(id);
  root -> setName(id);
  osgSim::ScalarBar::TextProperties text_properties;
  text_properties._fontFile = "";
  text_properties._fontResolution = std::make_pair<int, int>(40, 40);
  text_properties._characterSize = 0.0;
  text_properties._color = osg::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
  scalar_bar -> setTextProperties(text_properties);

  osg::StateSet * stateset = scalar_bar -> getOrCreateStateSet();
  stateset -> setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  stateset -> setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
  stateset -> setRenderBinDetails(11, "RenderBin");

  root -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
  root -> addChild(scalar_bar.get());
  resize();
}


ColorBar::ColorBar( const std::string & id
                  , const osg::Vec2f & scalar_range
                  , const std::vector<osg::Vec4f> & colors
                  , int label_count
                  , const std::string & title
                  , const osg::Vec2f & position
                  , const osg::Vec2f & size
                  , float orientation
                  , const std::string & text_font
                  , const osg::Vec2i text_font_resolution
                  , float text_character_size
                  , const osg::Vec4f & text_color
                  ): id(id)
                   , scalar_bar( new osgSim::ScalarBar( colors.size()
                                                      , label_count
                                                      , new osgSim::ColorRange(scalar_range.x(), scalar_range.y(), colors)
                                                      , title
                                                      , osgSim::ScalarBar::HORIZONTAL
                                                      , size.y() / size.x()
                                                      )
                               )
                   , _position(position)
                   , _size(size)
                   , _orientation(orientation)
                   , root(new osg::PositionAttitudeTransform())
                   , _parent_viewport(osg::Vec4i(0, 0, 100, 100))
{
    scalar_bar -> setName(id);
    root -> setName(id);
    osgSim::ScalarBar::TextProperties text_properties;
    text_properties._fontFile = text_font;
    text_properties._fontResolution = std::make_pair<int, int>(text_font_resolution.x(), text_font_resolution.y());
    text_properties._characterSize = text_character_size;
    text_properties._color = text_color;
    scalar_bar -> setTextProperties(text_properties);

    osg::StateSet * stateset = scalar_bar -> getOrCreateStateSet();
    stateset -> setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateset -> setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    stateset -> setRenderBinDetails(11, "RenderBin");

    root -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    root -> addChild(scalar_bar.get());
    resize();
}


void
ColorBar::resize()
{
  //std::cerr << "Width " << _parent_viewport.z() << std::endl;
  //std::cerr << "Height" << _parent_viewport.w() << std::endl;

  osg::Vec3f position( _parent_viewport.x() + _position.x() * _parent_viewport.z()
                     , _parent_viewport.y() + _position.y() * _parent_viewport.w()
                     , 0.0f
                     );
  //std::cerr << "Position " << position.x() << "," << position.y() << std::endl;

  osg::Vec3f size( _size.x() * _parent_viewport.z()
                 , _size.y() * _parent_viewport.w()
                 , 0.0f
                 );
  //std::cerr << "Size " << size.x() << "," << size.y() << std::endl;

  scalar_bar -> setWidth(size.x());
  scalar_bar -> setAspectRatio(size.y() / size.x());
  float cosangle = cos(_orientation);
  float sinangle = sin(_orientation);
  root -> setPosition( position
                     - osg::Vec3f( size.x() * cosangle / 2.0f - size.y() * sinangle / 2.0f
                                 , size.x() * sinangle / 2.0f + size.y() * cosangle / 2.0f
                                 , 0.0f
                                 )
                     );
  osg::Quat rotation;
  rotation.makeRotate(_orientation, 0.0, 0.0, 1.0);
  root -> setAttitude(rotation);
}

void
ColorBar::resize(int width, int height)
{
    resize(0, 0, width, height);
}

void
ColorBar::resize(int x, int y, int width, int height)
{
    set_parent_viewport(osg::Vec4i(x, y, width, height));
    resize();
}

void
ColorBar::set_parent_viewport(const osg::Vec4i & viewport)
{
  _parent_viewport = viewport;
}

void
ColorBar::set_scalar_range(const osg::Vec2f & scalar_range)
{
    scalar_bar -> setScalarsToColors(new osgSim::ColorRange( static_cast<int>(scalar_range.x())
                                                           , static_cast<int>(scalar_range.y())
                                                           , static_cast<const osgSim::ColorRange *>(scalar_bar -> getScalarsToColors()) -> getColors()
                                                           )
                                    );

}

osg::Vec2f
ColorBar::get_scalar_range() const
{
  return osg::Vec2f( scalar_bar -> getScalarsToColors() -> getMin()
                   , scalar_bar -> getScalarsToColors() -> getMax()
                   );
}

void
ColorBar::set_colors(const std::vector<osg::Vec4f> & colors)
{
  const osgSim::ScalarsToColors * s = scalar_bar -> getScalarsToColors();
  scalar_bar -> setScalarsToColors(new osgSim::ColorRange( s -> getMin()
                                                         , s -> getMax()
                                                         , colors
                                                         )
                                  );
  scalar_bar -> setNumColors(colors.size());
}

void
ColorBar::set_label_count(int label_count)
{
  scalar_bar -> setNumLabels(label_count);
}

int
ColorBar::get_label_count() const
{
  return scalar_bar -> getNumLabels();
}

void
ColorBar::set_title(const std::string & title)
{
  scalar_bar -> setTitle(title);
}

const std::string &
ColorBar::get_title () const
{
  return scalar_bar -> getTitle();
}

void
ColorBar::set_position(const osg::Vec2f & position)
{
  _position = position;
  resize();
}

const osg::Vec2f &
ColorBar::get_position() const
{
  return _position;
}

void
ColorBar::set_size(const osg::Vec2f & size)
{
  _size = size;
  resize();
}

const osg::Vec2f &
ColorBar::get_size() const
{
  return _size;
}

void
ColorBar::set_orientation(float orientation)
{
  _orientation = orientation;
  resize();
  //do transformation;
}

float
ColorBar::get_orientation() const
{
  return _orientation;
}

void
ColorBar::set_text_font(const std::string & font)
{
    osgSim::ScalarBar::TextProperties text_properties(scalar_bar -> getTextProperties());
    text_properties._fontFile = font;
    scalar_bar -> setTextProperties(text_properties);
}

const std::string &
ColorBar::get_text_font() const
{
  return scalar_bar -> getTextProperties()._fontFile;
}

void
ColorBar::set_text_font_resolution(const osg::Vec2i & font_resolution)
{
    osgSim::ScalarBar::TextProperties text_properties(scalar_bar -> getTextProperties());
    text_properties._fontResolution = std::make_pair( font_resolution.x()
                                                    , font_resolution.y()
                                                    );
    scalar_bar -> setTextProperties(text_properties);
}

osg::Vec2i
ColorBar::get_text_font_resolution() const
{
    const std::pair<int, int> & font_resolution = scalar_bar -> getTextProperties()._fontResolution;
    return osg::Vec2i(font_resolution.first, font_resolution.second);
}

void
ColorBar::set_text_character_size(float character_size)
{
  osgSim::ScalarBar::TextProperties text_properties(scalar_bar -> getTextProperties());
  text_properties._characterSize = character_size;
  scalar_bar -> setTextProperties(text_properties);
}

float
ColorBar::get_text_character_size() const
{
  return scalar_bar -> getTextProperties()._characterSize;
}

void
ColorBar::set_text_color(const osg::Vec4f & color)
{
    osgSim::ScalarBar::TextProperties text_properties(scalar_bar -> getTextProperties());
    text_properties._color = color;
    scalar_bar -> setTextProperties(text_properties);
}

const osg::Vec4f &
ColorBar::get_text_color() const
{
    return scalar_bar -> getTextProperties()._color;
}

void
ColorBar::set_label_formatting_precision(unsigned int precision)
{
    scalar_bar -> setScalarPrinter(new LabelFormatter(precision));
}

unsigned int
ColorBar::get_label_formatting_precision() const
{
  return static_cast<const LabelFormatter *>(scalar_bar -> getScalarPrinter()) -> get_precision();
}
