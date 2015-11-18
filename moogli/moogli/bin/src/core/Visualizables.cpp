#include "core/Visualizables.hpp"

Visualizables::Visualizables(const char * id) : Visualizables::Visualizables(string(id))
{}

Visualizables::Visualizables(const string id) : id(id)
{ }

Visualizables::~Visualizables()
{

}

const char *
Visualizables::get_id()
{
    return id.c_str();
}

uint
Visualizables::add_visualizable(Visualizable * visualizable)
{
    // int iserr = 0;
    // const sipAPIDef * sip = get_sip_api();
    Visualizable * v = visualizable;
    // cerr << "Here";
    // cerr << sip -> api_get_address(visualizable);
    // cerr << "Here2";
    // cerr << sip -> api_can_convert_to_type( visualizable
    //                                       , sip -> api_find_type("Visualizable")
    //                                       , SIP_NOT_NONE
    //                                       );
    // cerr << "Here3";
    // v = static_cast<Visualizable *> (sip -> api_convert_to_type( visualizable
    //                                                            , sip -> api_find_type("Visualizable")
    //                                                            , NULL
    //                                                            , SIP_NOT_NONE
    //                                                            , NULL
    //                                                            , &iserr
    //                                                            )
    //                                 );
    // cerr << "Here4";
    // assert(v != nullptr);
    // if(iserr !=0 )
    // {
    // v = static_cast<Visualizable *>(sip_api -> api_convert_to_type( visualizable
    //                               , sip_api -> api_find_type("ElectricalCompartment")
    //                               , NULL
    //                               , SIP_NOT_NONE
    //                               , NULL
    //                               , &iserr
    //                               ));
    // }
    // if(iserr !=0 )
    // {
    // v = static_cast<Visualizable *>(sip_api -> api_convert_to_type( visualizable
    //                               , sip_api -> api_find_type("ChemicalCompartment")
    //                               , NULL
    //                               , SIP_NOT_NONE
    //                               , NULL
    //                               , &iserr
    //                               ));
    // }

    // Visualizable * v = static_cast<Visualizable *>(sip_api -> api_get_address(dynamic_cast<sipSimpleWrapper *>(visualizable)));
    _visualizable_seq.push_back(v);
return static_cast<uint>(_visualizable_seq.size());
}

uint
Visualizables::remove_visualizable(PyObject * visualizable)
{
    Visualizable * v = static_cast<Visualizable *>(PyCObject_AsVoidPtr(visualizable));
    // int iserr = 0;
    // const sipAPIDef * sip_api = get_sip_api();
    // Visualizable * v;
    // v = static_cast<Visualizable *>(sip_api -> api_convert_to_type( visualizable
    //                               , sip_api -> api_find_type("Neuron")
    //                               , NULL
    //                               , SIP_NOT_NONE
    //                               , NULL
    //                               , &iserr
    //                               ));
    // if(iserr !=0 )
    // {
    // v = static_cast<Visualizable *>(sip_api -> api_convert_to_type( visualizable
    //                               , sip_api -> api_find_type("ElectricalCompartment")
    //                               , NULL
    //                               , SIP_NOT_NONE
    //                               , NULL
    //                               , &iserr
    //                               ));
    // }
    // if(iserr !=0 )
    // {
    // v = static_cast<Visualizable *>(sip_api -> api_convert_to_type( visualizable
    //                               , sip_api -> api_find_type("ChemicalCompartment")
    //                               , NULL
    //                               , SIP_NOT_NONE
    //                               , NULL
    //                               , &iserr
    //                               ));
    // }

    auto iter = find( _visualizable_seq.begin()
                    , _visualizable_seq.end()
                    , v
                    );
    if(iter == _visualizable_seq.end())
    {
        PyErr_SetString( PyExc_ValueError
                       , ( string("visualizable '")
                         + string(v -> id)
                         + string("' is not a part of group '")
                         + id
                         + string("'.")
                         ).c_str()
                       );
        return static_cast<uint>(_visualizable_seq.size());
    }
    _visualizable_seq.erase(iter);
    return static_cast<uint>(_visualizable_seq.size());
}

// void
// Group::set_radius(PyObject * radius_seq)
// {
//     if( !PyVisualizables_Check(radius_seq)
//       || PyUnicode_Check(radius_seq)
//       || PyString_Check(radius_seq)
//       || PyByteArray_Check(radius_seq)
//       )
//     {
//         PyErr_SetString( PyExc_TypeError
//                        , "set_radius expects a list, tuple, array, or its derivative as its argument."
//                        );
//         return;
//     }
//     uint size = min( _visualizable_seq.size()
//                    , static_cast<size_t>(PyVisualizables_Size(radius_seq))
//                    );
//     for(uint i = 0; i < size; ++i)
//     {
//         _visualizable_seq[i] -> set_radius(
//             ( PyVisualizables_GetItem( radius_seq
//                                 , static_cast<Py_ssize_t>(i)
//                                 )
//             )
//                                           );
//     }
// }


