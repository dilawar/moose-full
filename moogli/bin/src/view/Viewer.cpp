#include "view/Viewer.hpp"

Viewer::Viewer( QWidget * parent
              , const QGLWidget * share_widget
              , Qt::WindowFlags f
              ): QGLWidget( parent
                          , share_widget
                          , f)
               , _viewer(new osgViewer::CompositeViewer())
               , _shapes(new osg::PositionAttitudeTransform())
{

    QGLWidget::setFormat(QGLFormat(QGL::SampleBuffers));

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
    traits -> x = this -> x();
    traits -> y = this -> y();
    traits -> width = this -> width();
    traits -> height = this -> height();
    traits -> windowDecoration = false;
    traits -> doubleBuffer = true;
    traits -> sharedContext = 0;
    traits -> samples = 4;
    _graphics_window = new osgViewer::GraphicsWindowEmbedded(traits.get());

    _shapes -> getOrCreateStateSet() -> setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    _shapes -> getOrCreateStateSet() -> setMode(GL_BLEND, osg::StateAttribute::ON);
    _shapes -> getOrCreateStateSet() -> setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    _viewer -> setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);

    // This ensures that the widget will receive keyboard events. This focus
    // policy is not set by default. The default, Qt::NoFocus, will result in
    // keyboard events that are ignored.
    this -> setFocusPolicy(Qt::StrongFocus);
    this -> setMinimumSize(200, 200);

    // Ensures that the widget receives mouse move events even though no
    // mouse button has been pressed. We require this in order to let the
    // graphics window switch viewports properly.
    this -> setMouseTracking(true);
}

void Viewer::paintEvent(QPaintEvent * paintEvent)
{
    // this->makeCurrent();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paintGL();
    painter.end();
    swapBuffers();
    doneCurrent();
}

void
Viewer::paintGL()
{
    _viewer -> frame();
}

void Viewer::resizeGL(int width, int height)
{
  _graphics_window -> resized(0, 0, width, height);
    osgViewer::ViewerBase::Views views;
    _viewer->getViews(views);
    //for(osgViewer::View * view : views) { view -> resize(); }
}

osgViewer::View *
Viewer::get_view_with_focus()
{
   return _viewer -> getViewWithFocus();
}

void Viewer::keyPressEvent(QKeyEvent* event)
{
    QString keyString = event->text();
    const char* keyData = keyString.toLocal8Bit().data();
    // unsigned int view_index = _get_view_index_with_focus();
    // std::cout << "Index => " << view_index << std::endl;
    switch (event->key()) {
    case Qt::Key_Q:
        if (event->modifiers() & Qt::ControlModifier) {
            QApplication::quit();
        }
        // case Qt::Key_P          :   if(event->modifiers() & Qt::ControlModifier)
        //                             {
        //                                 QApplication::quit();
        //                             }
        // case Qt::Key_S          :   if(event->modifiers() & Qt::ShiftModifier)
        //                             {
        //                                 toggle_horizontal_split();
        //                             }
        // case Qt::Key_Z          :   if(event->modifiers() & Qt::ShiftModifier)
        //                             {
        //                                 toggle_vertical_split();
        //                             }
        // case Qt::Key_S          :   if(event->modifiers() & Qt::ShiftModifier)
        //                             {
        //                                 capture_continuous_toggle(view_index);
        //                             }
        //                             else
        //                             {
        //                                 capture_once(view_index);
        //                             }
        break;
    // case Qt::Key_Space:
    //     home();
    //     break;
    // case Qt::Key_Up:
    //     up(up_distance);
    //     break;
    // case Qt::Key_Down:
    //     down(down_distance);
    //     break;
    // case Qt::Key_Left:
    //     left(left_distance);
    //     break;
    // case Qt::Key_Right:
    //     right(right_distance);
    //     break;
    // case Qt::Key_F:
    //     forward(forward_distance);
    //     break;
    // case Qt::Key_B:
    //     backward(backward_distance);
    //     break;
    // case Qt::Key_Plus:
    //     zoom(zoom_factor);
    //     break;
    // case Qt::Key_Equal:
    //     zoom(zoom_factor);
    //     break;
    // case Qt::Key_Period:
    //     zoom(zoom_factor);
    //     break;
    // case Qt::Key_Greater:
    //     zoom(zoom_factor);
    //     break;
    // case Qt::Key_Minus:
    //     zoom(-zoom_factor);
    //     break;
    // case Qt::Key_Underscore:
    //     zoom(-zoom_factor);
    //     break;
    // case Qt::Key_Comma:
    //     zoom(-zoom_factor);
    //     break;
    // case Qt::Key_Less:
    //     zoom(-zoom_factor);
    //     break;
    // case Qt::Key_R:
    //     if (event->modifiers() & Qt::ShiftModifier) {
    //         roll(-roll_angle);
    //     }
    //     else {
    //         roll(roll_angle);
    //     }
    //     break;
    case Qt::Key_P:
        if (event->modifiers() & Qt::ControlModifier) {
            int x = 0, y = 0;
            unsigned int width = 1000, height = 500;
            // viewer.getCamera() -> getProjectionRectangle(x,y,width,height);
            osg::ref_ptr<osg::Image> image = new osg::Image;
            image->readPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE);
            osgDB::writeImageFile(*image, "./image.jpg");
        }
    //     else if (event->modifiers() & Qt::ShiftModifier) {
    //         pitch(-pitch_angle);
    //     }
    //     else {
    //         pitch(pitch_angle);
    //     }
    //     break;
    // case Qt::Key_Y:
    //     if (event->modifiers() & Qt::ShiftModifier) {
    //         yaw(-yaw_angle);
    //     }
    //     else {
    //         yaw(yaw_angle);
    //     }
        break;
    default:
        break;
    }
}

