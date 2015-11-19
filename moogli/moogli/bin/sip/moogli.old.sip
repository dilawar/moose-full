%Module _moogli

%Import QtGui/QtGuimod.sip
%Import QtOpenGL/QtOpenGLmod.sip

class Vec2f
{
%TypeHeaderCode
#include <osg/Vec2f>
using namespace osg;
%End
public:
 	  Vec2f();
 	  Vec2f(float x, float y);
    bool operator== (const Vec2f &v) const;
    bool operator!= (const Vec2f &v) const;
    bool operator< (const Vec2f &v) const;
    void set(float x, float y);
    float x() const;
    float y() const;
    bool valid() const;
    bool isNaN() const;
    float operator* (const Vec2f &rhs) const;
    const Vec2f	operator* (float rhs) const;
    const Vec2f	operator/ (float rhs) const;
    const Vec2f	operator+ (const Vec2f &rhs) const;
    const Vec2f	operator- (const Vec2f &rhs) const;
    const Vec2f	operator- () const;
    float length() const;
    float length2() const;
    float normalize();
};

class Vec3f
{
%TypeHeaderCode
#include <osg/Vec3f>
using namespace osg;
%End
public:
    Vec3f();
    Vec3f(const Vec2f &v2, float z);
 	  Vec3f(float x, float y, float z);
    bool operator== (const Vec3f &v) const;
    bool operator!= (const Vec3f &v) const;
    bool operator< (const Vec3f &v) const;
    void set(float x, float y, float z);
    void set(const Vec3f &rhs);
    float x() const;
    float y() const;
    float z() const;
    bool valid() const;
    bool isNaN () const;
    float operator* (const Vec3f &rhs) const;
    const Vec3f operator^ (const Vec3f &rhs) const;
    const Vec3f	operator* (float rhs) const;
    const Vec3f operator/ (float rhs) const;
    const Vec3f operator+ (const Vec3f &rhs) const;
    const Vec3f operator- (const Vec3f &rhs) const;
    const Vec3f operator- () const;
    float length() const;
    float length2() const;
    float normalize();
};

class Vec4f
{
%TypeHeaderCode
#include <osg/Vec4f>
using namespace osg;
%End
public:
 	  Vec4f();
 	  Vec4f(float x, float y, float z, float w);
 	  Vec4f(const Vec3f &v3, float w);
    bool operator== (const Vec4f &v) const;
    bool operator!= (const Vec4f &v) const;
    bool operator< (const Vec4f &v) const;
    void set(float x, float y, float z, float w);
    float x() const;
    float y() const;
    float z() const;
    float w() const;
    float r() const;
    float g() const;
    float b() const;
    float a() const;
    unsigned int asABGR() const;
    unsigned int asRGBA() const;
    bool valid() const;
    bool isNaN() const;
    float operator* (const Vec4f &rhs) const;
    Vec4f operator* (float rhs) const;
    Vec4f operator/ (float rhs) const;
    Vec4f operator+ (const Vec4f &rhs) const;
    Vec4f operator- (const Vec4f &rhs) const;
    const Vec4f operator- () const;
    float length() const;
    float length2() const;
    float normalize();
};

