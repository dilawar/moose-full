#ifndef __CONSTANTS_HPP__
#define __CONSTANTS_HPP__

const osg::Node::NodeMask NODE_SHOW_MASK = 0xffffffff;
const osg::Node::NodeMask NODE_HIDE_MASK = 0xfffffff0;

const unsigned int CYLINDRICAL_NEURON_POINTS      = 10;
const unsigned int SPHERICAL_NEURON_POINTS        = 10;
const unsigned int CYLINDRICAL_COMPARTMENT_POINTS = 10;
const unsigned int SPHERICAL_COMPARTMENT_POINTS   = 10;
const unsigned int CYLINDRICAL_VOXEL_POINTS       = 10;
const unsigned int SPHERICAL_VOXEL_POINTS         = 10;

const osg::Vec4f CYLINDRICAL_NEURON_COLOR      = osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f); //green
const osg::Vec4f SPHERICAL_NEURON_COLOR        = osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f); //blue
const osg::Vec4f CYLINDRICAL_COMPARTMENT_COLOR = osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f); //orange
const osg::Vec4f SPHERICAL_COMPARTMENT_COLOR   = osg::Vec4f(0.5f, 0.0f, 1.0f, 1.0f); //violet
const osg::Vec4f CYLINDRICAL_VOXEL_COLOR       = osg::Vec4f(0.0f, 1.0f, 1.0f, 1.0f); //yellow
const osg::Vec4f SPHERICAL_VOXEL_COLOR         = osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f); //red


// const osg::Vec4f RED
// const osg::Vec4f GREEN
// const osg::Vec4f BLUE
// const osg::Vec4f CYAN
// const osg::Vec4f MAGENTA
// const osg::Vec4f YELLOW
// const osg::Vec4f ORANGE
// const osg::Vec4f VIOLET

#endif /* __CONSTANTS_HPP__ */
