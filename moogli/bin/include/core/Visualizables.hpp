#ifndef __VISUALIZABLES_HPP__
#define __VISUALIZABLES_HPP__

#include "utility/libraries.hpp"
#include "utility/conversions.hpp"
#include "core/Visualizable.hpp"

class Visualizables
{
public:
    string id;

    Visualizables(const char * id);

    Visualizables(const string id);

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

    // void
    // set_radius(PyObject * color_seq);

    // void
    // set_height(PyObject * color_seq);

    ~Visualizables();
    /* data */

private:
    float _color_mapping_min_value;
    float _color_mapping_max_value;
    Vec4f _color_mapping_min_color;
    Vec4f _color_mapping_max_color;

    Vec4f
    _value_to_color(float value);

    std::vector<Visualizable *> _visualizable_seq;

};

#endif  /* __VISUALIZABLES_HPP__ */