class Quat
{
%TypeHeaderCode
#include <osg/Quat>
using namespace osg;
%End
public:
    Quat();
 	  Quat(float x, float y, float z, float w);
 	  Quat(const Vec4f &v);
 	  Quat(float angle, const Vec3f &axis);
 	  Quat(float angle1, const Vec3f &axis1, float angle2, const Vec3f &axis2, float angle3, const Vec3f &axis3);
    bool operator== (const Quat &v) const;
    bool operator!= (const Quat &v) const;
    bool operator< (const Quat &v) const;
    Vec4f asVec4() const;
    Vec3f asVec3() const;
    void set(float x, float y, float z, float w);
    void set(const Vec4f &v);
    float x() const;
    float y() const;
    float z() const;
    float w() const;
    bool zeroRotation() const;
    const Quat operator* (float rhs) const;
    const Quat operator* (const Quat &rhs) const;
    Quat operator/ (float rhs) const;
    const Quat operator/ (const Quat &denom) const;
    const Quat operator+ (const Quat &rhs) const;
    const Quat operator- (const Quat &rhs) const;
    const Quat operator- () const;
    float length() const;
    float length2() const;
    Quat conj() const;
    const Quat inverse() const;
    void makeRotate(float angle, float x, float y, float z);
    void makeRotate(float angle, const Vec3f &vec);
    void makeRotate(float angle1, const Vec3f &axis1, float angle2, const Vec3f &axis2, float angle3, const Vec3f &axis3);
    void makeRotate(const Vec3f &vec1, const Vec3f &vec2);
    Vec3f operator* (const Vec3f &v) const;
};

class Mesh /Abstract/ 
{
%TypeHeaderCode
#include "mesh/Mesh.hpp"
%End

public:
    Mesh(const char * id);

    PyObject *
    get_id() const;
    %MethodCode
        sipRes = PyString_FromString(sipCpp -> getName().c_str());
    %End

    // Mesh *
    // get_parent() const;

    // void
    // set_parent(Mesh * parent /Transfer/);

    // SIP_PYTUPLE
    // subdivide(unsigned int n) const;
    // %MethodCode
    //     sipRes = PyTuple_New(n);
    //     SIP_SSIZE_T index = 0;
    //     for(const Mesh * mesh : sipCpp -> subdivide(a0))
    //     {
    //         PyTuple_SET_ITEM( sipRes
    //                         , index
    //                         , sipConvertFromType( mesh
    //                                             , sipFindType("Mesh")
    //                                             , NULL
    //                                             )
    //                         );
    //         index += 1;
    //     }
    // %End
};


class FrustumMesh : public Mesh
{
%TypeHeaderCode
#include "mesh/FrustumMesh.hpp"
%End

public:

    FrustumMesh(const char * id);

    FrustumMesh( const char * id
               , const Vec3f & center
               , const Vec3f & axis
               , float length
               , float base_radius
               , float apex_radius
               , unsigned int vertices
               , const Vec4f & base_color
               , const Vec4f & apex_color
               );

    FrustumMesh( const char * id
               , const Vec3f & base
               , const Vec3f & apex
               , float base_radius
               , float apex_radius
               , unsigned int vertices
               , const Vec4f & base_color
               , const Vec4f & apex_color
               );
    void
    set_geometry( const Vec3f & center
                , const Vec3f & axis
                , float length
                , float base_radius
                , float apex_radius
                , unsigned int vertices
                );

    void
    set_geometry( const Vec3f & base
                , const Vec3f & apex
                , float base_radius
                , float apex_radius
                , unsigned int vertices
                );

    void
    set( const Vec3f & center
       , const Vec3f & axis
       , float length
       , float base_radius
       , float apex_radius
       , unsigned int vertices
       , const Vec4f & base_color
       , const Vec4f & apex_color
       );

    void
    set( const Vec3f & base
       , const Vec3f & apex
       , float base_radius
       , float apex_radius
       , unsigned int vertices
       , const Vec4f & base_color
       , const Vec4f & apex_color
       );

    void
    set_vertices(unsigned int vertices);

    unsigned int
    get_vertices();

    void
    set_axis(const Vec3f & axis);

    Vec3f
    get_axis() const;

    void
    set_length(const float & length);

    float
    get_length() const;

    void
    set_center(const Vec3f & center);

    Vec3f
    get_center() const;

    void
    set_apex(const Vec3f & apex);

    Vec3f
    get_apex() const;

    void
    set_base(const Vec3f & base);

    Vec3f
    get_base() const;

    void
    set_base_radius(const float base_radius);

    float
    get_base_radius() const;

    void
    set_apex_radius(const float apex_radius);

