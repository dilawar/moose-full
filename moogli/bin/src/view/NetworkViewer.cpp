#include "view/NetworkViewer.hpp"


NetworkViewer::NetworkViewer(Network* network, QWidget* parent,
    const QGLWidget* share_widget, Qt::WindowFlags f)
    : network(network)
    , QGLWidget(parent, share_widget, f)
    , _graphics_window(new osgViewer::GraphicsWindowEmbedded(
          this->x(), this->y(), this->width(), this->height()))
    , _viewer(new osgViewer::CompositeViewer)
    , capture_format("jpeg")
    , capture_location("/home/aviral/moogli_test")
{

    QGLWidget::setFormat(QGLFormat(QGL::SampleBuffers));
    // Light * light;
    // LightSource * light_source;

    // light = new osg::Light;
    // light->setLightNum( 0 );
    // light->setDiffuse( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setAmbient( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setDirection( osg::Vec3(1.0f, 0.0f, 0.0f) );
    // light->setPosition( osg::Vec4(0,0,0,0) );      // last param   w = 0.0
    // directional light (direction)
    //                                                 //              w = 1.0
    //                                                 point light (position)
    // // light source
    // light_source = new osg::LightSource;
    // light_source->setLight( light );
    // network-> node ->addChild( light_source );

    // light = new osg::Light;
    // light->setLightNum( 1 );
    // light->setDiffuse( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setAmbient( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setDirection( osg::Vec3(-1.0f, 0.0f, 0.0f) );
    // light->setPosition( osg::Vec4(0,0,0,0) );      // last param   w = 0.0
    // directional light (direction)
    //                                                 //              w = 1.0
    //                                                 point light (position)
    // // light source
    // light_source = new osg::LightSource;
    // light_source->setLight( light );
    // network-> node ->addChild( light_source );

    // light = new osg::Light;
    // light->setLightNum( 2 );
    // light->setDiffuse( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setAmbient( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setDirection( osg::Vec3(0.0f, 1.0f, 0.0f) );
    // light->setPosition( osg::Vec4(0,0,0,0) );      // last param   w = 0.0
    // directional light (direction)
    //                                                 //              w = 1.0
    //                                                 point light (position)
    // // light source
    // light_source = new osg::LightSource;
    // light_source->setLight( light );
    // network-> node ->addChild( light_source );

    // light = new osg::Light;
    // light->setLightNum( 3 );
    // light->setDiffuse( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setAmbient( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setDirection( osg::Vec3(0.0f, -1.0f, 0.0f) );
    // light->setPosition( osg::Vec4(0,0,0,0) );      // last param   w = 0.0
    // directional light (direction)
    //                                                 //              w = 1.0
    //                                                 point light (position)
    // // light source
    // light_source = new osg::LightSource;
    // light_source->setLight( light );
    // network-> node ->addChild( light_source );

    // light = new osg::Light;
    // light->setLightNum( 4 );
    // light->setDiffuse( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setAmbient( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setDirection( osg::Vec3(0.0f, 0.0f, 1.0f) );
    // light->setPosition( osg::Vec4(0,0,0,0) );      // last param   w = 0.0
    // directional light (direction)
    //                                                 //              w = 1.0
    //                                                 point light (position)
    // // light source
    // light_source = new osg::LightSource;
    // light_source->setLight( light );
    // network-> node ->addChild( light_source );

    // light = new osg::Light;
    // light->setLightNum( 5 );
    // light->setDiffuse( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setAmbient( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
    // light->setDirection( osg::Vec3(0.0f, 0.0f, -1.0f) );
    // light->setPosition( osg::Vec4(0,0,0,0) );      // last param   w = 0.0
    // directional light (direction)
    //                                                 //              w = 1.0
    //                                                 point light (position)
    // // light source
    // light_source = new osg::LightSource;
    // light_source->setLight( light );
    // network-> node ->addChild( light_source );

    // network -> node ->getOrCreateStateSet()->setMode( GL_LIGHT0,
    // osg::StateAttribute::OFF );
    // network -> node ->getOrCreateStateSet()->setMode( GL_LIGHT1,
    // osg::StateAttribute::ON );
    // network -> node ->getOrCreateStateSet()->setMode( GL_LIGHT2,
    // osg::StateAttribute::ON );
    // network -> node ->getOrCreateStateSet()->setMode( GL_LIGHT3,
    // osg::StateAttribute::ON );
    // network -> node ->getOrCreateStateSet()->setMode( GL_LIGHT4,
    // osg::StateAttribute::ON );
    // network -> node ->getOrCreateStateSet()->setMode( GL_LIGHT5,
    // osg::StateAttribute::ON );

    // network -> node ->getOrCreateStateSet()->setMode( GL_LIGHT0,
    // osg::StateAttribute::OFF );

    osg::DisplaySettings::instance()->setNumMultiSamples(16);
    osg::StateSet* stateSet = network->node->getOrCreateStateSet();
    osg::Material* material = new osg::Material;

    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    material->setAlpha(osg::Material::FRONT_AND_BACK, 1.0);
    material->setShininess(osg::Material::FRONT_AND_BACK, 128.0);
    material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));
    material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));

    // material->setColorMode(osg::Material::EMISSION);
    // material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));
    // material->setShininess(osg::Material::FRONT_AND_BACK, 128.0);
    // material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));
    // material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));

   //    stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
    //    stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    //    stateSet->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE); // just added this now
    // as a test but still
    // no luck
    osg::ShadeModel* shade_model = new osg::ShadeModel(osg::ShadeModel::SMOOTH);
    //    stateSet->setAttributeAndModes(shade_model, osg::StateAttribute::ON);
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    _previous_width = this->width();
    _previous_height = this->height();
    add_view( "main"
            , new osg::Viewport( this -> x()
                               , this -> y()
                               , this -> width() // 1366
                               , this -> height() // 768
                               )
            );

    // add_view( "commoner"
    //         , new osg::Viewport( this -> width() / 2
    //                                , this -> y()
    //                                , this -> width() / 2 // 1366
    //                                , this -> height()// 768
    //                                )
    //         );




    //    add_view(0, 0, this->width(), this->height());
    // add_view(0, 0, this->width() / 5, this->height() / 5);
    // add_view(this -> width()/2, 0, this -> width()/2, this -> height());
    //_viewer -> addCamera(create_hud_camera(0, 0, this -> width(), this -> height()))
    _viewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);

    // This ensures that the widget will receive keyboard events. This focus
    // policy is not set by default. The default, Qt::NoFocus, will result in
    // keyboard events that are ignored.
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMinimumSize(200, 200);

    // Ensures that the widget receives mouse move events even though no
    // mouse button has been pressed. We require this in order to let the
    // graphics window switch viewports properly.
    this->setMouseTracking(true);
    const osg::BoundingSphere& bs = network->node->getBound();

    up_distance = bs.radius() * 0.01;
    down_distance = bs.radius() * 0.01;
    left_distance = bs.radius() * 0.01;
    right_distance = bs.radius() * 0.01;
    forward_distance = bs.radius() * 0.01;
    backward_distance = bs.radius() * 0.01;
    zoom_factor = 0.10;
    roll_angle = M_PI / 36.0;
    pitch_angle = M_PI / 36.0;
    yaw_angle = M_PI / 36.0;



    // osgViewer::Viewer::Windows windows;
    // _viewer -> getWindows(windows);

    // osg::Camera* hud_camera = create_hud_camera(0, this -> width(), 0, this -> height());

    // set up cameras to render on the first window available.
   //, _graphics_window -> getTraits() -> height
    //                         );

    ///osgViewer::ViewerBase::Views views;
    ///_viewer -> getViews(views);

    ///hud_camera -> setGraphicsContext(_graphics_window);
    ///hud_camera -> setViewport(views[0] -> getCamera() -> getViewport());
    ///views[0] -> addSlave(hud_camera, false);

    // osgViewer::View* hud_view = new osgViewer::View;
    // hud_view->setCamera(hud_camera);
    // _hud_view = hud_view;
    // _viewer -> addView(hud_view);

}




