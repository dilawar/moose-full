#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "shapes/Shape.hpp"
#include "utility/libraries.hpp"

class Sphere : public Shape
{
public:

    Sphere(const string & id);

    Sphere( const string & id
          , const osg::Vec3f & center
          , const float radius
          , unsigned int vertices
          , const osg::Vec4f & color
          );
    void
    allocate();

    void
    construct_indices();

    void
    construct_vertices();

    void
    color();

    void
    set_geometry( const osg::Vec3f & center
                , float radius
                , unsigned int vertices
                );

    void
    set( const osg::Vec3f & center
       , float radius
       , unsigned int vertices
       , const osg::Vec4f & color
       );

    void
    set_vertices(unsigned int vertices);

    unsigned int
    get_vertices();

    void
    set_center(const osg::Vec3f & center);

    osg::Vec3f
    get_center() const;

    void
    set_radius(float radius);

    float
    get_radius() const;

    const osg::Vec4f &
    get_color() const;

    void
    set_color(const osg::Vec4f & color);

    void
    move_center_by(const osg::Vec3f & displacement);

private:
    osg::Vec3f _center;
    float _radius;
    unsigned int _vertices;
    osg::Vec4f _color;
};

#endif /* __SPHERE_HPP__ */