    float
    get_apex_radius() const;

    void
    set_radii(float base_radius, float apex_radius);

    void
    set_radii(float radius);

    void
    set_base_color(const Vec4f & base_color);

    void
    set_apex_color(const Vec4f & apex_color);

    const Vec4f &
    get_base_color() const;

    const Vec4f &
    get_apex_color() const;

    void
    set_colors(const Vec4f & color);

    void
    set_colors(const Vec4f & base_color, const Vec4f & apex_color);

    void
    move_apex_by(float dl);

    void
    move_base_by(float dl);

    void
    move_apex_along(const Vec3f & direction, float dl);

    void
    move_base_along(const Vec3f & direction, float dl);

    void
    move_center_by(const Vec3f & displacement);

    void
    rotate_axis_along(const Vec3f & rotation_axis, float angle);

};


// %ModuleCode

// PySequence * ELECTRICAL_COMPARTMENT_COLOR
// {
//     %GetCode
//         sipPy = vec4f_to_pysequence(DEFAULT_ELECTRICAL_COMPARTMENT_COLOR);
//     %End
// }

// %End

class Visualizable /Abstract/
{
%TypeHeaderCode
#include "core/Visualizable.hpp"
%End
public:
    const char * id
    {
        %GetCode
            sipPy = PyString_FromString(sipCpp -> id.c_str());
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Cannot change id of object."
                           );
        %End
    };

    PyObject * proximal
    {
        %GetCode
            sipPy = PyTuple_New(4);
            PyTuple_SET_ITEM(sipPy, 0, PyFloat_FromDouble(sipCpp -> proximal[0]));
            PyTuple_SET_ITEM(sipPy, 1, PyFloat_FromDouble(sipCpp -> proximal[1]));
            PyTuple_SET_ITEM(sipPy, 2, PyFloat_FromDouble(sipCpp -> proximal[2]));
            PyTuple_SET_ITEM(sipPy, 3, PyFloat_FromDouble(sipCpp -> proximal[3]));
        %End
        %SetCode
        if(PySequence_Check(sipPy))
        {
            if(PySequence_Length(sipPy) == 4)
            {
                sipCpp -> proximal[0] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 0)));
                sipCpp -> proximal[1] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 1)));
                sipCpp -> proximal[2] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 2)));
                sipCpp -> proximal[3] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 3)));
            }
            else
            {
                sipErr = 1;
                PyErr_SetString( PyExc_IndexError
                               , "Expected a sequence of size 4, (x, y, z, r)"
                               );
            }
        }
        else
        {
            sipErr = 1;
            PyErr_SetString( PyExc_TypeError
                           , "Expected a sequence such as list, tuple or numpy array"
                           );
        }
        %End
    };

    PyObject * distal
    {
        %GetCode
            sipPy = PyTuple_New(4);
            PyTuple_SET_ITEM(sipPy, 0, PyFloat_FromDouble(sipCpp -> distal[0]));
            PyTuple_SET_ITEM(sipPy, 1, PyFloat_FromDouble(sipCpp -> distal[1]));
            PyTuple_SET_ITEM(sipPy, 2, PyFloat_FromDouble(sipCpp -> distal[2]));
            PyTuple_SET_ITEM(sipPy, 3, PyFloat_FromDouble(sipCpp -> distal[3]));
        %End
        %SetCode
        if(PySequence_Check(sipPy))
        {
            if(PySequence_Length(sipPy) == 4)
            {
                sipCpp -> distal[0] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 0)));
                sipCpp -> distal[1] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 1)));
                sipCpp -> distal[2] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 2)));
                sipCpp -> distal[3] = static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(sipPy, 3)));
            }
            else
            {
                sipErr = 1;
                PyErr_SetString( PyExc_IndexError
                               , "Expected a sequence of size 4, (x, y, z, r)"
                               );
            }
        }
        else
        {
            sipErr = 1;
            PyErr_SetString( PyExc_TypeError
                           , "Expected a sequence such as list, tuple or numpy array"
                           );
        }
        %End
    };

    void
    set_color(PyObject * color_seq, int index = -1);
    %MethodCode
        if(PySequence_Check(a0))
        {
            if(PySequence_Length(a0) == 4)
            {
                Vec4f color( static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 0)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 1)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 2)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 3)))
                           );
                sipCpp -> set_color(color, a1);
            }
            else
            {
                sipError = sipErrorFail;
                PyErr_SetString( PyExc_IndexError
                               , "Expected a sequence of size 4,(red, green, blue, alpha)"
                               );
            }
        }
        else
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_TypeError
                           , "Expected a sequence such as list, tuple or numpy array"
                           );
       }
    %End

    // PyObject *
    // get_color(int index = 1);
    // %MethodCode
    //     int max_index = static_cast<int>(sipCpp -> node -> getNumChildren());
    //     if(a0 < max_index)
    //     {
    //         Vec4f color = sipCpp -> get_color(a0);
    //         sipRes = PyTuple_New(4);
    //         PyTuple_SET_ITEM(sipRes, PyFloat_FromDouble(color[0]), 0)
    //         PyTuple_SET_ITEM(sipRes, PyFloat_FromDouble(color[1]), 1)
    //         PyTuple_SET_ITEM(sipRes, PyFloat_FromDouble(color[2]), 2)
    //         PyTuple_SET_ITEM(sipRes, PyFloat_FromDouble(color[3]), 3)
    //     }
    //     else
    //     {
    //         sipError = sipErrorFail;
    //         PyErr_SetString( PyExc_IndexError
    //                        , ("Expected an index < " + to_string(max_index)).c_str()
    //                        );
    //     }
    // %End

    void
    hide(int index);

    void
    show(int index);

    bool
    visible(int index);

    void
    add_representation(bool as_sphere = false);

    unsigned int
    representation_count();

    bool
    write(const char * filename);

    // double r
    // {
    //     %GetCode
    //         sipPy = PyFloat_FromDouble(sipCpp -> r);
    //     %End
    //     %SetCode
    //         if(PyFloat_Check(sipPy))
    //         {
    //             sipCpp -> r = PyFloat_AS_DOUBLE(sipPy);
    //         }
    //         else
    //         {
    //             sipErr = 1;
    //             PyErr_SetString( PyExc_TypeError
    //                            , "Expected a value of type float"
    //                            );
    //         }
    //     %End
    // };

    // double x
    // {
    //     %GetCode
    //         sipPy = PyFloat_FromDouble(sipCpp -> x);
    //     %End
    //     %SetCode
    //         if(PyFloat_Check(sipPy))
    //         {
    //             sipCpp -> x = PyFloat_AS_DOUBLE(sipPy);
    //         }
    //         else
    //         {
    //             sipErr = 1;
    //             PyErr_SetString( PyExc_TypeError
    //                            , "Expected a value of type float"
    //                            );
    //         }
    //     %End
    // };


    // double y
    // {
    //     %GetCode
    //         sipPy = PyFloat_FromDouble(sipCpp -> y);
    //     %End
    //     %SetCode
    //         if(PyFloat_Check(sipPy))
    //         {
    //             sipCpp -> y = PyFloat_AS_DOUBLE(sipPy);
    //         }
    //         else
    //         {
    //             sipErr = 1;
    //             PyErr_SetString( PyExc_TypeError
    //                            , "Expected a value of type float"
    //                            );
    //         }
    //     %End
    // };

    // double x
    // {
    //     %GetCode
    //         sipPy = PyFloat_FromDouble(sipCpp -> z);
    //     %End
    //     %SetCode
    //         if(PyFloat_Check(sipPy))
    //         {
    //             sipCpp -> z = PyFloat_AS_DOUBLE(sipPy);
    //         }
    //         else
    //         {
    //             sipErr = 1;
    //             PyErr_SetString( PyExc_TypeError
    //                            , "Expected a value of type float"
    //                            );
    //         }
    //     %End
    // };


    // Visualizable();
    // virtual const char *
    // get_id() = 0;//    virtual

    // virtual void
    // set_radius();
    /* data */
};


