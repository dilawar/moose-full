#ifndef __RING_HPP__
#define __RING_HPP__

#include <osg/Vec3f>
#include <vector>
#include <osg/Geometry>
#include <iostream>

class Ring
{
private:
    std::vector<osg::Vec3Array *> _rings;

public:
    Ring();

    void
    unit( uint radial_segments
        , osg::Vec3Array * points
        , uint index
        );

    osg::Vec3Array *
    _unit(uint radial_segments);

    void
    operator()( const float        radius
              , const osg::Vec3f & direction
              , const osg::Vec3f & center
              , osg::Vec3Array   * points
              , uint               index
              );

    void
    operator()( const osg::Vec3f & center
              , const osg::Vec3f & direction
              , const float        radius
              , uint               radial_segments
              , osg::Vec3Array   * points
              , uint               index
              );

    void
    operator()( const osg::Vec3Array * centers
              , const osg::Vec3Array * directions
              , const std::vector<float>  & radii
              , uint                   radial_segments
              , osg::Vec3Array       * points
              , uint                   index
              , uint                   delta
              );
};

#endif /* __RING_HPP__ */
