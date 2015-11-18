#include "handlers/GeometrySelector.hpp"



GeometrySelector::GeometrySelector( ) : id("")
                                      , selected(false)
{
}

bool
GeometrySelector::handle( const osgGA::GUIEventAdapter& ea
                        , osgGA::GUIActionAdapter& aa
                        )
{
    // RECORD_INFO("reaching here");
    osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);

    if ( !viewer )
    {
        return false;

    }

    bool drag_event_occurred = ea.getEventType() & osgGA::GUIEventAdapter::DRAG;
    bool push_event_occurred = ea.getEventType() & osgGA::GUIEventAdapter::PUSH;
    bool release_event_occurred = ea.getEventType() & osgGA::GUIEventAdapter::RELEASE;
    bool left_mouse_button_pressed = ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON;
    bool ctrl_key_pressed = ea.getModKeyMask() &  osgGA::GUIEventAdapter::MODKEY_CTRL;

    if(left_mouse_button_pressed && push_event_occurred)
    {
        Geometry * geometry = _get_intersection(ea,viewer);
        bool blank_click    = geometry == nullptr;
        if(blank_click)
        {
            id = "";
            selected = false;
            RECORD_INFO("not selected");
        }
        else
        {
            id = geometry -> getName();
            selected = true;
            RECORD_INFO(std::string("Selected") + geometry -> getName() );
        }
    }
    else
    {
        selected = false;
    }
    return false;
}

Geometry *
GeometrySelector::_get_intersection( const osgGA::GUIEventAdapter& ea
                                   , osgViewer::View* viewer
                                   )
{
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (viewer -> computeIntersections(ea,intersections))
    {
        const osgUtil::LineSegmentIntersector::Intersection& hit =
            *intersections.begin();
        return hit.drawable -> asGeometry();
    }
    return nullptr;
}
