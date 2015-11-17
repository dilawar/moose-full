#include "utility/libraries.hpp"
#include "shapes/Shape.hpp"

class Cuboid : public Shape
{
public:
    Cuboid(const string & id);

    Cuboid( const string & id
          , const osg::Vec3f & center
          , const osg::Vec3f & axis
          , float length
          , float breadth
          , float height
          , const osg::Vec4f & top_face_color
          , const osg::Vec4f & bottom_face_color
          , const osg::Vec4f & left_face_color
          , const osg::Vec4f & right_face_color
          , const osg::Vec4f & front_face_color
          , const osg::Vec4f & back_face_color
          );

  void
  set_colors( const osg::Vec4f & top_face_color
            , const osg::Vec4f & bottom_face_color
            , const osg::Vec4f & left_face_color
            , const osg::Vec4f & right_face_color
            , const osg::Vec4f & front_face_color
            , const osg::Vec4f & back_face_color
            )
  {
      _top_face_color = top_face_color;
      _bottom_face_color = bottom_face_color;
      _left_face_color = left_face_color;
      _right_face_color = right_face_color;
      _front_face_color = front_face_color;
      _back_face_color = back_face_color;
      _coloring_required = true;
  }

  void
  set_color(const osg::Vec4f & color)
  {
    set_colors(color, color, color, color, color, color);
  }

  void
  set_top_face_color();

  void
  set_bottom_face_color();

  void
  set_left_face_color();

  void
  set_right_face_color();

  void
  set_front_face_color();

  void
  set_back_face_color();

  const string &
  id();

  void
  set();

  void
  set_geometry();

  void
  set_length(float length);

  void
  set_breadth(float breadth);

  void
  set_height(float height);

  void
  set_center(const osg::Vec3f & center);

  void
  set_axis(const osg::Vec3f & axis);

  float
  get_length();

  float
  get_breadth();

  float
  get_height();

  const osg::Vec3f &
  get_center()

    const osg::Vec3f &
    get_axis();

private:
  const std::string _id;
  float _length;
  float _breadth;
  float _height;
  osg::Vec3f _center;
  osg::Vec3f _axis;
};
