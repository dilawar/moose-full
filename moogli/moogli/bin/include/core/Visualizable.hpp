#ifndef __VISUALIZABLE_HPP__
#define __VISUALIZABLE_HPP__

#include "utility/libraries.hpp"
#include "utility/numerics.hpp"
#include "utility/constants.hpp"


class Visualizables;

class Visualizable
{
public:
    string id;
    std::vector<Visualizables *> visualizables_seq;
    osg::ref_ptr<osg::Switch> node;
    osg::Vec4f proximal;
    osg::Vec4f distal;

    Visualizable(const char * id);

    bool
    write(const std::string & filename) const;

    void
    set_color(const osg::Vec4f & color, int index = -1);

    void
    hide(int index);

    void
    show(int index);

    bool
    visible(int index);

    void
    add_representation(bool as_sphere = false);

    size_t
    representation_count() const;

};

#endif  /* __VISUALIZABLE_HPP__ */
