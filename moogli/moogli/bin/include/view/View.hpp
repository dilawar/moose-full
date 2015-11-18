#ifndef __VIEW_HPP__
#define __VIEW_HPP__

#include "utility/libraries.hpp"
#include "widgets/ColorBar.hpp"

class MeshView
{
public:
    MeshView();

    void
    set_position(const osg::Vec2f & position);

    const osg::Vec2f &
    get_position() const;

    void
    set_size(const osg::Vec2f & size);

    const osg::Vec2f &
    get_size() const;

    osg::Viewport *
    get_viewport();

    void
    resize(int width, int height);

    void
    set_graphics_window(osgViewer::GraphicsWindowEmbedded * graphics_window);

    osgViewer::GraphicsWindowEmbedded *
    get_graphics_window();

    void
    create_cameras();

    osg::Camera *
    create_perspective_projection_camera();

    osg::Camera *
    create_orthographic_projection_camera();

    void
    set_background_color(const osg::Vec4f & background_color);

    const osg::Vec4f &
    get_background_color();

    osgViewer::View *
    get_view();

    void
    set_view(osgViewer::View * view);

    void
    attach_color_bar(ColorBar * color_bar);

    void
    detach_color_bar(ColorBar * color_bar);

    void
    home();

    void
    forward(double distance_delta);

    void
    backward(double distance_delta);

    void
    left(double distance_delta);

    void
    right(double distance_delta);

    void
    up(double distance_delta);

    void
    down(double distance_delta);

    void
    zoom(double factor);

    void
    roll(double angle);

    void
    pitch(double angle);

    void
    yaw(double angle);

private:

    void
    _get_transformation( osg::Vec3d & eye
                       , osg::Vec3d & center
                       , double & distance
                       , osg::Vec3d & up
                       , osg::Vec3d & look
                       , osg::Vec3d & side
                       );

    void
    _set_transformation( const osg::Vec3d & eye
                       , const osg::Vec3d & center
                       , const osg::Vec3d & up
                       );

    osg::ref_ptr<osgGA::TrackballManipulator> _manipulator;
    osg::ref_ptr<osg::Camera> _main_camera;
    osg::ref_ptr<osg::Camera> _heads_up_display_camera;
    osg::ref_ptr<osgViewer::View> _view;
    osg::Vec2f _size;
    osg::Vec2f _position;
    osg::Vec4f _background_color;
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _graphics_window;
};

#endif /* __VIEW_HPP__ */
