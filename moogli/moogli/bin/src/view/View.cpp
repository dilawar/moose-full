#include "view/View.hpp"

MeshView::MeshView() : _view(new osgViewer::View())
                     , _manipulator(new osgGA::TrackballManipulator())
{
    create_cameras();
    _view -> setCameraManipulator(_manipulator.get());
}

void
MeshView::set_position(const osg::Vec2f & position)
{
  _position = position;
}

const osg::Vec2f &
MeshView::get_position() const
{
  return _position;
}

void
MeshView::set_size(const osg::Vec2f & size)
{
    _size = size;
}

const osg::Vec2f &
MeshView::get_size() const
{
    return _size;
}


osg::Viewport *
MeshView::get_viewport()
{
    int x, y, width, height;
    _graphics_window -> getWindowRectangle(x, y, width, height);
    return new osg::Viewport( static_cast<int>(x * _position.x())
                            , static_cast<int>(y * _position.y())
                            , static_cast<int>(width * _size.x())
                            , static_cast<int>(height * _size.y())
                            );
}


void
MeshView::resize(int width, int height)
{
    osg::Viewport * viewport = new osg::Viewport( static_cast<int>(width * _position.x())
                                                , static_cast<int>(height * _position.y())
                                                , static_cast<int>(width * _size.x())
                                                , static_cast<int>(height * _size.y())
                                                );

    _main_camera -> setViewport(viewport);
    _main_camera -> setProjectionMatrixAsPerspective( 30.0f
                                                      , viewport -> aspectRatio()
                                                      , 1.0f
                                                      , 1000.0f
                                                      );
    _heads_up_display_camera -> setProjectionMatrix(osg::Matrix::ortho2D( 0
                                                                        , viewport -> width()
                                                                        , 0
                                                                        , viewport -> height()
                                                                        )
                                                   );
    for(unsigned int i = 0; i < _view -> getNumSlaves(); ++i)
    {
        _view -> getSlave(i)._camera -> setViewport(viewport);
    }
}


void
MeshView::set_graphics_window(osgViewer::GraphicsWindowEmbedded * graphics_window)
{
    _graphics_window = graphics_window;
    _view -> getCamera() -> setGraphicsContext(_graphics_window.get());
    for(unsigned int i = 0; i < _view -> getNumSlaves(); ++i)
    {
        _view -> getSlave(i)._camera -> setGraphicsContext(_graphics_window.get());
    }
    int x, y, width, height;
    _graphics_window -> getWindowRectangle(x, y, width, height);
    resize(width, height);
}

osgViewer::GraphicsWindowEmbedded *
MeshView::get_graphics_window()
{
    return _graphics_window.get();
}

void
MeshView::create_cameras()
{
    _main_camera = create_perspective_projection_camera();
    _heads_up_display_camera = create_orthographic_projection_camera();
    _view -> setCamera(_main_camera.get());
    _view -> addSlave(_heads_up_display_camera.get(), false);
}

osg::Camera *
MeshView::create_perspective_projection_camera()
{
    osg::Camera * camera = new osg::Camera();
    //camera -> setViewport(viewport);
    camera -> setProjectionMatrixAsPerspective( 30.0f
                                              , 1.0f
                                              , 1.0f
                                              , 10000.0f
                                              );
    camera -> setClearColor(_background_color);
    camera -> setClearMask(camera -> getClearMask()|GL_STENCIL_BUFFER_BIT);
    camera -> setClearStencil(0);

    //camera -> setGraphicsContext(_graphics_window);
    camera -> getOrCreateStateSet() -> setGlobalDefaults();
    //camera -> setPostDrawCallback(new CaptureView());
    return camera;
}


osg::Camera *
MeshView::create_orthographic_projection_camera()
{
    osg::Camera * camera = new osg::Camera;
    // set the projection matrix
    camera -> setProjectionMatrix(osg::Matrix::ortho2D( 0
                                                      , 100
                                                      , 0
                                                      , 100
                                                      )
                                 );
    // set the view matrix
    camera -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera -> setViewMatrix(osg::Matrix::identity());

    // only clear the depth buffer
    camera -> setClearMask(GL_DEPTH_BUFFER_BIT);

    // draw subgraph after main camera view.
    camera -> setRenderOrder(osg::Camera::POST_RENDER);

    // we don't want the camera to grab event focus from the viewers main camera(s).
    camera -> setAllowEventFocus(false);
    //camera -> setGraphicsContext(_graphics_window);
    return camera;
}