// void
// Viewer::home()
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> home();
//     }
// }

// void
// Viewer::forward(double distance_delta)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> forward(distance_delta);
//     }
// }

// void
// Viewer::backward(double distance_delta)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> backward(distance_delta);
//     }
// }

// void
// Viewer::left(double distance_delta)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> left(distance_delta);
//     }
// }

// void
// Viewer::right(double distance_delta)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> right(distance_delta);
//     }
// }

// void
// Viewer::up(double distance_delta)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> up(distance_delta);
//     }
// }

// void
// Viewer::down(double distance_delta)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> down(distance_delta);
//     }
// }

// void
// Viewer::zoom(double factor)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> zoom(factor);
//     }
// }

// void
// Viewer::roll(double angle)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> roll(angle);
//     }
// }

// void
// Viewer::pitch(double angle)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> pitch(angle);
//     }
// }

// void
// Viewer::yaw(double angle)
// {
//     osgViewer::ViewerBase::Views views;
//     _viewer->getViews(views);
//     for(osgViewer::View * view : views)
//     {
//         view -> yaw(angle);
//     }
// }

void Viewer::keyReleaseEvent(QKeyEvent* event)
{
    QString keyString = event -> text();
    const char* keyData = keyString.toLocal8Bit().data();
    this->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KeySymbol(*keyData));
}

void Viewer::mouseMoveEvent(QMouseEvent* event)
{
    this->getEventQueue()->mouseMotion(static_cast<float>(event->x()),
        static_cast<float>(event->y()));
}

void Viewer::mousePressEvent(QMouseEvent* event)
{
    // 1 = left mouse button
    // 2 = middle mouse button
    // 3 = right mouse button

    unsigned int button = 0;

    switch (event->button()) {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    default:
        break;
    }

    this->getEventQueue()->mouseButtonPress(
        static_cast<float>(event->x()), static_cast<float>(event->y()), button);
}

void Viewer::mouseReleaseEvent(QMouseEvent* event)
{

    // 1 = left mouse button
    // 2 = middle mouse button
    // 3 = right mouse button

    unsigned int button = 0;

    switch (event->button()) {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    default:
        break;
    }

    this->getEventQueue()->mouseButtonRelease(
        static_cast<float>(event->x()), static_cast<float>(event->y()), button);
}

void Viewer::wheelEvent(QWheelEvent* event)
{

    event->accept();
    int delta = event->delta();

    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ? osgGA::GUIEventAdapter::SCROLL_UP
                                                               : osgGA::GUIEventAdapter::SCROLL_DOWN;

    this->getEventQueue()->mouseScroll(motion);
}

bool Viewer::event(QEvent* event)
{
    bool handled = QGLWidget::event(event);

    // This ensures that the OSG widget is always going to be repainted after the
    // user performed some interaction. Doing this in the event handler ensures
    // that we don't forget about some event and prevents duplicate code.
    switch (event->type()) {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
    case QEvent::Wheel:
        this->update();
        break;
    default:
        break;
    }
    if (event->type() == QEvent::MouseButtonPress
        // && QApplication::keyboardModifiers() & Qt::ControlModifier
        )
    {
        osgViewer::View* view = _viewer->getView(0);
        osgUtil::LineSegmentIntersector::Intersections intersections;
        bool result = view->computeIntersections(
             view->getCamera(), osgUtil::Intersector::CoordinateFrame::PROJECTION,
             this->getEventQueue()->getCurrentEventState()->getXnormalized(),
             this->getEventQueue()->getCurrentEventState()->getYnormalized(),
             intersections);
         if (result) {
             const osgUtil::LineSegmentIntersector::Intersection& hit = *intersections.begin();
             this->getEventQueue()->mouseButtonRelease(
                 this->getEventQueue()->getCurrentEventState()->getX(),
                 this->getEventQueue()->getCurrentEventState()->getY(), 1);
             emit selected(hit.drawable->asGeometry()->getName().c_str());
          }
    }
    return (handled);
}


osgGA::EventQueue* Viewer::getEventQueue() const
{
    osgGA::EventQueue* event_queue = _graphics_window->getEventQueue();

    if (event_queue)
        return (event_queue);
    else
        throw(std::runtime_error("Unable to obtain valid event queue"));
}

void
Viewer::attach_view(MeshView * mesh_view)
{
    mesh_view -> set_graphics_window(_graphics_window.get());
    mesh_view -> get_view() -> setSceneData(_shapes.get());
    _viewer -> addView(mesh_view -> get_view());
    mesh_view -> home();
}

void
Viewer::detach_view(MeshView * mesh_view)
{
  _viewer -> removeView(mesh_view -> get_view());
}

void
Viewer::attach_shape(MoogliShape * shape)
{
    _shapes -> addChild(shape -> root.get());
}

void
Viewer::detach_shape(MoogliShape * shape)
{
    _shapes -> removeChild(shape -> root.get());
}
