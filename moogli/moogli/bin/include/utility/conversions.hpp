#ifndef __CONVERSIONS_HPP__
#define __CONVERSIONS_HPP__

#include "Python.h"
#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/Vec3d"
#include "osg/Vec4d"
#include <sip.h>
#include <vector>
#include <utility>

const sipAPIDef *
get_sip_api();

osg::Vec4d
pysequence_to_vec4d(PyObject * sequence);

osg::Vec4f
pysequence_to_vec4f(PyObject * sequence);

osg::Vec3d
pysequence_to_vec3d(PyObject * sequence);

osg::Vec3f
pysequence_to_vec3f(PyObject * sequence);

PyObject *
vec4f_to_pysequence(const osg::Vec4f & color);

std::pair<int, int>
pysequence_to_int_pair(PyObject * sequence);

std::pair<float, float>
pysequence_to_float_pair(PyObject * sequence);

std::vector<osg::Vec4f>
pysequence_to_vec4f_vector(PyObject * sequence);

#endif