class Network
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Network.hpp"
#include "core/Neuron.hpp"
%End

public:

    const char * id
    {
        %GetCode
            sipPy = PyString_FromString(sipCpp -> id.c_str());
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Cannot change id of object."
                           );
        %End
    };

    Network(const char * id);

    ~Network();

    void
    hide();

    void
    show();

    bool
    visible() const;

    void
    attach(Neuron * neuron /Transfer/);
    %MethodCode
        if(!sipCpp -> attach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("Neuron(" + a0 -> id + ") is already attached to Network(" + sipCpp -> id + ").").c_str()
                           );
        }
    %End

    void
    detach(Neuron * neuron);
    %MethodCode
        if(!sipCpp -> detach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("Neuron(" + a0 -> id + ") is not attached to Network(" + sipCpp -> id + ").").c_str()
                           );
        }
    %End

    SIP_PYTUPLE
    neurons() const;
    %MethodCode
        sipRes = PyTuple_New(sipCpp -> size());
        SIP_SSIZE_T index = 0;
        for ( const auto &neuron_pair : sipCpp -> neuron_map )
        {
            PyTuple_SET_ITEM( sipRes
                            , index
                            , sipConvertFromType( neuron_pair.second
                                                , sipFindType("Neuron")
                                                , NULL
                                                )
                            );
            index += 1;
        }
    %End

    SIP_SSIZE_T
    size() const /__len__/;

    int
    __contains__(Neuron * neuron) const;
    %MethodCode
        return static_cast<int>(sipCpp -> get(a0 -> id) != nullptr);
    %End

    SIP_PYOBJECT
    __getitem__(const char * neuron_id) const;
    %MethodCode
        Neuron * neuron = sipCpp -> get(a0);
        if(neuron == nullptr)
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_KeyError
                           , a0
                           );
        }
        else
        {
            sipRes = sipConvertFromType( neuron
                                       , sipFindType("Neuron")
                                       , NULL
                                       );
        }
    %End

    bool
    write(const char * filename);

    SIP_PYOBJECT
    __str__() const;
    %MethodCode
        sipRes = PyString_FromString(("Network : " + sipCpp -> id).c_str());
    %End
};


