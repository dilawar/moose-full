#include "utility/conversions.hpp"
#include <sip.h>

const sipAPIDef *get_sip_api()
{
#if defined(SIP_USE_PYCAPSULE)
    return (const sipAPIDef *)PyCapsule_Import("sip._C_API", 0);
#else
    PyObject *sip_module;
    PyObject *sip_module_dict;
    PyObject *c_api;

    /* Import the SIP module. */
    sip_module = PyImport_ImportModule("sip");

    if (sip_module == NULL)
        return NULL;

    /* Get the module's dictionary. */
    sip_module_dict = PyModule_GetDict(sip_module);

    /* Get the "_C_API" attribute. */
    c_api = PyDict_GetItemString(sip_module_dict, "_C_API");

    if (c_api == NULL)
        return NULL;

    /* Sanity check that it is the right type. */
    if (!PyCObject_Check(c_api))
        return NULL;

    /* Get the actual pointer from the object. */
    return (const sipAPIDef *)PyCObject_AsVoidPtr(c_api);
#endif
}
osg::Vec4d
pysequence_to_vec4d(PyObject * sequence)
{
    // if(not PyList_Check(sequence))
    // {
    //     return osg::Vec4d();
    // }
    //
  return osg::Vec4d( PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 0))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 1))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 2))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 3))
                     );
}

PyObject *
vec4f_to_pysequence(const osg::Vec4f & color)
{
    PyObject * color_seq = PyTuple_New(4);
    PyTuple_SetItem(color_seq, 0, PyFloat_FromDouble(color[0]));
    PyTuple_SetItem(color_seq, 1, PyFloat_FromDouble(color[1]));
    PyTuple_SetItem(color_seq, 2, PyFloat_FromDouble(color[2]));
    PyTuple_SetItem(color_seq, 3, PyFloat_FromDouble(color[3]));
    return color_seq;
}

osg::Vec4f
pysequence_to_vec4f(PyObject * sequence)
{
    // if(not PyList_Check(sequence))
    // {
    //     return osg::Vec4f();
    // }

    return osg::Vec4f( static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 0))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 1))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 2))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 3))
                                         )
                     );
}

osg::Vec3d
pysequence_to_vec3d(PyObject * sequence)
{
    // if(not PyList_Check(sequence))
    // {
    //     return osg::Vec3d();
    // }

    //
    return osg::Vec3d( PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 0))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 1))
                     , PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 2))
                     );
}


osg::Vec3f
pysequence_to_vec3f(PyObject * sequence)
{
    // if(not PyList_Check(sequence))
    // {
    //     return osg::Vec3f();
    // }
    // // 
    return osg::Vec3f( static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 0))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 1))
                                         )
                     , static_cast<float>(
                        PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 2))
                                         )
                     );
}

std::pair<int, int>
pysequence_to_int_pair(PyObject * sequence)
{
    int x = static_cast<int>(PyLong_AsLong(PySequence_GetItem(sequence, 0)));
    int y = static_cast<int>(PyLong_AsLong(PySequence_GetItem(sequence, 1)));
    return std::make_pair(x, y);
}

std::pair<float, float>
pysequence_to_float_pair(PyObject * sequence)
{
  float x = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 0)));
  float y = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_GetItem(sequence, 1)));
  return std::make_pair(x, y);
}


std::vector<osg::Vec4f>
pysequence_to_vec4f_vector(PyObject * sequence)
{
    unsigned int size = PySequence_Size(sequence);
    std::vector<osg::Vec4f> colors(size);
    for(unsigned int i = 0; i < size; ++i)
    {
        colors[i] = pysequence_to_vec4f(PySequence_GetItem(sequence, i));
    }
    return colors;
}
