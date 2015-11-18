#include <QTimer>
#include <QApplication>
#include <QGridLayout>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/MultiTouchTrackballManipulator>

#include <osgDB/ReadFile>

#include <osgQt/GraphicsWindowQt>
#include <osgFX/Outline>
#include <osgFX/Scribe>
#include <osgFX/Cartoon>
#include <osgFX/SpecularHighlights>
#include <iostream>

// class MyGLWidget : GLWidget
// {
// public:

// }

class ViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
  QWidget * widget1, * widget2, * widget3, * widget4, *popupWidget;
public:
    ViewerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0, osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded) : QWidget(parent, f)
    {
        setThreadingModel(threadingModel);

        // disable the default setting of viewer.done() by pressing Escape.
        setKeyEventSetsDone(0);
        osg::Group * node = new osg::Group();
        osg::Node * model = osgDB::readNodeFile("VHC-neuron.osgt");
        osgFX::Outline * outline = new osgFX::Outline();
        osgFX::Scribe  * scribe  = new osgFX::Scribe();
        osgFX::Cartoon * cartoon = new osgFX::Cartoon();
        osgFX::SpecularHighlights * specular_highlights = new osgFX::SpecularHighlights();
        scribe -> setWireframeColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
        scribe -> setWireframeLineWidth(1.0f);
        outline -> setColor(osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
        outline -> setWidth(8);
        scribe -> addChild(model);
        outline -> addChild(scribe);
        cartoon -> addChild(outline);
        specular_highlights -> addChild(cartoon);
        node -> addChild(specular_highlights);
        specular_highlights ->setEnabled(false);
        cartoon -> setEnabled(true);
        outline -> setEnabled(true);
        scribe -> setEnabled(true);
        widget1 = createGraphicsWindow(0,0,3200, 2400, "", false, node);
        widget2 = createGraphicsWindow(0,0,3200, 2400, "", false, model); //osgDB::readNodeFile("glider.osgt") );
        widget3 = createGraphicsWindow(0,0,3200, 2400, "", false, model); // osgDB::readNodeFile("axes.osgt") );
        widget4 = createGraphicsWindow(0,0,3200, 2400, "", false, model);// osgDB::readNodeFile("fountain.osgt") );
        popupWidget = createGraphicsWindow(900,100,320,240,"Popup window",true, osgDB::readNodeFile("dumptruck.osgt") );
        // popupWidget->show();

        QGridLayout* grid = new QGridLayout;
        grid -> setVerticalSpacing(0);
        grid -> setHorizontalSpacing(0);
        grid->addWidget( widget1, 0, 0 );
        grid->addWidget( widget2, 0, 1 );
        grid->addWidget( widget3, 1, 0 );
        grid->addWidget( widget4, 1, 1 );
        setLayout( grid );

        setMouseTracking(true);
        //connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
        //_timer.start( 10 );
    }

    bool
    event(QEvent * event)
    {
        update();
        return QWidget::event(event);
    }

    // bool
    // resizeEvent(QResizeEvent * event)
    // {
    //     widget1 -> resizeEvent(event);
    //     widget2 -> resizeEvent(event);
    //     widget3 -> resizeEvent(event);
    //     widget4 -> resizeEvent(event);
    //     popupWidget -> resizeEvent(event);
    //     return QWidget::resizeEvent(event);
    // }

  osgQt::GLWidget* createGraphicsWindow( int x, int y, int w, int h, const std::string& name= "Aviral's Window", bool windowDecoration=true, osg::Node * scene = 0)
    {
        osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
        ds -> setMinimumNumStencilBits(1);
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits(ds);
        traits -> windowName = name;
        traits -> windowDecoration = windowDecoration;
        traits -> x = x;
        traits -> y = y;
        traits -> width = w;
        traits -> height = h;
        traits -> doubleBuffer = true;
        traits -> alpha = ds->getMinimumNumAlphaBits();
        traits -> stencil = 1;//ds->getMinimumNumStencilBits();
        traits -> sampleBuffers = ds->getMultiSamples();
        traits -> samples = ds->getNumMultiSamples();
        osgQt::GLWidget * gl_widget = new osgQt::GLWidget(osgQt::GraphicsWindowQt::traits2qglFormat(traits.get()), NULL, NULL, Qt::Window | Qt::CustomizeWindowHint, true);
        gl_widget -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        gl_widget -> setForwardKeyEvents(true);

        osgQt::GraphicsWindowQt * gw = new osgQt::GraphicsWindowQt(gl_widget);
        //gl_widget -> setGraphicsWindow(gw);
        osgViewer::View* view = new osgViewer::View;
        addView( view );

        osg::Camera* camera = view->getCamera();
        camera->setGraphicsContext( gw );
        camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
        camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
        camera->setProjectionMatrixAsPerspective( 30.0f
                                                , static_cast<double>(traits->width) / static_cast<double>(traits->height)
                                                , 1.0f
                                                , 10000.0f
                                                );
        camera->setClearMask(camera -> getClearMask() | GL_STENCIL_BUFFER_BIT);
        camera->setClearStencil(0);
        view->setSceneData( scene );
        view->addEventHandler( new osgViewer::StatsHandler );
        view->setCameraManipulator( new osgGA::MultiTouchTrackballManipulator );
        //gw->setTouchEventsEnabled( true );
        std::cerr << gw -> getTraits() -> supportsResize << std::endl;
        // gl_widget->setWindowTitle( traits->windowName.c_str() );
        // gl_widget->move( traits->x, traits->y );
        // if (!traits->supportsResize)
        //   gl_widget->setFixedSize(traits->width, traits->height);
        // else
        gw -> resized(x, y, w, h);
        gl_widget->resize(traits->width, traits->height);

        return gl_widget;
        //return gw -> getGLWidget();//gl_widget;
    }

    virtual void paintEvent( QPaintEvent* event )
    {
      frame();
    }

protected:

    QTimer _timer;
};

int main( int argc, char** argv )
{
    osg::ArgumentParser arguments(&argc, argv);

#if QT_VERSION >= 0x050000
    // Qt5 is currently crashing and reporting "Cannot make QOpenGLContext current in a different thread" when the viewer is run multi-threaded, this is regression from Qt4
    osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::SingleThreaded;
#else
    osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
#endif

    while (arguments.read("--SingleThreaded")) threadingModel = osgViewer::ViewerBase::SingleThreaded;
    while (arguments.read("--CullDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
    while (arguments.read("--DrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::DrawThreadPerContext;
    while (arguments.read("--CullThreadPerCameraDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext;

#if QT_VERSION >= 0x040800
    // Required for multithreaded QGLWidget on Linux/X11, see http://blog.qt.io/blog/2011/06/03/threaded-opengl-in-4-8/
    if (threadingModel != osgViewer::ViewerBase::SingleThreaded)
        QApplication::setAttribute(Qt::AA_X11InitThreads);
#endif
    
    QApplication app(argc, argv);
    ViewerWidget* viewWidget = new ViewerWidget(0, Qt::Widget, osgViewer::ViewerBase::SingleThreaded);
    viewWidget->setGeometry( 100, 100, 800, 600 );
    viewWidget->show();
    return app.exec();
}
