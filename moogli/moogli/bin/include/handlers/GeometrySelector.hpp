#ifndef __GEOMETRY_SELECTOR_HPP__
#define __GEOMETRY_SELECTOR_HPP__

#include "utility/libraries.hpp"
#include "utility/record.hpp"

class GeometrySelector : public osgGA::GUIEventHandler
{
public:
    string id;
    bool   selected;

    GeometrySelector();

    virtual bool
    handle( const osgGA::GUIEventAdapter& ea
          , osgGA::GUIActionAdapter& aa
          );

private:

    Geometry *
    _get_intersection( const osgGA::GUIEventAdapter& ea
                     , osgViewer::View* viewer
                     );
};

#endif /* __GEOMETRY_SELECTOR_HPP__ */
