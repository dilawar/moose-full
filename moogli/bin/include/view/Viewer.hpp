#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__

#include "utility/libraries.hpp"
#include "utility/constants.hpp"
//#include "core/Network.hpp"
//#include "handlers/GeometrySelector.hpp"
//#include "callbacks/CaptureView.hpp"
//#include "core/CoordinateSystem.hpp"
#include "view/View.hpp"
#include "shapes/Shape.hpp"
//#include "callbacks/MeshUpdateCallback.hpp"

class Viewer : public QGLWidget
{
  Q_OBJECT

public:
    Viewer( QWidget * parent = nullptr
          , const QGLWidget * share_widget = nullptr
          , Qt::WindowFlags f = 0
          );

    osgGA::EventQueue* getEventQueue() const;

    void
    attach_view(MeshView * mesh_view);

    void
    detach_view(MeshView * mesh_view);

    void
    attach_shape(Shape * shape);

    void
    detach_shape(Shape * shape);

    osgViewer::View *
    get_view_with_focus();

signals:
    void
    selected(const char * id);


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

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> _shapes;
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _graphics_window;
    osg::ref_ptr<osgViewer::CompositeViewer>        _viewer;
};

#endif /* __VIEWER_HPP__ */
