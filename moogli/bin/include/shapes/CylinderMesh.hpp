#ifndef _CYLINDER_MESH_HPP_
#define _CYLINDER_MESH_HPP_

#include <osg/Shape>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Vec3d>
#include <osg/ShapeDrawable>
#include <osg/ref_ptr>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>
#include <osg/LOD>

#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <vector>


#include "utility/record.hpp"
using namespace std;
using namespace osg;

class CylinderMesh
{
public:

    CylinderMesh();

    float
    angle(Vec3f& vector1, Vec3f& vector2);

    void
    operator()( Vec3f        center
              , float        upper_radius
              , float        lower_radius
              , float        height
              , Vec3f        direction
              , Geometry  *  geometry
              , unsigned int points
              , const Vec4&  color
              , Vec3f        parent = osg::Vec3f()
              );

    Geometry *
    operator()( Vec3f        center
              , float        upper_radius
              , float        lower_radius
              , float        height
              , Vec3f        direction
              , unsigned int points
              , const Vec4&  color
              , Vec3f        parent = osg::Vec3f()
              );

    void
    operator()( Vec4f        distal
              , Vec4f        proximal
              , Geometry  *  geometry
              , unsigned int points
              , const Vec4&  color
              , Vec3f        parent = osg::Vec3f()
              );

    Geometry *
    operator()( Vec4f        distal
              , Vec4f        proximal
              , unsigned int points
              , const Vec4&  color
              , Vec3f        parent = osg::Vec3f()
              );


private:

    unordered_map< unsigned int
                 , const tuple< const Vec3Array * const
                              , const DrawElementsUShort * const
                              , const Vec3Array * const
                              >
                 > cylinders;

    const tuple< const Vec3Array * const
               , const DrawElementsUShort * const
               , const Vec3Array * const
               >
    unit(unsigned int points = 10);
};

#endif /* _CYLINDER_MESH_HPP_ */
