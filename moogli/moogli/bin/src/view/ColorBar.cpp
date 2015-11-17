#include "view/ColorBar.hpp"

class ColorBar(MatrixTransform)
{
    void
      set_position(float x, float y);

    void
      set_size(float length, float width);

    void
      set_text_properties();

    void
    set_orientation();

    void
    recreate();

    void
    resize(osg::Viewport * viewport)
}
osg::StateSet * stateset = scalar_bar -> getOrCreateStateSet();
stateset -> setMode(GL_LIGHTING, osg::StateAttribute::OFF);
stateset -> setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
stateset -> setRenderBinDetails(11, "RenderBin");

pat_transform -> addChild(scalar_bar);
osg::PositionAttitudeTransform * pat_transform = new osg::PositionAttitudeTransform();
pat_transform -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
 
void
ColorBar::set_orientation(float angle)
{
    this -> _angle = angle;
    osg::Quat orientation;
    orientation.makeRotate(angle, 0.0f, 0.0f, 1.0f);
    this -> pat_transform -> setAttitude(quat);
}

osgSim::ScalarBar::TextProperties
ColorBar::get_text_properties()
{
    osgSim::ScalarBar::TextProperties text_properties;
    text_properties._fontFile = this -> label_font_file;
    text_properties._fontResolution = this -> label_font_resolution;
    text_properties._characterSize = this -> label_character_size;
    text_properties._color = this -> label_color;
    return text_properties;
}


void
ColorBar::compute_dimensions()
{

    float m = tan(_angle);
    float c = viewport -> y() * position.y() - m * viewport -> x() * position.x();


    if(m == 0.0f)
    {
      _max_length = viewport -> width();
      _max_breadth = viewport -> height();
      return;
    }

    else if (m == nan)
    {
      _max_length  = viewport -> height();
      _max_breadth = viewport -> width();
    }



}

void
ColorBar::redraw()
{
    float length = dimensions.first;
    float breadth = dimensions.second;
    _scalar_bar -> setNumColor(_num_colors);
    _scalar_bar -> setNumLabels(_num_labels);
    _scalar_bar -> setTitle(_title);
    _scalar_bar -> setAspectRatio(_breadth / _length);
    _scalar_bar -> setScalarsToColors( new osgSim::ColorRange( _scalar_range.first
                                                             , _scalar_range.second
                                                             , _colors
                                                             )
                                     );

    _scalar_bar -> setWidth(_length);
    _scalar_bar -> setTextProperties(get_text_properties);


    const BoundingBox & bounding_box = _scalar_bar -> getBoundingBox();
    length = bounding_box.xMax() - bounding_box.xMin();
    breadth = bounding_box.yMax() - bounding_box.yMin();
    float cosangle = cos(angle);
    float sinangle = sin(angle);
    pat_transform -> setPosition( position
                                - osg::Vec3f( length * cosangle / 2.0f - breadth * sinangle / 2.0f
                                            , length * sinangle / 2.0f + breadth * cosangle / 2.0f
                                            , 0.0f
                                            )
                                );
    osg::Quat orientation;
    orientation.makeRotate(angle, 0.0, 0.0, 1.0);
    _pat_transform -> setAttitude(orientation);
}

void
ColorBar::line_length( osg::Viewport * viewport
                     , float m
                     , float c
                     )
{
    float y_max = viewport -> y() + viewport -> height();
    float x_max = viewport -> x() + viewport -> width();
    float top_y = y_max;
    float top_x = (y_max - c) / m;
    float right_y = m * x_max + c;
    float bottom_x = (viewport -> y() - c) / m;
    float left_y = m * viewport -> x() + c;

    if((x_top > x_max) || (x_top < x_min))
    {
        
    }

    if(top_x > x_max)
    {
        top_boundary_y = m * x_max + c;
        top_boundary_x = x_max;
    }
}

void
ColorBar::resize(osg::Viewport * viewport)
{
  float x = viewport -> x() * this -> x;
  float y = viewport -> y() * this -> y;
  float length = viewport -> 
    this -> setPosition( position
                                - osg::Vec3f( length * cosangle / 2.0f - breadth * sinangle / 2.0f
                                              , length * sinangle / 2.0f + breadth * cosangle / 2.0f
                                              , 0.0f
                                              )
                                );
  this -> x 
}