void
MeshView::set_background_color(const osg::Vec4f & background_color)
{
    _background_color = background_color;
    _view -> getCamera() -> setClearColor(_background_color);
    for(unsigned int i = 0; i < _view -> getNumSlaves(); ++i)
    {
        _view -> getSlave(i)._camera -> setClearColor(_background_color);
    }
}

const osg::Vec4f &
MeshView::get_background_color()
{
    return _background_color;
}

osgViewer::View *
MeshView::get_view()
{
    return _view.get();
}

void
MeshView::set_view(osgViewer::View * view)
{
    _view = view;
}

void
MeshView::attach_color_bar(ColorBar * color_bar)
{
    _heads_up_display_camera -> addChild(color_bar -> root.get());
    osg::Viewport * viewport = _main_camera -> getViewport();
    color_bar -> resize( 0//viewport -> x()
                         , 0//viewport -> y()
                         , 100//viewport -> width()
                         , 100//viewport -> height()
                       );
}

void
MeshView::detach_color_bar(ColorBar * color_bar)
{
    _heads_up_display_camera -> removeChild(color_bar -> root.get());
}

void
MeshView::home()
{
    _manipulator -> home(0.0);
}

void
MeshView::forward(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye + look * distance_delta, center + look * distance_delta, up);
}

void
MeshView::backward(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye - look * distance_delta, center - look * distance_delta, up);
}

void
MeshView::left(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye + side * distance_delta, center + side * distance_delta, up);
}

void
MeshView::right(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye - side * distance_delta, center - side * distance_delta, up);
}

void
MeshView::up(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye - up * distance_delta, center - up * distance_delta, up);
}

void
MeshView::down(double distance_delta)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye + up * distance_delta, center + up * distance_delta, up);
}

void
MeshView::zoom(double factor)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    _set_transformation(eye + look * factor * distance, center, up);
}

void
MeshView::_get_transformation( osg::Vec3d & eye
                             , osg::Vec3d & center
                             , double & distance
                             , osg::Vec3d & up
                             , osg::Vec3d & look
                             , osg::Vec3d & side
                             )
{
    _manipulator -> getTransformation(eye, center, up);
    up.normalize();
    look = center - eye;
    distance = look.normalize();
    side = look ^ up;
    side.normalize();
}

void
MeshView::_set_transformation( const osg::Vec3d & eye
                               , const osg::Vec3d & center
                               , const osg::Vec3d & up
                               )
{
    _manipulator->setTransformation(eye, center, up);
}

void
MeshView::roll(double angle)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    osg::Quat rotation(-angle, look);
    osg::Vec3f rotated_up = rotation * up;
    rotated_up.normalize();
    _set_transformation(eye, center, rotated_up);
}

void
MeshView::pitch(double angle)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    osg::Quat rotation(-angle, side);//osg::X_AXIS);//side);
    osg::Vec3f rotated_up = rotation * up;
    rotated_up.normalize();
    osg::Vec3f rotated_look = rotation * look;
    rotated_look.normalize();
    _set_transformation( center - rotated_look * distance
                       , center
                       , rotated_up
                       );
}

void
MeshView::yaw(double angle)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(eye, center, distance, up, look, side);
    osg::Quat rotation = osg::Quat(-angle, up);
    osg::Vec3f rotated_look = rotation * look;
    rotated_look.normalize();
    _set_transformation(center - rotated_look * distance, center, up);
}

    // void
    // attach_text_twod();

    // void
    // detach_text_twod();

    // void
    // attach_image();

    // void
    // detach_image();

    // void
    // attach_text_threed();

    // void
    // detach_text_threed();

    // void
    // attach_coordinate_axes();

    // void
    // detach_coordinate_axes();

    // void
    // attach_scale_mesh();

    // void
    // detach_scale_mesh();

    /* data */