class Neuron : public Visualizable
{

%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/ElectricalCompartment.hpp"
#include "core/Neuron.hpp"
%End

public:

    const char * network
    {
        %GetCode
            sipPy = sipConvertFromType( sipCpp -> network
                                      , sipType_Network
                                      , NULL
                                      );
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Use Network.attach to change the network this neuron belongs to"
                           );
        %End
    };

    Neuron(const char * id);
    ~Neuron();

    SIP_SSIZE_T
    size() const /__len__/;

    SIP_PYTUPLE
    electrical_compartments() const;
    %MethodCode
        sipRes = PyTuple_New(sipCpp -> size());
        SIP_SSIZE_T index = 0;
        for (const auto &electrical_compartment_pair : sipCpp -> electrical_compartment_map)
        {
            PyTuple_SET_ITEM( sipRes
                            , index
                            , sipConvertFromType( electrical_compartment_pair.second
                                                , sipFindType("ElectricalCompartment")
                                                , NULL
                                                )
                            );
            index += 1;
        }
    %End

    int
    __contains__(ElectricalCompartment * electrical_compartment) const;
    %MethodCode
        return static_cast<int>(sipCpp -> get(a0 -> id) != nullptr);
    %End

    SIP_PYOBJECT
    __getitem__(const char * electrical_compartment_id) const;
    %MethodCode
        ElectricalCompartment * electrical_compartment = sipCpp -> get(std::string(a0));
        if(electrical_compartment == nullptr)
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_KeyError
                           , a0
                           );
        }
        else
        {
            sipRes = sipConvertFromType( electrical_compartment
                                       , sipFindType("ElectricalCompartment")
                                       , NULL
                                       );
        }
    %End

    SIP_PYOBJECT
    __str__() const;
    %MethodCode
        sipRes = PyString_FromString(("Neuron : " + sipCpp -> id).c_str());
    %End

    void
    attach(ElectricalCompartment * electrical_compartment /Transfer/);
    %MethodCode
        if(!sipCpp -> attach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("ElectricalCompartment : " + a0 -> id + " is already attached to Neuron : " + sipCpp -> id).c_str()
                           );
        }
    %End

    void
    detach(ElectricalCompartment * electrical_compartment);
    %MethodCode
        if(!sipCpp -> detach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("ElectricalCompartment : " + a0 -> id + " is not attached to Neuron : " + sipCpp -> id).c_str()
                           );
        }
    %End
};




