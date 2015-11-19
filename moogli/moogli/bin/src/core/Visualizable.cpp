#include "core/Visualizable.hpp"

Visualizable::Visualizable(const char * id) : id(id)
                                            , node(new osg::Switch())
{ }

void
Visualizable::hide(int index)
{
    // node -> setNodeMask(NODE_SHOW_MASK);
    // node -> setNodeMask(NODE_HIDE_MASK);

    if(index == -1)
    {
        node -> setAllChildrenOff();
    }
    else
    {
        node -> setValue(index, false);
    }
}

void
Visualizable::show(int index)
{
    if(index == -1)
    {
        node -> setAllChildrenOn();
    }
    else
    {
        node -> setValue(index, true);
    }
}

bool
Visualizable::visible(int index)
{
    if(index == -1)
    {
        for(uint i = 0; i < node -> getNumChildren(); ++i)
        {
            if(node -> getValue(i) == false) { return false; }
        }
        return true;
    }
    return node -> getValue(index);
    // return (node -> getNodeMask() == NODE_SHOW_MASK ? true : false);
}

size_t
Visualizable::representation_count() const
{
    return node -> getNumChildren();
}

void
Visualizable::add_representation(bool as_sphere)
{
    osg::Geometry * geometry = new osg::Geometry();
    geometry -> setName(id);
    osg::Geode    * geode   = new osg::Geode();
    geode -> setName(id);

    if(proximal == distal)
    {
        sphere( osg::Vec3f(distal[0], distal[1], distal[2])
              , distal[3]
              , geometry
              , SPHERICAL_COMPARTMENT_POINTS
              , SPHERICAL_COMPARTMENT_COLOR
              );
    }
    else
    {
        cylinder( distal
                , proximal
                , geometry
                , CYLINDRICAL_COMPARTMENT_POINTS
                , CYLINDRICAL_COMPARTMENT_COLOR
                );
    }
    geode -> addDrawable(geometry);
    node -> addChild(geode);
}

void
Visualizable::set_color(const Vec4f & color, int index)
{
    if(index == -1)
    {
        for(unsigned int i = 1; i < node -> getNumChildren(); ++i)
        {
            Geode * geode = node -> getChild(i) -> asGeode();
            for(unsigned int j = 0; j < geode -> getNumDrawables(); ++j)
            {
                Geometry * geometry = geode -> getDrawable(j) -> asGeometry();
                Vec4Array * colors = new Vec4Array();
                colors -> push_back(color);
                geometry -> setColorArray(colors, osg::Array::BIND_OVERALL);
                geometry -> setColorBinding(osg::Geometry::BIND_OVERALL);
            }
        }
    }
    else
    {
        Geode * geode = node -> getChild(index) -> asGeode();
        for(unsigned int j = 0; j < geode -> getNumDrawables(); ++j)
        {
            Geometry * geometry = geode -> getDrawable(j) -> asGeometry();
            Vec4Array * colors = new Vec4Array();
            colors -> push_back(color);
            geometry -> setColorArray(colors, osg::Array::BIND_OVERALL);
            geometry -> setColorBinding(osg::Geometry::BIND_OVERALL);
        }
    }
}

bool
Visualizable::write(const std::string & filename) const
{
    return osgDB::writeNodeFile( *node
                               , filename
                               );
}