// void
// Group::set_height(PyObject * color_seq)
// {

// }


// void
// Group::_set_height_from_seq(PyObject * color_seq)
// {

// }

void
Visualizables::set_color(PyObject * color_seq)
{
    if( !PySequence_Check(color_seq)
      || PyUnicode_Check(color_seq)
      || PyString_Check(color_seq)
      || PyByteArray_Check(color_seq)
      )
    {
        PyErr_SetString( PyExc_TypeError
                       , "set_color expects a list, tuple, array, or its derivative as its argument."
                       );
        return;
    }
    uint size = min( _visualizable_seq.size()
                   , static_cast<size_t>(PySequence_Size(color_seq))
                   );
    for(uint i = 0; i < size; ++i)
    {
        _visualizable_seq[i] -> set_color(
            _value_to_color( PyFloat_AS_DOUBLE( PySequence_GetItem( color_seq
                                                                  , static_cast<Py_ssize_t>(i)
                                                                  )
                                              )
                           )
                                         );
    }
}

Vec4f
Visualizables::_value_to_color(float value)
{
    float normalized_value = (value - _color_mapping_min_value)
                           / (_color_mapping_max_value - _color_mapping_min_value);
    normalized_value = constrain<float>(0.0f, normalized_value, 1.0f);
    return ( _color_mapping_min_color
           + ( _color_mapping_max_color
             - _color_mapping_min_color
             ) * normalized_value
           );
}


// void
// Group::_set_color_from_map(PyObject * color_seq)
// {

// }

// void
// Group::_set_color_from_seq(PyObject * color_seq)
// {

// }

void
Visualizables::set_color_mapping(PyObject * value_range, PyObject * color_range)
{
    _color_mapping_min_value = PyFloat_AS_DOUBLE(PySequence_GetItem(value_range, 0));
    _color_mapping_max_value = PyFloat_AS_DOUBLE(PySequence_GetItem(value_range, 1));
    _color_mapping_min_color = pysequence_to_vec4f(PySequence_GetItem(color_range, 0));
    _color_mapping_max_color = pysequence_to_vec4f(PySequence_GetItem(color_range, 1));
}

    // osg::Geometry * geometry = new osg::Geometry();
    // geometry -> setName(id);
    // osg::Geode    * geode   = new osg::Geode();
    // geode -> setName(id);

// uint
// Visualizable::create_representation(PyObject * representation)
// {
//     uint type       = PyObject_GetAttrString(representation, "type");
//     uint position   = PyObject_GetAttrString(representation, "position");
//     bool continuous = PyObject_GetAttrString(representation, "continuous");
//     bool radius     = PyObject_GetAttrString(representation, "radius");
//     uint points     = PyObject_GetAttrString(representation, "points");

// }


// Geometry *
// Visualizable::create_representation( uint representation
//                                    , uint position            = 0
//                                    , bool continuous          = false
//                                    , bool fixed_radius        = true
//                                    )
// {
//     Geometry * geometry;
//     switch(representation)
//     {
//         case CENTER_SPHERE      :   geometry = create_sphere((proximal + distal) / 2.0f);
//                                     break;
//         case PROXIMAL_SPHERE    :   geomtery = create_sphere(proximal);
//                                     break;
//         case DISTAL_SPHERE      :   geometry = create_sphere(distal);
//                                     break;
//         case CYLINDER           :   geometry = create_cylinder();
//                                     break;
//         case TETRAHEDRON        :   geometry = create_tetrahedron();
//                                     break;
//         case OCTAHEDRON         :
//         case ICOSAHEDRON        :
//         case DODECAHEDRON       :
//         case CUBE               :
//         case CONE               :   set_cone()
//         case TRIANGULAR_PRISM   :   create_prism();
//                                     break;
//         case RECTANGULAR_PRISM  :   create_prism();
//                                     break;
//         case PENTAGONAL_PRISM   :
//         case HEXAGONAL_PRISM    :

//     }
//     if(representation == CYLINDER)
//     {

//     }
// }


// void
// Group::set_height_mapping(PyObject * height_mapping)
// {
//     _height_min
//     _height_max
//     _height
//     (min, max)
// }

// void
// Group::set_radius_mapping()
// {

// }

// void
// Group::get_color_mapping()
// {
// }

// void
// Group::get_height_mapping()
// {
// }

// void
// Group::get_radius_mapping()
// {

// }

