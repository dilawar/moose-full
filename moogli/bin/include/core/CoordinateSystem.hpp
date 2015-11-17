#ifndef __COORDINATE_SYSTEM_HPP__
#define __COORDINATE_SYSTEM_HPP__

#include "utility/libraries.hpp"

class OSG_EXPORT CoordinateSystem : public PositionAttitudeTransform
{
public :
    CoordinateSystem();

    CoordinateSystem( const CoordinateSystem & pat
                    , const CopyOp & copyop = CopyOp::SHALLOW_COPY
                    );
    virtual void accept(NodeVisitor& nv);
    osg::ref_ptr<osg::Camera> camera;
    osg::ref_ptr<osg::Geometry> plane;
};

#endif /* __COORDINATE_SYSTEM_HPP__ */