class ElectricalCompartment : public Visualizable
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/ElectricalCompartment.hpp"
#include "core/ChemicalCompartment.hpp"
%End

public:

    SIP_PYOBJECT neuron
    {
        %GetCode
            sipPy = sipConvertFromType( sipCpp -> neuron
                                      , sipFindType("Neuron")
                                      , NULL
                                      );
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Use Neuron.attach to change the neuron this electrical compartment belongs to"
                           );
        %End
    };

    SIP_PYOBJECT parent
    {
        %GetCode
            sipPy = sipConvertFromType( sipCpp -> parent
                                      , sipFindType("ElectricalCompartment")
                                      , NULL
                                      );
        %End
        %SetCode
            if(sipCanConvertToType(sipPy, sipFindType("ElectricalCompartment"), SIP_NO_CONVERTORS))
            {
                int state = 0;
                int iserr = 0;
                ElectricalCompartment * electrical_compartment =
                    static_cast<ElectricalCompartment *>( sipConvertToType( sipPy
                                                                          , sipFindType("ElectricalCompartment")
                                                                          , NULL
                                                                          , SIP_NO_CONVERTORS
                                                                          , NULL
                                                                          , &iserr
                                                                          )
                                                        );
                sipCpp -> set_parent(electrical_compartment);
            }
            else
            {
                sipErr = 1;
                PyErr_SetString( PyExc_TypeError
                               , "Expected a value of type ElectricalCompartment"
                               );
            }
        %End
    };

    SIP_PYOBJECT children();
    %MethodCode
        sipRes = PyTuple_New(sipCpp -> children.size());
        SIP_SSIZE_T index = 0;
        for (const auto &electrical_compartment_pair : sipCpp -> children)
        {
            PyTuple_SET_ITEM( sipRes
                            , index
                            , sipConvertFromType( electrical_compartment_pair.second
                                                , sipFindType("ElectricalCompartment")
                                                , NULL
                                                )
                            );
            index += 1;
        }
    %End

    ElectricalCompartment(const char * id);

    ~ElectricalCompartment();

    SIP_SSIZE_T
    size() const /__len__ /;

    void
    attach(ChemicalCompartment * chemical_compartment /Transfer/);
    %MethodCode
        if(!sipCpp -> attach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("ChemicalCompartment : " + a0 -> id + " is already attached to ElectricalCompartment : " + sipCpp -> id).c_str()
                           );
        }
    %End

    void
    detach(ChemicalCompartment * chemical_compartment);
    %MethodCode
        if(!sipCpp -> detach(a0))
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_ValueError
                           , ("ChemicalCompartment : " + a0 -> id + " is not attached to ElectricalCompartment : " + sipCpp -> id).c_str()
                           );
        }
    %End

    void
    subdivide(PyObject * ids);

    SIP_PYTUPLE
    chemical_compartments() const;
    %MethodCode
        sipRes = PyTuple_New(sipCpp -> size());
        SIP_SSIZE_T index = 0;
        for (const auto &chemical_compartment_pair : sipCpp -> chemical_compartment_map)
        {
            PyTuple_SET_ITEM( sipRes
                            , index
                            , sipConvertFromType( chemical_compartment_pair.second
                                                , sipType_ChemicalCompartment
                                                , NULL
                                                )
                            );
            index += 1;
        }
    %End

    int
    __contains__(ChemicalCompartment * chemical_compartment) const;
    %MethodCode
        return static_cast<int>(sipCpp -> get(a0 -> id) != nullptr);
    %End

    SIP_PYOBJECT
    __getitem__(const char * chemical_compartment_id) const;
    %MethodCode
        ChemicalCompartment * chemical_compartment = sipCpp -> get(std::string(a0));
        if(chemical_compartment == nullptr)
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_KeyError
                           , a0
                           );
        }
        else
        {
            sipRes = sipConvertFromType( chemical_compartment
                                       , sipType_ChemicalCompartment
                                       , NULL
                                       );
        }
    %End

    SIP_PYOBJECT
    __str__() const;
    %MethodCode
        sipRes = PyString_FromString(("ElectricalCompartment : " + sipCpp -> id).c_str());
    %End
};