//     osg::Geode* geode = new osg::Geode();

//     std::string timesFont("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf");

//   // turn lighting off for the text and disable depth test to ensure it's always ontop.
//     osg::StateSet* stateset = geode->getOrCreateStateSet();
//     stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

//     osg::Vec3 position(150.0f,800.0f,0.0f);
//     osg::Vec3 delta(0.0f,-120.0f,0.0f);

//   {
//       osgText::Text* text = new  osgText::Text;
//       geode->addDrawable( text );

//       text->setFont(timesFont);
//       text->setPosition(position);
//       text->setText("Sample Text");

//       position += delta;
//   }
//   network -> node -> addChild(geode);
//   camera -> addChild(geode);

//   osgSim::ScalarBar * geode1 = new osgSim::ScalarBar;
//   osgSim::ScalarBar::TextProperties tp;
//   tp._fontFile = "/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf";
//   geode1->setTextProperties(tp);
//   osg::StateSet * stateset1 = geode1->getOrCreateStateSet();
//   stateset1->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//   stateset1->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
//   stateset1->setRenderBinDetails(11, "RenderBin");

//   osg::MatrixTransform * modelview = new osg::MatrixTransform;
//   modelview->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//   osg::Matrixd matrix( osg::Matrixd::scale(1000,1000,1000)
//                      * osg::Matrixd::translate(120,10,0)
//                      ); // I've played with these values a lot and it seems to work, but I have no idea why
//   modelview->setMatrix(matrix);
//   modelview->addChild(geode1);
//   // camera -> addChild(modelview);
//   return camera;
// }


osg::Billboard *
NetworkViewer::create_billboard()
{
    return new osg::Billboard();
}

/*
osg::Billboard *
create_billboard(const osg::Billboard::Mode & rotation_mode, const osg::Vec3f & rotation_axis, const osg::Vec3f & front_face_normal)
{
    osg::Billboard * billboard = new Billboard();
    billboard -> setMode(rotation_mode);
    billboard -> setAxis(rotation_axis);
    billboard -> setNormal(front_face_normal);
    return billboard;
}
*/


// osg::Geode *
// axis( const osg::Vec3f & center
//     , const float radius
//     , const float height
//     , const osg::Vec3f & direction
//     , const osg::Vec4f & color
//     , const std::string label
//     )
// {
//   osg::Geode * geode = new Geode();
//   osg::Cylinder * axis = new osg::Cylinder(center , radius, height);
//   osg::Quat rotation;
//   rotation.makeRotate(osg::Z_AXIS , direction);
//   axis -> setRotation(rotation);
//   osg::ShapeDrawable * shape  = new osg::ShapeDrawable(axis);
//   shape -> setColor(color);
//   osgText::Text* text = new osgText::Text;
//   std::string timesfont("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf");
//   osg::Vec3 position(center + osg::Vec3f(radius / 2.0f, 0.0f, 0.0f));
//   text -> setFont(timesfont);
//   text -> setPosition(position);
//   text -> setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
//   text -> setText("X");
//   text -> setCharacterSize(20.0f);
//   text -> setAxisAlignment(osgText::TextBase::SCREEN);
//   geode -> addDrawable(text);
//   return geode;
// }