class ChemicalCompartment : public Visualizable
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/ChemicalCompartment.hpp"
%End

public:

    SIP_PYOBJECT electrical_compartment
    {
        %GetCode
            sipPy = sipConvertFromType( sipCpp -> electrical_compartment
                                      , sipFindType("ElectricalCompartment")
                                      , NULL
                                      );
        %End
        %SetCode
            sipErr = 1;
            PyErr_SetString( PyExc_AttributeError
                           , "Use ElectricalCompartment.attach to change the electrical compartment this chemical compartment belongs to"
                           );
        %End
    };

    ChemicalCompartment(const char * id);

    ~ChemicalCompartment();

    SIP_PYOBJECT
    __str__() const;
    %MethodCode
        sipRes = PyString_FromString(("ChemicalCompartment : " + sipCpp -> id).c_str());
    %End
};

class Visualizables
{
%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/conversions.hpp"
#include "core/Visualizable.hpp"
#include "core/Visualizables.hpp"
%End

public:

    Visualizables(const char * id);

    const char *
    get_id();

    uint
    add_visualizable(Visualizable * visualizable);

    uint
    remove_visualizable(PyObject * visualizable);

    void
    set_color(PyObject * color_seq);

    void
    set_color_mapping( PyObject * value_range
                     , PyObject * color_range
                     );
};

class NetworkViewer : public QGLWidget
{

%TypeHeaderCode
#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Network.hpp"
#include "view/NetworkViewer.hpp"
%End

public:

    Network *
    get_network();

    NetworkViewer( Network * network
                          , QWidget * parent             = 0
                          , const QGLWidget* shareWidget = 0
                          , Qt::WindowFlags f            = 0
                          );

    void
    set_background_color(PyObject * color_seq);
    %MethodCode
        if(PySequence_Check(a0))
        {
            if(PySequence_Length(a0) == 4)
            {
                Vec4f color( static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 0)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 1)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 2)))
                           , static_cast<float>(PyFloat_AS_DOUBLE(PySequence_ITEM(a0, 3)))
                           );
                sipCpp -> set_background_color(color);
            }
            else
            {
                sipError = sipErrorFail;
                PyErr_SetString( PyExc_IndexError
                               , "Expected a sequence of size 4,(red, green, blue, alpha)"
                               );
            }
        }
        else
        {
            sipError = sipErrorFail;
            PyErr_SetString( PyExc_TypeError
                           , "Expected a sequence such as list, tuple or numpy array"
                           );
        }
    %End

    SIP_PYOBJECT
    create_color_bar( const char * id
                    , const char * title
                    , PyObject * scalar_range
                    , PyObject * colors
                    , int num_colors
                    , int num_labels
                    , float angle
                    , PyObject * size
                    , PyObject * position
                    , PyObject * label_color
                    , const char * label_font_file
                    , PyObject * label_font_resolution
                    , float label_character_size
                    );
    %MethodCode
        bool result = sipCpp -> create_color_bar( a0
                                                , a1
                                                , pysequence_to_float_pair(a2)
                                                , pysequence_to_vec4f_vector(a3)
                                                , a4
                                                , a5
                                                , a6
                                                , pysequence_to_float_pair(a7)
                                                , pysequence_to_vec3f(a8)
                                                , pysequence_to_vec4f(a9)
                                                , a10
                                                , pysequence_to_int_pair(a11)
                                                , a12
                                                );
        sipRes = PyBool_FromLong(result);
    %End

    bool
    hide_color_bar( const char * id
                  , unsigned int view_index = 0
                  );

    bool
    show_color_bar( const char * id
                  , unsigned int view_index = 0
                  );


    void
    add_view( const char * id
            , PyObject * viewport
            , PyObject * background_color
            , int camera_manipulator_id = 5 
            , float fovy = 30.0f
            , float z_near = 1.0f
            , float z_far = 10000.0f
            );
    %MethodCode
      std::vector<osgGA::StandardManipulator *> standard_manipulators = { new osgGA::FirstPersonManipulator()
                                                                        , new osgGA::FlightManipulator()
                                                                        , new osgGA::OrbitManipulator()
                                                                        , new osgGA::NodeTrackerManipulator()
                                                                        , new osgGA::TerrainManipulator()
                                                                        , new osgGA::TrackballManipulator()
                                                                        };
      sipCpp -> add_view( a0
                        , new osg::Viewport( static_cast<int>(PyLong_AsUnsignedLong(PySequence_GetItem(a1, 0)))
                                           , static_cast<int>(PyLong_AsUnsignedLong(PySequence_GetItem(a1, 1)))
                                           , static_cast<int>(PyLong_AsUnsignedLong(PySequence_GetItem(a1, 2)))
                                           , static_cast<int>(PyLong_AsUnsignedLong(PySequence_GetItem(a1, 3)))
                                           )
                        , pysequence_to_vec4f(a2)
                        , standard_manipulators[a3]
                        , a4
                        , a5
                        , a6
                        );
    %End


    void
    split_horizontally( unsigned int view_index   = 0
                      , unsigned int width_factor = 2
                      );

    void
    split_vertically( unsigned int view_index    = 0
                    , unsigned int height_factor = 2
                    );

    void
    home(unsigned int index = 0);

    void
    forward( double distance
           , unsigned int index = 0
           );
    void
    backward( double distance
            , unsigned int index = 0
            );
    void
    left( double distance
        , unsigned int index = 0
        );
    void
    right( double distance
         , unsigned int index = 0
         );

    void
    up( double distance
      , unsigned int index = 0
      );

    void
    down( double distance
        , unsigned int index = 0
        );

    void
    zoom( double factor
        , unsigned int index = 0
        );

    void
    roll( double angle
        , unsigned int index = 0
        );

    void
    pitch( double angle
         , unsigned int index = 0
         );

    void
    yaw( double angle
       , unsigned int index = 0
       );

    void
    capture( const char * viewer_id
           , const char * filename
           , const char * extension
           , unsigned long frame_count
           );


    // void
    // capture_continuous_toggle(unsigned int index);

    // void
    // capture_once(unsigned int index);

    virtual
    ~NetworkViewer();

protected:

    virtual void paintEvent( QPaintEvent* paintEvent );
    virtual void paintGL();
    virtual void resizeGL( int width, int height );

    virtual void keyPressEvent( QKeyEvent* event );
    virtual void keyReleaseEvent( QKeyEvent* event );

    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent* event );

    virtual bool event( QEvent* event );
};