osg::Geode *
NetworkViewer::coordinate_system( const Vec3f & center
                                , const float length
                                , const float radius
                                )
{

    float cone_length = length / 5.0f;
    osg::Geode * geode = new osg::Geode();

    osg::Cylinder * x_axis = new osg::Cylinder(center + osg::X_AXIS * length / 2.0f, radius, length);
    osg::Cone     * x_cone = new osg::Cone(center + osg::X_AXIS * length , 2 * radius, cone_length);
    osg::Quat x_direction; x_direction.makeRotate(osg::Z_AXIS , osg::X_AXIS);
    x_axis -> setRotation(x_direction);
    x_cone -> setRotation(x_direction);
    osg::ShapeDrawable * x_axis_shape = new osg::ShapeDrawable(x_axis);
    osg::ShapeDrawable * x_cone_shape = new osg::ShapeDrawable(x_cone);
    x_axis_shape -> setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    x_cone_shape -> setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    geode -> addDrawable(x_axis_shape);
    geode -> addDrawable(x_cone_shape);

    osg::Cylinder * y_axis = new osg::Cylinder(center + osg::Y_AXIS * length / 2.0f, radius, length);
    osg::Cone     * y_cone = new osg::Cone(center + osg::Y_AXIS * length , 2 * radius, cone_length);
    osg::Quat y_direction; y_direction.makeRotate(osg::Z_AXIS, osg::Y_AXIS);
    y_axis -> setRotation(y_direction);
    y_cone -> setRotation(y_direction);
    osg::ShapeDrawable * y_axis_shape = new osg::ShapeDrawable(y_axis);
    osg::ShapeDrawable * y_cone_shape = new osg::ShapeDrawable(y_cone);
    y_axis_shape -> setColor(osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    y_cone_shape -> setColor(osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    geode -> addDrawable(y_axis_shape);
    geode -> addDrawable(y_cone_shape);

    osg::Cylinder * z_axis = new osg::Cylinder(center + osg::Z_AXIS * length / 2.0f, radius, length);
    osg::Cone     * z_cone = new osg::Cone(center + osg::Z_AXIS * length, 2 * radius, cone_length);
    // osg::Quat z_direction; z_direction.makeRotate(z_axis -> getRotation(), osg::Z_AXIS);
    // z_axis -> setRotation(z_direction);
    osg::ShapeDrawable * z_axis_shape = new osg::ShapeDrawable(z_axis);
    osg::ShapeDrawable * z_cone_shape = new osg::ShapeDrawable(z_cone);
    z_axis_shape -> setColor(osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    z_cone_shape -> setColor(osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    geode -> addDrawable(z_axis_shape);
    geode -> addDrawable(z_cone_shape);

    {
      osgText::Text* text = new osgText::Text;
      std::string timesfont("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf");
      osg::Vec3 position(center + osg::Vec3f(length + cone_length, 0.0f, 0.0f));
      text -> setFont(timesfont);
      text -> setPosition(position);
      text -> setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
      text -> setText("X");
      text -> setCharacterSize(radius * 5.0f);
      text -> setAxisAlignment(osgText::TextBase::SCREEN);
      geode -> addDrawable(text);
    }
    {
      osgText::Text* text = new osgText::Text;
      std::string timesfont("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf");
      osg::Vec3 position(center + osg::Vec3f(0.0f, length + cone_length, 0.0f));
      text -> setFont(timesfont);
      text -> setPosition(position);
      text -> setColor(osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
      text -> setText("Y");
      text -> setCharacterSize(radius * 5.0f);
      text -> setAxisAlignment(osgText::TextBase::SCREEN);
      geode -> addDrawable(text);
    }
    {
      osgText::Text* text = new osgText::Text;
      std::string timesfont("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf");
      osg::Vec3 position(center + osg::Vec3f(0.0f, 0.0f, length + cone_length));
      text -> setFont(timesfont);
      text -> setPosition(position);
      text -> setColor(osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
      text -> setText("Z");
      text -> setCharacterSize(radius * 5.0f);
      text -> setAxisAlignment(osgText::TextBase::SCREEN);
      geode -> addDrawable(text);
    }
 
    return geode;
    // CylinderMesh cylinder_mesh;
    // osg::Geode * coordinates = new osg::Geode();
    // osg::Geometry * px_axis = new osg::Geometry();
    // height = height * 4;
    // cylinder_mesh( center + osg::Vec3f(1.0f, 0.0f, 0.0f) * height / 4.0f
    //              , radius / 16.0f
    //              , radius / 16.0f
    //              , height / 2.0f
    //              , osg::Vec3f(1.0f, 0.0f, 0.0f)
    //              , px_axis
    //              , 20
    //              , osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f)
    //              );
    // osg::Geometry * nx_axis = new osg::Geometry();
    // cylinder_mesh( center + osg::Vec3f(-1.0f, 0.0f, 0.0f) * height / 4.0f
    //              , radius / 16.0f
    //              , radius / 16.0f
    //              , height / 2.0f
    //              , osg::Vec3f(-1.0f, 0.0f, 0.0f)
    //              , nx_axis
    //              , 20
    //              , osg::Vec4f(1.0f, 0.0f, 0.0f, 0.5f)
    //              );

    // osg::Geometry * py_axis = new osg::Geometry();
    // cylinder_mesh( center + osg::Vec3f(0.0f, 1.0f, 0.0f) * height / 4.0f
    //              , radius / 16.0f
    //              , radius / 16.0f
    //              , height / 2.0f
    //              , osg::Vec3f(0.0f, 1.0f, 0.0f)
    //              , py_axis
    //              , 20
    //              , osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f)
    //              );
    // osg::Geometry * ny_axis = new osg::Geometry();
    // cylinder_mesh( center + osg::Vec3f(0.0f, -1.0f, 0.0f) * height / 4.0f
    //              , radius / 16.0f
    //              , radius / 16.0f
    //              , height / 2.0f
    //              , osg::Vec3f(0.0f, -1.0f, 0.0f)
    //              , ny_axis
    //              , 20
    //              , osg::Vec4f(0.0f, 1.0f, 0.0f, 0.5f)
    //              );

    // osg::Geometry * pz_axis = new osg::Geometry();
    // cylinder_mesh( center + osg::Vec3f(0.0f, 0.0f, 1.0f) * height / 4.0f
    //              , radius / 16.0f
    //              , radius / 16.0f
    //              , height / 2.0f
    //              , osg::Vec3f(0.0f, 0.0f, 1.0f)
    //              , pz_axis
    //              , 20
    //              , osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f)
    //              );
    // osg::Geometry * nz_axis = new osg::Geometry();
    // cylinder_mesh( center + osg::Vec3f(0.0f, 0.0f, -1.0f) * height / 4.0f
    //              , radius / 16.0f
    //              , radius / 16.0f
    //              , height / 2.0f
    //              , osg::Vec3f(0.0f, 0.0f, -1.0f)
    //              , nz_axis
    //              , 20
    //              , osg::Vec4f(0.0f, 0.0f, 1.0f, 0.5f)
    //              );

    // osg::Geometry * pn_axis = new osg::Geometry();
    // cylinder_mesh( center + direction * height / 4.0f
    //              , radius / 16.0f
    //              , radius / 16.0f
    //              , height / 2.0f
    //              , direction
    //              , pn_axis
    //              , 20
    //              , osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f)
    //              );
    // osg::Geometry * nn_axis = new osg::Geometry();
    // cylinder_mesh( center -direction * height / 4.0f
    //              , radius / 16.0f
    //              , radius / 16.0f
    //              , height / 2.0f
    //              , -direction
    //              , nn_axis
    //              , 20
    //              , osg::Vec4f(1.0f, 1.0f, 0.0f, 0.5f)
    //              );

    // coordinates -> addDrawable(px_axis);
    // coordinates -> addDrawable(py_axis);
    // coordinates -> addDrawable(pz_axis);
    // coordinates -> addDrawable(pn_axis);
    // coordinates -> addDrawable(nx_axis);
    // coordinates -> addDrawable(ny_axis);
    // coordinates -> addDrawable(nz_axis);
    // coordinates -> addDrawable(nn_axis);

    // return coordinates;
}


osg::MatrixTransform *
NetworkViewer::create_scene_data()
{
  osg::MatrixTransform * scene_data = new MatrixTransform();
  scene_data -> addChild(network -> node);
  osg::Billboard * billboard = create_billboard();
  scene_data -> addChild(billboard);
  osg::Geode * geode = new Geode();
  scene_data -> addChild(geode);
  {
     osgText::FadeText* text = new osgText::FadeText;
     std::string timesfont("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf");
     osg::Vec3 position(0.0f,0.0f,0.0f);
     text -> setFont(timesfont);
     text -> setPosition(position);
     text -> setText("Sample Text");
     text -> setCharacterSize(30.0f);
     text -> setAxisAlignment(osgText::TextBase::SCREEN);
     text -> setFadeSpeed(0.001);
     //geode -> addDrawable(text);
  }
  {
       osgText::Text* text = new osgText::Text;
       std::string timesfont("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf");
       osg::Vec3 position(100.0f,0.0f,0.0f);
       text -> setFont(timesfont);
       text -> setPosition(position);
       text -> setText("Sample Text");
       text -> setCharacterSize(30.0f);
       text -> setAxisAlignment(osgText::TextBase::SCREEN);
       //geode -> addDrawable(text);
  }
  {
    osgText::Text3D* text = new osgText::Text3D;
    std::string timesfont("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf");
    osg::Vec3 position(100.0f,100.0f,0.0f);
    text -> setFont(timesfont);
    text -> setPosition(position);
    text -> setText("Sample Text");
    text -> setCharacterSize(30.0f);
    text -> setCharacterDepth(10.0f);
    //text -> setAxisAlignment(osgText::TextBase::XY_PLANE);
    text -> setAxisAlignment(osgText::TextBase::SCREEN);
    text -> setAutoRotateToScreen(true);
    //geode -> addDrawable(text);




  }
  {
    const osg::BoundingSphere& bs = network->node->getBound();
    float radius = bs.radius();
    osg::Geode * geode = new osg::Geode();
    osg::InfinitePlane * plane = new osg::InfinitePlane();
    plane -> set(osg::Vec3f(1.0f, 0.0f, 0.0f), bs.center());
    osg::ShapeDrawable * plane_shape = new osg::ShapeDrawable(plane);
    plane_shape -> setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    geode -> addDrawable(plane_shape);
    scene_data -> addChild(geode);
  }
  //billboard -> setMode(osg::Billboard::POINT_ROT_WORLD);
  billboard -> setNormal(osg::Vec3f(1.0f, -1.0f, 0.0f));
  return scene_data;
}

osg::Camera *
NetworkViewer::create_master_camera( osg::Viewport * viewport
                                   , osg::Vec4f clear_color
                                   , double fovy
                                   , double z_near
                                   , double z_far
                                   )
{
  osg::Camera * camera = new osg::Camera();
  camera -> setViewport(viewport);
  camera -> setProjectionMatrixAsPerspective( fovy
                                            , viewport -> aspectRatio()
                                            , z_near
                                            , z_far
                                            );
  camera -> setClearColor(clear_color);
  camera -> setGraphicsContext(_graphics_window);
  camera -> getOrCreateStateSet() -> setGlobalDefaults();
  camera -> setPostDrawCallback(new CaptureView());
  return camera;
}

osg::Camera*
NetworkViewer::create_hud_camera(osg::Viewport * viewport)
{
    osg::Camera * camera = new osg::Camera;

    // set the projection matrix
    camera -> setProjectionMatrix(osg::Matrix::ortho2D( 0 //viewport -> x()
                                                      , viewport -> width()
                                                      , 0 // viewport -> y()
                                                      , viewport -> height()
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
    camera -> setGraphicsContext(_graphics_window);
    camera -> setViewport(viewport);
    return camera;
}


osgViewer::View *
NetworkViewer::create_view( const char * id
                          , osg::Viewport * viewport
                          , osg::Camera * master_camera
                          , osg::Camera * hud_camera
                          , osg::MatrixTransform * scene_data
                          , osgGA::StandardManipulator * camera_manipulator
                          )
{
    osgViewer::View* view = new osgViewer::View();
    view -> setName(id);
    view -> setCamera(master_camera);
    view -> addSlave(hud_camera, false);
    view -> setSceneData(scene_data);
    view -> setCameraManipulator(camera_manipulator);
    view -> setLightingMode(osg::View::HEADLIGHT);
    view -> addEventHandler(new osgViewer::ScreenCaptureHandler());
    return view;
}

osgViewer::View *
NetworkViewer::add_view( const char * id
        , osg::Viewport * viewport
        , osg::Vec4f clear_color
        , osgGA::StandardManipulator * camera_manipulator
        , double fovy
        , double z_near
        , double z_far
        )
{
  osgViewer::View * view = create_view( id
                                  , viewport
                                  , create_master_camera( viewport
                                                        , clear_color
                                                        , fovy
                                                        , z_near
                                                        , z_far
                                                        )
                                  , create_hud_camera(viewport)
                                  , create_scene_data()
                                  , camera_manipulator
                                  );
  // https://groups.google.com/forum/#!searchin/osg-users/move$20with$20camera/osg-users/HxSTSSOS-fE/HeqYeqrezPkJ
  // https://groups.google.com/forum/#!searchin/osg-users/move$20with$20camera/osg-users/jowv_ou9dWs/A214Wy35JKgJ
  // https://groups.google.com/forum/#!searchin/osg-users/move$20with$20camera/osg-users/RbxhdyeBVpU/OC2VprlSF4oJ
  // https://groups.google.com/forum/#!searchin/osg-users/move$20with$20camera/osg-users/O-G5Vy2TBxo/r828XByPrHgJ
  osg::Camera * camera = new osg::Camera();
  camera -> setViewport(viewport);
  camera -> setProjectionMatrixAsPerspective( fovy
                                            , viewport -> aspectRatio()
                                            , z_near
                                            , z_far
                                            );
  //camera -> setClearColor(clear_color);
  camera -> setGraphicsContext(_graphics_window);
  camera -> getOrCreateStateSet() -> setGlobalDefaults();
  camera -> setPostDrawCallback(new CaptureView());
  // camera -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
  // camera -> setViewMatrix(osg::Matrix::identity());
  // only clear the depth buffer
  // camera -> setClearMask(GL_DEPTH_BUFFER_BIT);

  // draw subgraph after main camera view.
  camera -> setRenderOrder(osg::Camera::POST_RENDER);

  // we don't want the camera to grab event focus from the viewers main camera(s).
  camera -> setAllowEventFocus(false);
  const osg::BoundingSphere& bs = network->node->getBound();
  float radius = bs.radius();
  osg::Vec3f center = bs.center();
  // scene_data -> addChild(coordinate_system(center, radius));

  // osg::AutoTransform * absolute_node = new osg::Transform();
  // absolute_node -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
  // //absolute_node -> setViewMatrix(osg::Matrix::identity());
  // absolute_node -> addChild(coordinate_system(center, radius));
  // //scene_data -> addChild(absolute_node);

  CoordinateSystem * system = new CoordinateSystem();
  system -> setStateSet( network -> node -> getOrCreateStateSet());

  system -> getStateSet() -> setMode(GL_BLEND, osg::StateAttribute::ON);
  system -> camera = view -> getCamera();
  system -> addChild(coordinate_system(osg::Vec3f(0.0f, 0.0f, 0.0f), 50.0f, 2.5f));
  static_cast<osg::MatrixTransform *>(view -> getSceneData()) -> addChild(system);


  //view -> addSlave(camera, true);

  camera -> addChild(coordinate_system(osg::Vec3f(0.0f, 0.0f, 0.0f), 50.0f, 2.5f));
  _viewer -> addView(view);
  _views[id] = view;
  return view;

}

osgText::Text *
NetworkViewer::create_two_d_texts( const char * id
                                 , const char * content
                                 , const osg::Vec3 position
                                 )
                          //   osg::Vec4f color
                          //   const char * font
                          //   font_resolution_width
                          //   font_resolution_height
                          //   character_height
                          //   character_aspect_ratio
                          // )
{
    osg::Geode* geode = new osg::Geode();

    std::string font("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf");

    // turn lighting off for the text and disable depth test to ensure it's always ontop.
    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    osgText::Text* text = new  osgText::Text;
    geode->addDrawable(text);
    text->setFont(font);
    text->setPosition(position);
    text->setText(content);
    _two_d_texts[id] = geode;
}

    // bool
    // NetworkViewer::show_text_2D()
    // {
    //   osg::Geode * geode =  _twod_texts[id];
    //   _views[view_id] -> getSceneData() -> asGroup() -> getChild() -> 
    // }
    //                             //     float character_size, osg::Vec4f color,
    //     osg::Vec4f background_color,
    //     osgText::TextBase::DrawMode draw_mode, )
    // {
    //     osg::ref_ptr<osgText::Text> text = new osgText::Text;
    //     text->setFont(_font.get());
    //     text->setCharacterSize(character_size);
    //     text->setAxisAlignment(osgText::TextBase::XY_PLANE);
    //     text->setPosition(position);
    //     text->setText(content);
    //     text->setColor(Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    //     text->setBackdropColor(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    //     text->setBackdropType(osgText::Text::OUTLINE);
    //     text->setDrawMode(osgText::TextBase::TEXT | osgText::TextBase::FILLEDBOUNDINGBOX);
    //     text->setAlignment(osgText::TextBase::LEFT_CENTER);
    //     // Quat quat;
    //     // quat.makeRotate( 0.0f, Vec3f(1.0f, 0.0f, 0.0f)
    //     //                , M_PI, Vec3f(0.0f, 1.0f, 0.0f)
    //     //                , M_PI, Vec3f(0.0f, 0.0f, 1.0f)
    //     //                );
    //     // text -> setRotation(quat);
    //     // text -> setAutoRotateToScreen(true);
    //     return text.release();
    // }
    // 
    // void NetworkViewer::insert_text(const char* text, ) {}

    // void NetworkViewer::hide_text() {}

    // void NetworkViewer::show_text() {}

    // void NetworkViewer::remove_text() {}

void
NetworkViewer::capture( const std::string & viewer_id
                      , const std::string & filename
                      , const std::string & extension
                      , unsigned long frame_count
                      )
{
    CaptureView * capture_view = static_cast<CaptureView *>(_views[viewer_id] -> getCamera() -> getPostDrawCallback());
    capture_view -> start_capture(filename, extension, frame_count);
    return;
    osgViewer::ScreenCaptureHandler * handler = static_cast<osgViewer::ScreenCaptureHandler *>(_views[viewer_id] -> getEventHandlers().front().get());
    handler -> setFramesToCapture(frame_count);
    handler -> setCaptureOperation(new osgViewer::ScreenCaptureHandler::WriteToFile(filename, extension));
    handler -> startCapture();
}

void
NetworkViewer::set_background_color(const osg::Vec4f& color)
{
    _viewer->getView(0)->getCamera()->setClearColor(color);
}

bool
NetworkViewer::create_color_bar( const char * id
                               , const char * title
                               , std::pair<float, float> scalar_range
                               , std::vector< osg::Vec4 > colors
                               , int num_colors
                               , int num_labels
                               , float angle//osgSim::ScalarBar::Orientation orientation
                               , std::pair<float, float> dimensions
                               , osg::Vec3f position
                               , osg::Vec4 label_color
                               , const char * label_font_file
                               , std::pair<int, int> label_font_resolution
                               , float label_character_size
                               )
{
    std::cerr << num_colors << std::endl;
    std::cerr << num_labels << std::endl;
    std::cerr << colors[1][0] << ", " << colors[1][1] << ", " << colors[1][2] << ", " << colors[1][3] << std::endl;
    std::cerr << position[0] << ", " << position[1] << ", " << position[2] << std::endl;

    osgSim::ScalarBar::TextProperties text_properties;
    text_properties._fontFile = label_font_file;
    text_properties._fontResolution = label_font_resolution;
    text_properties._characterSize = label_character_size;
    text_properties._color = label_color;
    float length = dimensions.first;
    float breadth = dimensions.second;
    osgSim::ScalarBar * scalar_bar = new osgSim::ScalarBar( num_colors
                                                          , num_labels
                                                          , new osgSim::ColorRange( scalar_range.first
                                                                                  , scalar_range.second
                                                                                  , colors
                                                                                  )
                                                          , title
                                                          , osgSim::ScalarBar::HORIZONTAL
                                                          , breadth / length
                                                          );

    //    scalar_bar -> setPosition(position);
    scalar_bar -> setWidth(length);
    scalar_bar -> setTextProperties(text_properties);

    osg::StateSet * stateset = scalar_bar -> getOrCreateStateSet();
    stateset -> setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateset -> setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    stateset -> setRenderBinDetails(11, "RenderBin");

    const BoundingBox & bounding_box = scalar_bar -> getBoundingBox();
    length = bounding_box.xMax() - bounding_box.xMin();
    breadth = bounding_box.yMax() - bounding_box.yMin();
    float cosangle = cos(angle);
    float sinangle = sin(angle);
    osg::PositionAttitudeTransform * pat_transform = new osg::PositionAttitudeTransform();
    pat_transform -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    pat_transform -> setPosition( position
                                - osg::Vec3f( length * cosangle / 2.0f - breadth * sinangle / 2.0f
                                            , length * sinangle / 2.0f + breadth * cosangle / 2.0f
                                            , 0.0f
                                            )
                                );
    osg::Quat orientation;
    orientation.makeRotate(angle, 0.0, 0.0, 1.0);
    pat_transform -> setAttitude(orientation);
    //   etMatrix( osg::Matrixd::rotate(angle, 0.0, 0.0, 1.0)
    //                              * osg::Matrixd::translate(                                                           )
    //                              );
    pat_transform -> addChild(scalar_bar);
    _scalar_bars[id] = pat_transform;
    return true;
}


// void
// resize_color_bar(osg::PositionAttitudeTransform * pat_transform)
// {
//     osg::Vec3f position = pat_transform -> getPosition();
//     osg::Quat orientation = pat_transform -> getAttitude();
//     float angle;
//     osg::Vec3f axis;
//     orientation.getRotate(angle, axis);
//     osg::ScalarBar * scalar_bar = pat_transform -> getChild(0);
//     float length = scalar_bar -> getWidth();
//     float breadth = length * scalar_bar -> getAspectRatio();

// }

    osg::Camera *
    NetworkViewer::get_hud_camera(unsigned int view_index)
    {
        // osgViewer::ViewerBase::Cameras cameras;
        // _viewer.getCameras(cameras);
        // return cameras[1];
        // // Previous portion
        osgViewer::ViewerBase::Views views;
        _viewer->getViews(views);
        std::cerr << "View " << views[view_index] << std::endl;
        std::cerr <<"Number of Children : " << views[view_index] -> getCamera() -> getNumChildren() << std::endl;
        return views[view_index] -> getSlave(0)._camera.get();
    }

    osg::Billboard *
    NetworkViewer::get_billboard(unsigned int view_index)
    {
        osgViewer::ViewerBase::Views views;
        _viewer->getViews(views);
        return static_cast<osg::Billboard *>(views[view_index] -> getCamera() -> getChild(1));
    }


    bool
    NetworkViewer::hide_color_bar( const char * id
                                , unsigned int view_index
                                )
    {
        auto search = _scalar_bars.find(id);
        if (search == _scalar_bars.end()) { return false; }
        osg::Camera * hud_camera = get_hud_camera(view_index);
        return hud_camera -> removeChild((search -> second).get());
    }

// bool
// resize_color_bar( osg::MatrixTransform * matrix_transform
//                 , osg::Viewport * old_viewport
//                 , osg::Viewport * new_viewport
//                 )
// {
    
// }

    bool
    NetworkViewer::show_color_bar( const char * id
                                , unsigned int view_index
                                )
    {

        osgViewer::ViewerBase::Views views;
        _viewer -> getViews(views);
        // std::cerr << "View Count " << views.size() << std::endl;
        // osg::MatrixTransform * modelview = new osg::MatrixTransform;
        // modelview->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        // osg::Matrixd matrix( osg::Matrixd::scale(1000,1000,1000)
        //                      * osg::Matrixd::translate(1200,10,0)
        //                      ); // I've played with these values a lot and it seems to work, but I have no idea why
        // modelview->setMatrix(matrix);
        // modelview->addChild(geode1);

        views[0] -> getSlave(0)._camera -> addChild(_scalar_bars[id].get());
        std::cerr << "Viewport Size" << std::endl;
        std::cerr << views[0] -> getSlave(0)._camera -> getViewport() -> x() << std::endl;
        std::cerr << views[0] -> getSlave(0)._camera -> getViewport() -> y() << std::endl;
        std::cerr << views[0] -> getSlave(0)._camera -> getViewport() -> width() << std::endl;
        std::cerr << views[0] -> getSlave(0)._camera -> getViewport() -> height() << std::endl;
        return true;
        _hud_view -> getCamera() -> addChild(_scalar_bars[id].get());
        return true;
        // // network -> node -> asGroup() -> addChild(
        // osg::Projection * projection = create_heads_up_display( 0
        //                                                       , 0
        //                                                       , 1024
        //                                                       , 768
        //                                                       , _scalar_bars[id].get()
        //                                                       );

        // osg::Group * group = new osg::Group();
        // group -> addChild(projection);
        // group -> addChild(network -> node.get());
        // views[0] -> setSceneData(group);
        // return true;
        auto search = _scalar_bars.find(id);
        if (search == _scalar_bars.end()) { return false; }
        return true;
    }

    // bool
    // destroy_color_bar( const char * id
    //                  , unsigned int view_index
    //                  )
    // {
    //     auto search = _scalar_bars.find(id);
    //     if (search == _scalar_bars.end()) { return false; }
    //     DETACH_NODE(search -> second)
    //     _scalar_bars.erase(search);
    //     return true;
    // }

    // color_bar(const char * id)
    // {
    // }

    // void
    // modify_color_bar(const char * id)
    // {
    // }


        // Geometry *
        // NetworkViewer::create_arrow( const Vec3f & source
        //                            , const Vec3f & destination
        //                            , const Vec4f & color
        //                            )
        // {
        //     Vec3Array vertices(4);
        //     ref_ptr<DrawElementsUShort> indices()
        //     ref_ptr<DrawElementsUShort> indices(

        //     ref_ptr<Vec3Array> vertices(
        //         new Vec3Array(unit_vertices -> size())
        //                                );
        //     ref_ptr<Vec3Array> normals(
        //         new Vec3Array(unit_normals -> size())
        //                               );
        //     ref_ptr<DrawElementsUShort> indices(
        //         new DrawElementsUShort(*unit_indices)
        //                                        );

        // }

        // void
        // NetworkViewer::create_coordinate_axes()
        // {
        //     x_axis = new osg::Geometry();
        //     x_axis.
        //     coordinate_axes_node -> addDrawable(x_axis.get());
        //     coordinate_axes_node -> addDrawable(y_axis.get());
        //     coordinate_axes_node -> addDrawable(z_axis.get());
        // }

        // void
        // NetworkViewer::show_coordinate_axes()
        // {
        //     coordinate_axes_node -> setNodeMask(NODE_SHOW_MASK);
        // }

        // void
        // NetworkViewer::hide_coordinate_axes()
        // {
        //     coordinate_axes_node -> setNodeMask(NODE_HIDE_MASK);
        // }

        Network*
        NetworkViewer::get_network()
    {
        return network;
    }

    void NetworkViewer::split_horizontally(unsigned int view_index,
        unsigned int width_factor)
    {
      return;
        // float aspect_ratio = static_cast<float>(this->width()) / static_cast<float>(this->height());
        // osgViewer::View* parent_view = _viewer->getView(view_index);
        // osg::Camera* parent_camera = parent_view->getCamera();
        // osg::Viewport* parent_viewport = parent_camera->getViewport();
        // int new_parent_width = parent_viewport->width() * (width_factor - 1) / width_factor;
        // add_view(parent_viewport->x() + new_parent_width, parent_viewport->y(),
        //     parent_viewport->width() - new_parent_width,
        //     parent_viewport->height());
        // parent_camera->setViewport(parent_viewport->x(), parent_viewport->y(),
        //     new_parent_width, parent_viewport->height());
        // parent_camera->setProjectionMatrixAsPerspective(30.f, static_cast<float>(new_parent_width) / static_cast<float>(parent_viewport->height()),
        //     1.0f, 10000.0f);
    }

    void NetworkViewer::split_vertically( unsigned int view_index
                                        , unsigned int height_factor
                                        )
    {
      return;
    //     float aspect_ratio = static_cast<float>(this->width()) / static_cast<float>(this->height());
    //     osgViewer::View* parent_view = _viewer->getView(view_index);
    //     osg::Camera* parent_camera = parent_view->getCamera();
    //     osg::Viewport* parent_viewport = parent_camera->getViewport();

    //     // std::cout << "parent x      => "    << parent_viewport -> x() << std::endl;
    //     // std::cout << "parent y      => "    << parent_viewport -> y() << std::endl;
    //     // std::cout << "parent width  => "    << parent_viewport -> width() <<
    //     // std::endl;
    //     // std::cout << "parent height => "    << parent_viewport -> height() <<
    //     // std::endl;

    //     int new_parent_height = parent_viewport->height() * (height_factor - 1) / height_factor;
    //     add_view(parent_viewport->x(), parent_viewport->y() + new_parent_height,
    //         parent_viewport->width(),
    //         parent_viewport->height() - new_parent_height);

    //     // std::cout << "child x      => "    << parent_viewport -> x() << std::endl;
    //     // std::cout << "child y      => "    << parent_viewport -> y() +
    //     // new_parent_height << std::endl;
    //     // std::cout << "child width  => "    << parent_viewport -> width() <<
    //     // std::endl;
    //     // std::cout << "child height => "    << parent_viewport -> height() -
    //     // new_parent_height << std::endl;

    //     parent_camera->setViewport(parent_viewport->x(), parent_viewport->y(),
    //         parent_viewport->width(), new_parent_height);
    //     // std::cout << "parent2 x      => "    << parent_viewport -> x() <<
    //     // std::endl;
    //     // std::cout << "parent2 y      => "    << parent_viewport -> y() <<
    //     // std::endl;
    //     // std::cout << "parent2 width  => "    << parent_viewport -> width() <<
    //     // std::endl;
    //     // std::cout << "parent2 height => "    << new_parent_height << std::endl;

    //     parent_camera->setProjectionMatrixAsPerspective(
    //         30.0f, static_cast<float>(parent_viewport->width()) / static_cast<float>(new_parent_height),
    //         1.0f, 10000.0f);
    //
    }


NetworkViewer::~NetworkViewer() {}

void NetworkViewer::paintEvent(QPaintEvent* /* paintEvent */)
{
    // this->makeCurrent();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    this->paintGL();
    painter.end();
    this->swapBuffers();
    this->doneCurrent();
}

void NetworkViewer::paintGL() { _viewer->frame(); }

osg::Viewport *
NetworkViewer::compute_viewport( osg::Viewport * viewport
                                 , int width
                                 , int height
                                 )
{
    return new osg::Viewport( static_cast<int>(viewport -> x() * width * 1.0f / _previous_width)
                            , static_cast<int>(viewport -> y() * height * 1.0f / _previous_height)
                            , static_cast<int>(viewport -> width() * width * 1.0f / _previous_width)
                            , static_cast<int>(viewport -> height() * height * 1.0f / _previous_height)
                            );

}

void
NetworkViewer::set_master_camera_viewport( osg::Camera * camera
                                         , osg::Viewport * viewport
                                         )
{
    double fovy;
    double aspect_ratio;
    double z_near;
    double z_far;
    camera -> getProjectionMatrixAsPerspective( fovy
                                              , aspect_ratio
                                              , z_near
                                              , z_far
                                              );
    camera -> setProjectionMatrixAsPerspective( fovy
                                              , viewport -> aspectRatio()
                                              , z_near
                                              , z_far
                                              );
    camera -> setViewport(viewport);
}

void
NetworkViewer::set_hud_camera_viewport( osg::Camera * camera
                                      , osg::Viewport * viewport
                                      )
{
    camera -> setProjectionMatrix(osg::Matrix::ortho2D( 0 //viewport -> x()
                                                      , viewport -> width()
                                                      , 0 // viewport -> y()
                                                      , viewport -> height()
                                                      )
                                 );
    camera -> setViewport(viewport);
}

osg::Camera *
NetworkViewer::get_hud_camera(osgViewer::View * view)
{
    return view -> getSlave(0)._camera.get();
}

void
NetworkViewer::set_viewport( const char * view_id
                           , osg::Viewport * viewport
                           )
{
    set_viewport( _views[view_id].get()
                , viewport
                );
}

void
NetworkViewer::set_viewport( osgViewer::View * view
                           , osg::Viewport * viewport
                           )
{
    osg::Camera * master_camera = view -> getCamera();
    osg::Camera * hud_camera = get_hud_camera(view);
    set_master_camera_viewport(master_camera, viewport);
    set_hud_camera_viewport(hud_camera, viewport);
}

void
NetworkViewer::resize_views(int width, int height)
{
  osgViewer::ViewerBase::Views views;
  _viewer->getViews(views);
  for(osgViewer::View * view : views)
  {
      set_viewport( view
                  , compute_viewport( view -> getCamera() -> getViewport()
                                    , width
                                    , height
                                    )
                  );
  }
}


void NetworkViewer::resizeGL(int width, int height)
{
    this->getEventQueue()->windowResize(this->x(), this->y(), width, height);
    _graphics_window->resized(this->x(), this->y(), width, height);
    std::cerr << "Resized !!" << std::endl;
    std::cerr << this -> x() << std::endl;
    std::cerr << this -> y() << std::endl;
    std::cerr << this -> width() << std::endl;
    std::cerr << this -> height() << std::endl;
    //resize_views(width, height);
    this->onResize(width, height);
}

unsigned int
NetworkViewer::_get_view_index_with_focus()
{
    osgViewer::ViewerBase::Views views;
    osgViewer::View* view = _viewer->getViewWithFocus();
    _viewer->getViews(views);
    auto iter = std::find(views.begin(), views.end(), view);
    if (iter == views.end()) {
        return 0;
    }
    return (iter - views.begin());
}

void NetworkViewer::keyPressEvent(QKeyEvent* event)
{
    QString keyString = event->text();
    const char* keyData = keyString.toLocal8Bit().data();
    unsigned int view_index = _get_view_index_with_focus();
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
    case Qt::Key_Space:
        home(view_index);
        break;
    case Qt::Key_Up:
        up(up_distance, view_index);
        break;
    case Qt::Key_Down:
        down(down_distance, view_index);
        break;
    case Qt::Key_Left:
        left(left_distance, view_index);
        break;
    case Qt::Key_Right:
        right(right_distance, view_index);
        break;
    case Qt::Key_F:
        forward(forward_distance, view_index);
        break;
    case Qt::Key_B:
        backward(backward_distance, view_index);
        break;
    case Qt::Key_Plus:
        zoom(zoom_factor, view_index);
        break;
    case Qt::Key_Equal:
        zoom(zoom_factor, view_index);
        break;
    case Qt::Key_Period:
        zoom(zoom_factor, view_index);
        break;
    case Qt::Key_Greater:
        zoom(zoom_factor, view_index);
        break;
    case Qt::Key_Minus:
        zoom(-zoom_factor, view_index);
        break;
    case Qt::Key_Underscore:
        zoom(-zoom_factor, view_index);
        break;
    case Qt::Key_Comma:
        zoom(-zoom_factor, view_index);
        break;
    case Qt::Key_Less:
        zoom(-zoom_factor, view_index);
        break;
    case Qt::Key_R:
        if (event->modifiers() & Qt::ShiftModifier) {
            roll(-roll_angle, view_index);
        }
        else {
            roll(roll_angle, view_index);
        }
        break;
    case Qt::Key_P:
        if (event->modifiers() & Qt::ControlModifier) {
            int x = 0, y = 0;
            unsigned int width = 1000, height = 500;
            // viewer.getCamera() -> getProjectionRectangle(x,y,width,height);
            osg::ref_ptr<osg::Image> image = new osg::Image;
            image->readPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE);
            osgDB::writeImageFile(*image, "./image.jpg");
        }
        else if (event->modifiers() & Qt::ShiftModifier) {
            pitch(-pitch_angle, view_index);
        }
        else {
            pitch(pitch_angle, view_index);
        }
        break;
    case Qt::Key_Y:
        if (event->modifiers() & Qt::ShiftModifier) {
            yaw(-yaw_angle, view_index);
        }
        else {
            yaw(yaw_angle, view_index);
        }
        break;
    default:
        break;
    }
}

// void
// NetworkViewer::capture_continuous_toggle(unsigned int index)
// {
//     osgViewer::View * view = _viewer -> getView(index);
//     auto * handler = dynamic_cast<MorphologyCaptureHandler *>((view ->
//     getEventHandlers()).front().get());
//     handler -> toggle_capture();
// }

// void
// NetworkViewer::capture_once(unsigned int index)
// {
//     osgViewer::View * view = _viewer -> getView(index);
//     auto * handler = dynamic_cast<MorphologyCaptureHandler *>((view ->
//     getEventHandlers()).front().get());
//     handler->setFramesToCapture(1);
// }

// void
// NetworkViewer::export(const char * filename)
// {

// }

// void
// NetworkViewer::export(std::string filename, std::string format)
// {
//     auto* capture_operation =
//         new osgViewer::ScreenCaptureHandler::WriteToFile( filename, format);

// }

// void
// NetworkViewer::scale(std::string filename, std::string format)
// {
//     auto* capture_operation =
// new osgViewer::ScreenCaptureHandler::WriteToFile( filename, format);
// }

// void
// NetworkViewer::rotate(std::string filename, std::string format)
// {
//     auto* capture_operation =
// new osgViewer::ScreenCaptureHandler::WriteToFile( filename, format);
// }

// void
// NetworkViewer::translate()
// {
//     auto* capture_operation =
// new osgViewer::ScreenCaptureHandler::WriteToFile( filename, format);
// }

// osgText::Text* create_text_2d(const char* font_file,
//     const char* content const osg::Vec3& pos, const std::string&
//                                                   content,
//     float size)
// {
//     osg::ref_ptr<osgText::Font> font = osgText::readFontFile(font_file);
//     osg::ref_ptr<osgText::FadeText> fade_text = new osgText::FadeText;
//     text->setDataVariance(osg::Object::DYNAMIC);
//     text->setFont(font.get());
//     text->setCharacterSize(character_size);
//     text->setAxisAlignment(osgText::TextBase::XY_PLANE);
//     text->setPosition(position);
//     text->setText(content);
//     return text.release();
// }

// void show_text_2d(const char* text_id)
// {
// }

// void hide_text_2d(const char* text_id)
// {
// }

// void destroy_text_2d(const char* text_id)
// {
// }

// void create_text_3d(const char* text_3d_id)
// {
// }



osg::Projection *
NetworkViewer::create_heads_up_display(int x, int y, int width, int height, osg::Node * node)
{
  // network -> node -> asGroup() -> addChild(node);
    osg::StateSet * stateset = node -> getOrCreateStateSet();
    stateset -> setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateset -> setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    stateset -> setRenderBinDetails(11, "RenderBin");
    osg::MatrixTransform * matrix_transform = new osg::MatrixTransform;
    matrix_transform -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    // osg::Matrixd matrix(osg::Matrixd::scale(1000,1000,1000) * osg::Matrixd::translate(120,10,0)); // I've played with these values a lot and it seems to work, but I have no idea why
    // matrix_transform -> setMatrix(matrix);
    matrix_transform -> addChild(node);
    osg::Projection * projection = new osg::Projection;
    projection -> setMatrix(osg::Matrix::ortho2D(0,1366,0,768)); // or whatever the OSG window res is
    projection -> addChild(matrix_transform);
    return projection; //make sure you delete the return sb line
}

// osg::Billboard *
// NetworkViewer::create_user_facing_display(osg::Node * node)
// {
  
// }

//     osg::Camera * camera = new osg::Camera;
//     camera -> setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//     camera -> setClearMask(GL_DEPTH_BUFFER_BIT);
//     camera -> setRenderOrder(osg::Camera::POST_RENDER);
//     camera -> setAllowEventFocus(false);
//     std::cerr << "x " << x << std::endl;
//     std::cerr << "y " << y << std::endl;
//     std::cerr << "width " << width << std::endl;
//     std::cerr << "height " << height << std::endl;
//     camera -> setProjectionMatrix(osg::Matrix::ortho2D(x, x + width, y, y + height));
//     camera -> getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//     camera -> setViewMatrix(osg::Matrix::identity());
//     return camera;
// }


void NetworkViewer::home(unsigned int index)
{
    osgViewer::View* view = _viewer->getView(index);
    osgGA::TrackballManipulator* manipulator = dynamic_cast<osgGA::TrackballManipulator*>(view->getCameraManipulator());
    manipulator->home(0.0);
}

void NetworkViewer::forward(double distance_delta, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    _set_transformation(index, eye + look * distance_delta,
        center + look * distance_delta, up);
}

void NetworkViewer::backward(double distance_delta, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    _set_transformation(index, eye - look * distance_delta,
        center - look * distance_delta, up);
}

void NetworkViewer::left(double distance_delta, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    _set_transformation(index, eye + side * distance_delta,
        center + side * distance_delta, up);
}

void NetworkViewer::right(double distance_delta, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    _set_transformation(index, eye - side * distance_delta,
        center - side * distance_delta, up);
}

void NetworkViewer::up(double distance_delta, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    _set_transformation(index, eye - up * distance_delta,
        center - up * distance_delta, up);
}

void NetworkViewer::down(double distance_delta, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    _set_transformation(index, eye + up * distance_delta,
        center + up * distance_delta, up);
}

void NetworkViewer::zoom(double factor, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    _set_transformation(index, eye + look * factor * distance, center, up);
}

void NetworkViewer::_get_transformation(unsigned int index, osg::Vec3d& eye,
    osg::Vec3d& center, double& distance,
    osg::Vec3d& up, osg::Vec3d& look,
    osg::Vec3d& side)
{
    osgViewer::View* view = _viewer->getView(index);
    osgGA::TrackballManipulator* manipulator = dynamic_cast<osgGA::TrackballManipulator*>(view->getCameraManipulator());
    manipulator->getTransformation(eye, center, up);
    up.normalize();
    look = center - eye;
    distance = look.normalize();
    side = look ^ up;
    side.normalize();
}

void NetworkViewer::_set_transformation(unsigned int index,
    const osg::Vec3d& eye,
    const osg::Vec3d& center,
    const osg::Vec3d& up)
{
    osgViewer::View* view = _viewer->getView(index);
    osgGA::TrackballManipulator* manipulator = dynamic_cast<osgGA::TrackballManipulator*>(view->getCameraManipulator());
    manipulator->setTransformation(eye, center, up);
}

void NetworkViewer::roll(double angle, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    Quat rotation = Quat(-angle, look);
    Vec3f rotated_up = rotation * up;
    rotated_up.normalize();
    _set_transformation(index, eye, center, rotated_up);
}

void NetworkViewer::pitch(double angle, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    Quat rotation = Quat(-angle, side);//osg::X_AXIS);//side);
    Vec3f rotated_up = rotation * up;
    rotated_up.normalize();
    Vec3f rotated_look = rotation * look;
    rotated_look.normalize();
    _set_transformation( index
                       , center - rotated_look * distance
                       , center
                       , rotated_up
                       );
}

void NetworkViewer::yaw(double angle, unsigned int index)
{
    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    osg::Vec3d look;
    osg::Vec3d side;
    double distance;
    _get_transformation(index, eye, center, distance, up, look, side);
    Quat rotation = Quat(-angle, up);
    Vec3f rotated_look = rotation * look;
    rotated_look.normalize();
    _set_transformation(index, center - rotated_look * distance, center, up);
}

void NetworkViewer::keyReleaseEvent(QKeyEvent* event)
{
    QString keyString = event->text();
    const char* keyData = keyString.toLocal8Bit().data();

    this->getEventQueue()->keyRelease(
        osgGA::GUIEventAdapter::KeySymbol(*keyData));
}

void NetworkViewer::mouseMoveEvent(QMouseEvent* event)
{
    this->getEventQueue()->mouseMotion(static_cast<float>(event->x()),
        static_cast<float>(event->y()));
}

void NetworkViewer::mousePressEvent(QMouseEvent* event)
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

void NetworkViewer::mouseReleaseEvent(QMouseEvent* event)
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

void NetworkViewer::wheelEvent(QWheelEvent* event)
{

    event->accept();
    int delta = event->delta();

    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ? osgGA::GUIEventAdapter::SCROLL_UP
                                                               : osgGA::GUIEventAdapter::SCROLL_DOWN;

    this->getEventQueue()->mouseScroll(motion);
}

/*
    unsigned int view_index = _get_view_index_with_focus();
    osgViewer::View * view = _viewer -> getView(view_index);
    osgUtil::LineSegmentIntersector::Intersections intersections;
    float x = (event -> x() / (width() - 0.0f)) * 2.0f - 1.0f;
    float y = (event -> y() / (height() - 0.0f)) * 2.0f - 1.0f;
    RECORD_INFO(to_string(x));
    RECORD_INFO(to_string(y));
    RECORD_INFO(to_string(this->getEventQueue()->getCurrentEventState() ->
   getXnormalized()));
    RECORD_INFO(to_string(this->getEventQueue()->getCurrentEventState() ->
   getYnormalized()));


    osgGA::GUIEventAdapter* eventa = new
    osgGA::GUIEventAdapter(*(this->getEventQueue()->getCurrentEventState()));
    eventa->setEventType(osgGA::GUIEventAdapter::PUSH);
    eventa->setButton(osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);

    // osgUtil::LineSegmentIntersector::Intersections intersections;

    // osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
    //     new osgUtil::LineSegmentIntersector( osgUtil::Intersector::WINDOW
    //                                        ,
   this->getEventQueue()->getCurrentEventState() -> getX()
    //                                        ,
   this->getEventQueue()->getCurrentEventState() -> getY()
    //                                        );
    // osgUtil::IntersectionVisitor iv( intersector.get() );
    // view ->getCamera() -> accept( iv );

    // if ( intersector->containsIntersections() )
    // {
    //     const osgUtil::LineSegmentIntersector::Intersection& result =
    //             *(intersector->getIntersections().begin());

    //         // LOD * lod = dynamic_cast<LOD *>(result.drawable ->
   getParent(0) -> getParent(0));
    //     // RECORD_INFO("Reaching here!");
    //     RECORD_INFO(result.drawable -> asGeometry() -> getName());
    // }

    bool result = view -> computeIntersections ( *eventa
                                               , intersections
                                               );


    // bool result = view -> computeIntersections ( view -> getCamera()
    //                                            ,
   osgUtil::Intersector::CoordinateFrame::PROJECTION
    //                                            ,
   this->getEventQueue()->getCurrentEventState() -> getXnormalized()
    //                                            ,
   this->getEventQueue()->getCurrentEventState() -> getYnormalized()
    //                                            , intersections
    //                                            );

                                 //                view -> getCamera()
                                 // ,
   osgUtil::Intersector::CoordinateFrame::PROJECTION
                                 // , x
                                 // , y
                                 // , intersections
                                 // );
    if(result)
    {
        const osgUtil::LineSegmentIntersector::Intersection& hit =
   *intersections.begin();
        RECORD_INFO(hit.drawable -> asGeometry() -> getName());
    }
    // RECORD_INFO((static_cast<GeometrySelector *>(view ->
   getEventHandlers().front().get()))->id);

*/

bool NetworkViewer::event(QEvent* event)
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
        ) {
        unsigned int view_index = _get_view_index_with_focus();
        osgViewer::View* view = _viewer->getView(view_index);
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

void NetworkViewer::onHome()
{
    osgViewer::ViewerBase::Views views;
    _viewer->getViews(views);

    for (std::size_t i = 0; i < views.size(); i++) {
        osgViewer::View* view = views.at(i);
        view->home();
    }
}

void NetworkViewer::onResize(int width, int height)
{
    // std::vector<osg::Camera*> cameras;
    // _viewer->getCameras( cameras );
    // float width_factor = static_cast<float>(width)
    //                    / static_cast<float>(_previous_width);
    // float height_factor = static_cast<float>(height)
    //                     / static_cast<float>(_previous_height);
    // for(osg::Camera * camera : cameras)
    // {
    //     osg::Viewport * viewport = camera -> getViewport();
    //     cout << "x => " << viewport -> x()      * width_factor << endl;
    //     cout << "y => " << viewport -> y()      * height_factor << endl;
    //     camera -> setViewport( viewport -> x()      * width_factor
    //                          , viewport -> y()      * height_factor
    //                          , viewport -> width()  * width_factor
    //                          , viewport -> height() * height_factor
    //                          );
    //     camera -> setProjectionMatrixAsPerspective( 30.f
    //                                               , (viewport -> width()  *
    //                                               width_factor)
    //                                               / (viewport -> height() *
    //                                               height_factor)
    //                                               , 1.0f
    //                                               , 10000.0f
    //                                               );
    // }
    _previous_height = height;
    _previous_width = width;
}

osgGA::EventQueue* NetworkViewer::getEventQueue() const
{
    osgGA::EventQueue* event_queue = _graphics_window->getEventQueue();

    if (event_queue)
        return (event_queue);
    else
        throw(std::runtime_error("Unable to obtain valid event queue"));
}
