#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>

#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgUtil/SmoothingVisitor>
#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/io_utils>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgText/Text>
#include <sstream>
#include <tuple>
#include <iostream>
#include "mesh/CylinderMesh.hpp"
#include "mesh/Ring.hpp"
#include "utility/record.hpp"
#include "catmull_rom.hpp"
#include "handlers/GeometrySelector.hpp"
#include <cmath>
uint runs = 0;
float
vector_angle(osg::Vec3f a, osg::Vec3f b, osg::Vec3f c)
{
  osg::Vec3f V0(b - a); V0.normalize();
  osg::Vec3f V1(c - b); V1.normalize();
  return acos((V0 * V1));
}

osg::Vec3f convert(osg::Vec4f v)
{
  return osg::Vec3f(v.x(), v.y(), v.z());
}


Ring ring;

osg::Geode *
coordinate_system(const osg::Vec3f & center, float height, const osg::Vec3f & direction, float radius)
{
    CylinderMesh cylinder_mesh;
    osg::Geode * coordinates = new osg::Geode();
    osg::Geometry * px_axis = new osg::Geometry();
    height = height * 4;
    cylinder_mesh( center + osg::Vec3f(1.0f, 0.0f, 0.0f) * height / 4.0f
                 , radius / 16.0f
                 , radius / 16.0f
                 , height / 2.0f
                 , osg::Vec3f(1.0f, 0.0f, 0.0f)
                 , px_axis
                 , 20
                 , osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f)
                 );
    osg::Geometry * nx_axis = new osg::Geometry();
    cylinder_mesh( center + osg::Vec3f(-1.0f, 0.0f, 0.0f) * height / 4.0f
                 , radius / 16.0f
                 , radius / 16.0f
                 , height / 2.0f
                 , osg::Vec3f(-1.0f, 0.0f, 0.0f)
                 , nx_axis
                 , 20
                 , osg::Vec4f(1.0f, 0.0f, 0.0f, 0.5f)
                 );

    osg::Geometry * py_axis = new osg::Geometry();
    cylinder_mesh( center + osg::Vec3f(0.0f, 1.0f, 0.0f) * height / 4.0f
                 , radius / 16.0f
                 , radius / 16.0f
                 , height / 2.0f
                 , osg::Vec3f(0.0f, 1.0f, 0.0f)
                 , py_axis
                 , 20
                 , osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f)
                 );
    osg::Geometry * ny_axis = new osg::Geometry();
    cylinder_mesh( center + osg::Vec3f(0.0f, -1.0f, 0.0f) * height / 4.0f
                 , radius / 16.0f
                 , radius / 16.0f
                 , height / 2.0f
                 , osg::Vec3f(0.0f, -1.0f, 0.0f)
                 , ny_axis
                 , 20
                 , osg::Vec4f(0.0f, 1.0f, 0.0f, 0.5f)
                 );

    osg::Geometry * pz_axis = new osg::Geometry();
    cylinder_mesh( center + osg::Vec3f(0.0f, 0.0f, 1.0f) * height / 4.0f
                 , radius / 16.0f
                 , radius / 16.0f
                 , height / 2.0f
                 , osg::Vec3f(0.0f, 0.0f, 1.0f)
                 , pz_axis
                 , 20
                 , osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f)
                 );
    osg::Geometry * nz_axis = new osg::Geometry();
    cylinder_mesh( center + osg::Vec3f(0.0f, 0.0f, -1.0f) * height / 4.0f
                 , radius / 16.0f
                 , radius / 16.0f
                 , height / 2.0f
                 , osg::Vec3f(0.0f, 0.0f, -1.0f)
                 , nz_axis
                 , 20
                 , osg::Vec4f(0.0f, 0.0f, 1.0f, 0.5f)
                 );

    osg::Geometry * pn_axis = new osg::Geometry();
    cylinder_mesh( center + direction * height / 4.0f
                 , radius / 16.0f
                 , radius / 16.0f
                 , height / 2.0f
                 , direction
                 , pn_axis
                 , 20
                 , osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f)
                 );
    osg::Geometry * nn_axis = new osg::Geometry();
    cylinder_mesh( center -direction * height / 4.0f
                 , radius / 16.0f
                 , radius / 16.0f
                 , height / 2.0f
                 , -direction
                 , nn_axis
                 , 20
                 , osg::Vec4f(1.0f, 1.0f, 0.0f, 0.5f)
                 );

    coordinates -> addDrawable(px_axis);
    coordinates -> addDrawable(py_axis);
    coordinates -> addDrawable(pz_axis);
    coordinates -> addDrawable(pn_axis);
    coordinates -> addDrawable(nx_axis);
    coordinates -> addDrawable(ny_axis);
    coordinates -> addDrawable(nz_axis);
    coordinates -> addDrawable(nn_axis);

    return coordinates;
}


osg::Geode *
mark(osg::Geometry * G, float radius, uint axial_segments, uint radial_segments)
{
    osg::Vec3Array & V = *(static_cast<osg::Vec3Array *>(G -> getVertexArray()));
    osg::Geode * markers = new osg::Geode();
    osg::ShapeDrawable * marker;
    marker = new osg::ShapeDrawable(new osg::Sphere(V[(axial_segments * radial_segments) / 2], radius / 8.0f)); //radius / 4.0f));//0.1f));
    marker -> setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    markers -> addDrawable(marker);
    marker = new osg::ShapeDrawable(new osg::Sphere(V[(axial_segments * radial_segments) / 2 + radial_segments / 4], radius / 8.0f)); //, radius / 4.0f));
    marker -> setColor(osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    markers -> addDrawable(marker);
    marker = new osg::ShapeDrawable(new osg::Sphere(V[(axial_segments * radial_segments) / 2 + radial_segments / 2], radius / 8.0f)); //, radius / 4.0f));
    marker -> setColor(osg::Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    markers -> addDrawable(marker);
    marker = new osg::ShapeDrawable(new osg::Sphere(V[(axial_segments * radial_segments) / 2 + radial_segments * 3 / 4],radius / 8.0f)); //, radius / 4.0f));
    marker -> setColor(osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
    markers -> addDrawable(marker);
    return markers;
}


osg::Geometry *
create_geometry_ring( osg::Vec3f proximal
                    , osg::Vec3f distal
                    , float      r
                    , size_t axial_segments
                    , size_t radial_segments
                    , uint   children           = 1
                    , osg::Vec4f color          = osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f)
                    )
{
    osg::Geometry * G = new osg::Geometry();
    const size_t vertex_count           = axial_segments * radial_segments
                                        + (children - 1)
                                        * radial_segments
                                        * ( (axial_segments - 1) / 2 );
    const size_t triangle_count         = 2 * (vertex_count - radial_segments);
    const uint   normal_count           = vertex_count;
    osg::ref_ptr<osg::Vec3Array>    V   = new osg::Vec3Array(vertex_count);
    osg::ref_ptr<osg::Vec3Array>    N   = new osg::Vec3Array(normal_count);
    osg::ref_ptr<osg::DrawElementsUShort> I =
            new osg::DrawElementsUShort(GL_TRIANGLES, triangle_count * 3);
    osg::ref_ptr<osg::Vec4Array> colors           = new osg::Vec4Array();
    colors -> push_back(color);
    for(uint i = 1; i < vertex_count; ++i)
    {
        colors -> push_back(osg::Vec4f(0.0f, 0.5f, 0.5f, 1.0f));
    }
    osg::Vec3f C = (distal + proximal) / 2.0f;
    osg::Vec3f D = (distal - proximal);
    D.normalize();
    ring(C, D, r, radial_segments, V, axial_segments * radial_segments / 2 );
    // G -> setVertexArray(V);
    G -> setVertexArray(V.get());
    G -> addPrimitiveSet(I.get());
    G -> addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::POINTS
                                             , 0
                                             , V -> size()
                                             )
                        );
    G -> setNormalArray(N.get(), osg::Array::BIND_PER_VERTEX);
    G -> setColorArray(colors);
    G -> setColorBinding( osg::Geometry::BIND_PER_VERTEX);
    return G;
}


void
catmull_rom_segment( osg::Vec3Array * vertex
                   , std::vector<float> & radii
                   , size_t axial_segments
                   , size_t radial_segments
                   )
{


    // Ring ring;
    // osg::Geode * geode = new osg::Geode();
    // for(i = 0; i < vertex -> size() - 2; ++i)
    // {
    //     osg::Vec3f P0 = vertex[i    ];
    //     osg::Vec3f P1 = vertex[i + 1];
    //     osg::Vec3f P2 = vertex[i + 2];
    //     osg::Vec3f P3 = vertex[i + 3];
    //     osg::Vec3f P4 = vertex[i + 4];

    //     ring.ring(
    //              );

    // }
}



int
old_main(int argc, char * argv[])
{
    osg::Group * group = new osg::Group();
    osg::Geode * geode = new osg::Geode();
    osg::Geometry * G0, *G1, *G2, *G3, *G4, *G5, *G6;
    uint radial_segments = 20;
    uint axial_segments  = 30;

    // G0 = create_geometry_ring( osg::Vec3f(161.6, -75.57, -9.635)
    //                          , osg::Vec3f(161.7, -76.36, -9.625)
    //                          , 8.869
    //                          , 10
    //                          , 10
    //                          );
    // geode -> addDrawable(G0);

    // G1 = create_geometry_ring( osg::Vec3f(161.7, -76.36, -9.625)
    //                          , osg::Vec3f(161.8, -77.14, -9.614)
    //                          , 8.66
    //                          , 10
    //                          , 10
    //                          );
    // geode -> addDrawable(G1);

    // G2 = create_geometry_ring( osg::Vec3f(161.8, -77.14, -9.614)
    //                          , osg::Vec3f(162, -77.93, -9.604)
    //                          , 8.349
    //                          , 10
    //                          , 10
    //                          );
    // geode -> addDrawable(G2);

    // G3 = create_geometry_ring( osg::Vec3f(162, -77.93, -9.604)
    //                          , osg::Vec3f(162.1, -78.72, -9.594)
    //                          , 7.941
    //                          , 10
    //                          , 10
    //                          );
    // geode -> addDrawable(G3);

    // G4 = create_geometry_ring( osg::Vec3f(162.1, -78.72, -9.594)
    //                          , osg::Vec3f(162.3, -79.5 ,-9.584)
    //                          , 7.439
    //                          , 10
    //                          , 10
    //                          );
    // geode -> addDrawable(G4);

    G0 = create_geometry_ring( osg::Vec3f(-10.0f, 0.0f, 0.0f)
                             , osg::Vec3f(-5.0f, 0.0f, 0.0f)
                             , 5.0f
                             , axial_segments
                             , radial_segments
                             );
    // geode -> addDrawable(G0);

    G1 = create_geometry_ring( osg::Vec3f(-5.0f, 0.0f, 0.0f)
                             , osg::Vec3f(-2.5f, 0.0f, 0.0f)
                             , 2.5f
                             , axial_segments
                             , radial_segments
                             );
    // geode -> addDrawable(G1);

    G2 = create_geometry_ring( osg::Vec3f(-2.5f, 0.0f, 0.0f)
                             , osg::Vec3f(0.0f, 0.0f, 0.0f)
                             , 5.0f
                             , axial_segments
                             , radial_segments
                             );
    geode -> addDrawable(G2);

    G3 = create_geometry_ring( osg::Vec3f(0.0f, 0.0f, 0.0f)
                             , osg::Vec3f(2.5f, 0.0f, 0.0f)
                             , 2.5f
                             , axial_segments
                             , radial_segments
                             );
    geode -> addDrawable(G3);

    G4 = create_geometry_ring( osg::Vec3f(2.5f, 0.0f, 0.0f)
                             , osg::Vec3f(10.0f, 0.0f, 0.0f)
                             , 5.0f
                             , axial_segments
                             , radial_segments
                             );
    geode -> addDrawable(G4);

    G5 = create_geometry_ring( osg::Vec3f(10.0f, 0.0f, 0.0f)
                             , osg::Vec3f(20.0f, 0.0f, 0.0f)
                             , 10.0f
                             , axial_segments
                             , radial_segments
                             );

    G6 = create_geometry_ring( osg::Vec3f(20.0f, 0.0f, 0.0f)
                             , osg::Vec3f(25.0f, 0.0f, 0.0f)
                             , 15.0f
                             , axial_segments
                             , radial_segments
                             );

    // geode -> addDrawable(G5);

    std::cerr << "BEGIN::catmull_rom_bounded_compartment";
    catmull_rom_bounded_compartment( G0
                                   , G1
                                   , G2
                                   , G3
                                   , G4
                                   , 0.25f
                                   , axial_segments
                                   , radial_segments
                                   );
    std::cerr << "DONE::catmull_rom_bounded_compartment";


    std::cerr << "BEGIN::catmull_rom_bounded_compartment";
    catmull_rom_bounded_compartment( G1
                                   , G2
                                   , G3
                                   , G4
                                   , G5
                                   , 0.25f
                                   , axial_segments
                                   , radial_segments
                                   );
    std::cerr << "DONE::catmull_rom_bounded_compartment";

    std::cerr << "BEGIN::catmull_rom_bounded_compartment";
    catmull_rom_bounded_compartment( G2
                                   , G3
                                   , G4
                                   , G5
                                   , G6
                                   , 0.25f
                                   , axial_segments
                                   , radial_segments
                                   );
    std::cerr << "DONE::catmull_rom_bounded_compartment";

    group -> addChild(geode);
    // std::cerr << "Export Result : "
    //           << osgDB::writeNodeFile(*group, "./continous_segment.svg");
    // std::cerr << "Export Result : "
    //           << osgDB::writeNodeFile(*group, "./continous_segment.3ds");
    osgViewer::Viewer viewer;
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    viewer.addEventHandler(new GeometrySelector());

    viewer.setCameraManipulator( new osgGA::TrackballManipulator() );
    // viewer.setEventHandler(new osgViewer::ScreenCaptureHandler());
    viewer.getCamera() -> setClearColor(osg::Vec4f(0.0f, 0.0f, 0.0f, 0.0f));
    // viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);
    viewer.setSceneData(group);
    viewer.realize();
    viewer.run();
    return 0;
}

osg::Group *
catmull_rom_segment()
{
    osg::Group * group = new osg::Group();
    osg::Geode * geode = new osg::Geode();
    osg::Geometry * G0, * G1, * G2, * G3, * G4;

    size_t axial_segments  = 50;
    size_t radial_segments = 50;

    osg::ref_ptr<osg::Vec4Array> points(new osg::Vec4Array());
    points -> push_back(osg::Vec4f(161.6, -75.57, -9.635, 8.975));
    points -> push_back(osg::Vec4f(161.7, -76.36, -9.625, 8.869));
    points -> push_back(osg::Vec4f(161.8, -77.14, -9.614, 8.66));
    points -> push_back(osg::Vec4f(162, -77.93, -9.604, 8.349));
    points -> push_back(osg::Vec4f(162.1, -78.72, -9.594, 7.941));
    points -> push_back(osg::Vec4f(162.3, -79.5 ,-9.584 ,7.439));
    points -> push_back(osg::Vec4f(162.4, -80.29, -9.573, 6.85));
    points -> push_back(osg::Vec4f(162.5, -81.08, -9.563, 6.181));
    points -> push_back(osg::Vec4f(162.7, -81.86, -9.553, 5.439));
    points -> push_back(osg::Vec4f(162.8, -82.65, -9.543, 4.634));
    points -> push_back(osg::Vec4f(162.9, -83.44, -9.533, 3.774));
    points -> push_back(osg::Vec4f(163.1, -84.22, -9.522, 3.365));
    points -> push_back(osg::Vec4f(163.2, -85.01, -9.512, 3.365));
    points -> push_back(osg::Vec4f(163.3, -85.8 ,-9.502 ,3.365));
    points -> push_back(osg::Vec4f(163.5, -86.58, -9.492, 3.365));
    points -> push_back(osg::Vec4f(163.4, -86.0, -9.499, 1.24));
    // 15 - 14 = -0.1, 0.58, -0.007
    //points -> push_back(osg::Vec4f(164.2, -87.9, -9.499, 0.59));
    // 16 - 15 = 0.8, -1.9, 0.0
    // points -> push_back(osg::Vec4f(164.5, -89.03, -9.499, 0.415));
    // points -> push_back(osg::Vec4f(164.6, -90.4, -9.499, 0.415));
    // points -> push_back(osg::Vec4f(164.8, -91.61, -9.499, 0.415));
    /*
    points -> push_back(osg::Vec4f(164.7, -93.04, -9.499, 0.415));
    points -> push_back(osg::Vec4f(164.6, -94.71, -9.929, 0.415));
    points -> push_back(osg::Vec4f(164.7, -96.33, -9.929, 0.415));
    points -> push_back(osg::Vec4f(164.0, -98.01, -10.29, 0.415));
    points -> push_back(osg::Vec4f(163.7, -100.3, -10.57, 0.415));
    points -> push_back(osg::Vec4f(163.4, -102.1, -10.57, 0.415));
    points -> push_back(osg::Vec4f(162.9, -104.4, -10.54, 0.415));
    points -> push_back(osg::Vec4f(163.0, -106.4, -10.57, 0.415));
    points -> push_back(osg::Vec4f(163.6, -108.2, -10.85, 0.355));
    points -> push_back(osg::Vec4f(164.4, -109.7, -10.85, 0.355));
    points -> push_back(osg::Vec4f(165.6, -111.0, -10.85, 0.355));
    points -> push_back(osg::Vec4f(166.6, -112.6, -10.85, 0.355));
    points -> push_back(osg::Vec4f(167.4, -113.9, -10.97, 0.235));
    points -> push_back(osg::Vec4f(167.8, -114.8, -10.97, 0.235));
    points -> push_back(osg::Vec4f(168.0, -116.4, -11.49, 0.235));
    points -> push_back(osg::Vec4f(167.9, -117.7, -11.61, 0.235));
    points -> push_back(osg::Vec4f(167.8, -118.5, -11.61, 0.235));
    points -> push_back(osg::Vec4f(167.5, -119.5, -11.65, 0.235));
    points -> push_back(osg::Vec4f(167.5, -120.4, -11.65, 0.235));
    points -> push_back(osg::Vec4f(167.6, -121.5, -11.65, 0.235));
    points -> push_back(osg::Vec4f(167.8, -123.1, -11.61, 0.235));
    points -> push_back(osg::Vec4f(167.4, -124.5, -11.61, 0.235));
    points -> push_back(osg::Vec4f(167.1, -125.4, -11.61, 0.235));
    points -> push_back(osg::Vec4f(166.7, -126.4, -11.61, 0.235));
    points -> push_back(osg::Vec4f(166.4, -127.2, -11.89, 0.175));
    points -> push_back(osg::Vec4f(167.2, -127.6, -11.89, 0.175));
    points -> push_back(osg::Vec4f(168.1, -127.8, -11.89, 0.175));
    points -> push_back(osg::Vec4f(168.7, -128.5, -12.26, 0.175));
    points -> push_back(osg::Vec4f(168.9, -129.7, -12.26, 0.175));
    points -> push_back(osg::Vec4f(168.8, -129.8, -12.26, 0.175));
    points -> push_back(osg::Vec4f(168.5, -130.9, -12.29, 0.175));
    points -> push_back(osg::Vec4f(168.4, -132.2, -13.03, 0.175));
    points -> push_back(osg::Vec4f(169.3, -137.9, -13.09, 0.175));
    points -> push_back(osg::Vec4f(169.6, -138.8, -13.12, 0.175));
    points -> push_back(osg::Vec4f(170.2, -140.6, -13.12, 0.175));
    points -> push_back(osg::Vec4f(170.2, -140.7, -13.12, 0.175));
    points -> push_back(osg::Vec4f(170.4, -142.1, -13.12, 0.175));
    points -> push_back(osg::Vec4f(170.3, -143.2, -13.12, 0.175));
    points -> push_back(osg::Vec4f(170.3, -143.3, -13.12, 0.175));
    points -> push_back(osg::Vec4f(169.6, -144.4, -13.12, 0.175));
    points -> push_back(osg::Vec4f(169.4, -145.3, -13.12, 0.175));
    points -> push_back(osg::Vec4f(170.7, -150.6, -12.78, 0.175));
    points -> push_back(osg::Vec4f(171.0, -152.0, -12.78, 0.175));
    points -> push_back(osg::Vec4f(170.9, -152.1, -12.78, 0.175));
    points -> push_back(osg::Vec4f(171.4, -152.8, -12.78, 0.175));
    points -> push_back(osg::Vec4f(171.5, -152.7, -12.78, 0.175));
    points -> push_back(osg::Vec4f(172.3, -153.2, -12.78, 0.175));
    points -> push_back(osg::Vec4f(172.9, -154.2, -12.78, 0.175));
    points -> push_back(osg::Vec4f(172.8, -154.3, -12.78, 0.175));
    points -> push_back(osg::Vec4f(172.9, -155.7, -12.97, 0.175));
    points -> push_back(osg::Vec4f(173.1, -157.2, -12.97, 0.175));
    points -> push_back(osg::Vec4f(173.2, -157.3, -12.97, 0.175));
    points -> push_back(osg::Vec4f(173.1, -158.2, -12.97, 0.175));
    points -> push_back(osg::Vec4f(173.2, -159.7, -12.97, 0.175));
    points -> push_back(osg::Vec4f(173.2, -159.8, -12.97, 0.175));
    points -> push_back(osg::Vec4f(173.3, -161.1, -12.97, 0.175));
    points -> push_back(osg::Vec4f(174.1, -162.1, -12.97, 0.175));
    points -> push_back(osg::Vec4f(174.3, -163.5, -12.97, 0.175));
    points -> push_back(osg::Vec4f(172.6, -164.5, -13.67, 0.175));
    points -> push_back(osg::Vec4f(171.9, -166.0, -13.67, 0.175));
    points -> push_back(osg::Vec4f(172.0, -166.3, -13.67, 0.175));
    points -> push_back(osg::Vec4f(172.3, -167.1, -13.67, 0.175));
    points -> push_back(osg::Vec4f(171.9, -168.0, -13.67, 0.175));
    points -> push_back(osg::Vec4f(172.1, -169.2, -13.83, 0.175));
    points -> push_back(osg::Vec4f(172.1, -169.9, -13.83, 0.175));
    points -> push_back(osg::Vec4f(172.0, -170.1, -13.83, 0.175));
    points -> push_back(osg::Vec4f(171.8, -170.8, -13.83, 0.175));
    points -> push_back(osg::Vec4f(172.4, -171.9, -13.83, 0.175));
    points -> push_back(osg::Vec4f(172.5, -171.9, -13.86, 0.175));
    points -> push_back(osg::Vec4f(173.2, -173.0, -13.86, 0.175));
    points -> push_back(osg::Vec4f(173.6, -174.0, -13.89, 0.175));
    points -> push_back(osg::Vec4f(173.6, -174.3, -13.89, 0.175));
    points -> push_back(osg::Vec4f(173.6, -175.3, -13.89, 0.175));
    points -> push_back(osg::Vec4f(173.7, -175.4, -13.89, 0.175));

    // points -> clear();
    points -> push_back(osg::Vec4f(174.1, -176.1, -13.89, 0.175));
    points -> push_back(osg::Vec4f(174.3, -176.2, -13.89, 0.175));
    points -> push_back(osg::Vec4f(174.7, -177, -13.89, 0.175));
    points -> push_back(osg::Vec4f(174.9, -177, -13.89, 0.175));
    points -> push_back(osg::Vec4f(176.0, -177.5, -13.4, 0.175));
    points -> push_back(osg::Vec4f( 176.7, -178, -13.4, 0.175));
    points -> push_back(osg::Vec4f( 176.7, -178.3, -13.4, 0.175));
    points -> push_back(osg::Vec4f( 177.7, -178.8, -13.4, 0.175));
    points -> push_back(osg::Vec4f( 179.2, -179.2, -12.97, 0.175));
    points -> push_back(osg::Vec4f( 179.9, -180.1, -12.97, 0.175));
    points -> push_back(osg::Vec4f( 180.8, -180.3, -12.78, 0.175));
    points -> push_back(osg::Vec4f( 181.7, -180.1, -12.78, 0.175));
    points -> push_back(osg::Vec4f( 183, -179.9, -12.84, 0.175));
    points -> push_back(osg::Vec4f( 184.2, -180.7, -12.84, 0.175));
    points -> push_back(osg::Vec4f( 185, -181.6, -12.84, 0.175));
    points -> push_back(osg::Vec4f( 185.8, -182.3, -12.26, 0.175));
    points -> push_back(osg::Vec4f( 186.4, -183.3, -12.26, 0.175));
    points -> push_back(osg::Vec4f( 187.3, -184.1, -12.26, 0.175));
    points -> push_back(osg::Vec4f( 187.5, -185.2, -11.95, 0.175));
    points -> push_back(osg::Vec4f( 187.5, -185.9, -11.95, 0.175));
    points -> push_back(osg::Vec4f( 187.6, -186.4, -11.95, 0.175));
    points -> push_back(osg::Vec4f( 187.6, -186.6, -11.95, 0.175));
    points -> push_back(osg::Vec4f( 187.1, -186.7, -11.12, 0.175));
    points -> push_back(osg::Vec4f( 187.4, -187.3, -10.88, 0.175));
    points -> push_back(osg::Vec4f( 188.1, -187.8, -10.79, 0.175));
    points -> push_back(osg::Vec4f( 188.7, -188.8, -10.79, 0.175));
    points -> push_back(osg::Vec4f( 188.7, -188.9, -10.79, 0.175));
    points -> push_back(osg::Vec4f( 189.5, -188.9, -10.79, 0.175));
    points -> push_back(osg::Vec4f( 189.6, -188.8, -10.79, 0.175));
    points -> push_back(osg::Vec4f( 190.5, -189.2, -10.79, 0.175));
    points -> push_back(osg::Vec4f( 190.9, -190.0, -10.48, 0.175));
    points -> push_back(osg::Vec4f( 191.4, -191.0, -10.08, 0.175));
    points -> push_back(osg::Vec4f( 191.7, -192.1,-10.08, 0.175));
    points -> push_back(osg::Vec4f( 191.9, -192.5,-9.829, 0.175));
    points -> push_back(osg::Vec4f( 192.6, -192.9,-9.829, 0.175));
    points -> push_back(osg::Vec4f( 193.7, -193.4,-8.879, 0.175));
    points -> push_back(osg::Vec4f( 194.5, -194.3,-8.179, 0.175));
    points -> push_back(osg::Vec4f( 195.3, -194.6,-8.179, 0.175));
    points -> push_back(osg::Vec4f( 195.9, -195.2,-8.079, 0.175));
    points -> push_back(osg::Vec4f( 196.5, -195.9,-8.079, 0.175));
    points -> push_back(osg::Vec4f( 196.6, -196.0, -8.079, 0.175));
    points -> push_back(osg::Vec4f( 197.4, -196.0, -8.079, 0.175));
    points -> push_back(osg::Vec4f( 197.2, -197.0, -8.079, 0.175));
    points -> push_back(osg::Vec4f( 197.8, -197.4, -7.779, 0.175));
    points -> push_back(osg::Vec4f( 198.3, -197.5, -7.779, 0.175));
    points -> push_back(osg::Vec4f( 199.3, -197.8, -8.209, 0.175));
    points -> push_back(osg::Vec4f( 200.1, -198.6, -8.209, 0.175));
    points -> push_back(osg::Vec4f( 200.2, -198.8, -8.209, 0.175));
    points -> push_back(osg::Vec4f( 201.0, -199.8, -8.209, 0.175));
    points -> push_back(osg::Vec4f( 201.2, -200.1, -8.209, 0.175));
    points -> push_back(osg::Vec4f( 202.1, -201.2, -8.209, 0.175));
    points -> push_back(osg::Vec4f( 202.2, -201.4, -8.209, 0.175));
    points -> push_back(osg::Vec4f( 203.0, -202.3, -8.209, 0.175));
    points -> push_back(osg::Vec4f( 204.0, -203.0, -6.609, 0.175));
    points -> push_back(osg::Vec4f( 205.0, -203.8, -6.609, 0.175));
    points -> push_back(osg::Vec4f( 207.0, -203.8, -5.969, 0.175));
    points -> push_back(osg::Vec4f( 207.1, -203.7, -5.969, 0.175));
    points -> push_back(osg::Vec4f( 208.1, -203.9, -5.969, 0.175));
    points -> push_back(osg::Vec4f( 209.2, -203.8, -5.969, 0.175));
    points -> push_back(osg::Vec4f( 210.7, -203.6, -5.969, 0.175));
    points -> push_back(osg::Vec4f( 211.3, -203.6, -5.969, 0.175));
    points -> push_back(osg::Vec4f( 211.4, -203.7, -5.969, 0.175));
    points -> push_back(osg::Vec4f( 212.2, -204.3, -5.969, 0.175));
    points -> push_back(osg::Vec4f( 212.4, -204.3, -5.969, 0.175));
    points -> push_back(osg::Vec4f( 214.0, -204.1, -5.349, 0.175));
    points -> push_back(osg::Vec4f( 214.4, -204.2, -5.349, 0.175));
    points -> push_back(osg::Vec4f( 214.7, -205.1, -5.349, 0.175));
    points -> push_back(osg::Vec4f( 214.9, -206.0, -4.799, 0.175));
    points -> push_back(osg::Vec4f( 215.4, -206.8, -4.799, 0.175));
    points -> push_back(osg::Vec4f( 215.5, -206.8, -4.799, 0.175));
    points -> push_back(osg::Vec4f( 216.2, -207.6, -4.799, 0.175));
    points -> push_back(osg::Vec4f( 216.4, -207.7, -4.799, 0.175));
    points -> push_back(osg::Vec4f( 217.2, -207.9, -4.799, 0.175));
    points -> push_back(osg::Vec4f( 218.1, -208.5, -4.769, 0.175));
    points -> push_back(osg::Vec4f( 218.9, -208.8, -4.769, 0.175));
    points -> push_back(osg::Vec4f( 220.0, -208.9, -4.769, 0.175));
    points -> push_back(osg::Vec4f( 221.2, -208.4, -4.369, 0.175));
    points -> push_back(osg::Vec4f( 222.2, -208.1, -4.369, 0.175));
    points -> push_back(osg::Vec4f( 222.8, -207.7, -4.369, 0.175));
    points -> push_back(osg::Vec4f( 222.9, -207.7, -4.369, 0.175));
    points -> push_back(osg::Vec4f( 223.8, -207.4, -4.369, 0.175));
    points -> push_back(osg::Vec4f( 223.9, -207.4, -4.369, 0.175));
    points -> push_back(osg::Vec4f( 224.8, -207.3, -4.369, 0.175));
    points -> push_back(osg::Vec4f( 225.8, -206.8, -4.579, 0.175));
    points -> push_back(osg::Vec4f( 227.5, -206.3, -4.709, 0.175));
    points -> push_back(osg::Vec4f( 228.4, -206.2, -4.949, 0.175));
    points -> push_back(osg::Vec4f( 229.6, -206.8, -4.949, 0.175));
    points -> push_back(osg::Vec4f( 231.1, -207.5, -6.059, 0.175));
    points -> push_back(osg::Vec4f( 232.1, -208.2, -6.059, 0.175));
    points -> push_back(osg::Vec4f( 232.6, -208.9, -6.429, 0.175));
    points -> push_back(osg::Vec4f( 232.6, -209.0, -6.429, 0.175));
    points -> push_back(osg::Vec4f( 233.4, -209.2, -6.429, 0.175));
    points -> push_back(osg::Vec4f( 233.5, -209.3, -6.429, 0.175));
    points -> push_back(osg::Vec4f( 234.1, -209.5, -6.429, 0.175));
    points -> push_back(osg::Vec4f( 234.6, -210.0, -6.429, 0.175));
    points -> push_back(osg::Vec4f( 235.5, -210.5, -6.669, 0.175));
    points -> push_back(osg::Vec4f( 235.9, -210.8, -6.669, 0.175));
    points -> push_back(osg::Vec4f( 236.5, -210.4, -6.669, 0.175));
    points -> push_back(osg::Vec4f( 237.2, -210.3, -6.669, 0.175));
    points -> push_back(osg::Vec4f( 238.4, -209.9, -6.789, 0.175));
    points -> push_back(osg::Vec4f( 239.6, -210.3, -6.789, 0.175));
    points -> push_back(osg::Vec4f( 240.5, -210.9, -6.789, 0.175));
    points -> push_back(osg::Vec4f( 241.4, -211.9, -6.789, 0.175));
    points -> push_back(osg::Vec4f( 242.2, -212.9, -7.129, 0.175));
    points -> push_back(osg::Vec4f( 243.2, -213.2, -7.129, 0.175));
    points -> push_back(osg::Vec4f( 243.4, -213.4, -7.129, 0.175));
    points -> push_back(osg::Vec4f( 244.2, -213.6, -7.129, 0.175));
    points -> push_back(osg::Vec4f( 245.0, -214.2, -7.749, 0.175));
    points -> push_back(osg::Vec4f( 245.8, -214.8, -7.749, 0.175));
    points -> push_back(osg::Vec4f( 246.4, -215.7, -8.019, 0.175));
    points -> push_back(osg::Vec4f( 247.1, -216.3, -8.019, 0.175));
    points -> push_back(osg::Vec4f( 247.2, -216.4, -8.019, 0.175));
    points -> push_back(osg::Vec4f( 247.8, -217.0, -8.329, 0.175));
    points -> push_back(osg::Vec4f( 248.2, -217.8, -8.329, 0.175));
    points -> push_back(osg::Vec4f( 248.7, -218.8, -8.329, 0.175));
    points -> push_back(osg::Vec4f( 248.8, -218.9, -8.329, 0.175));
    points -> push_back(osg::Vec4f( 249.3, -220.0, -8.329, 0.175));
    points -> push_back(osg::Vec4f( 249.4, -220.1, -8.329, 0.175));
    points -> push_back(osg::Vec4f( 249.8, -220.7, -8.329, 0.175));
    points -> push_back(osg::Vec4f( 249.9, -220.9, -8.329, 0.175));
    points -> push_back(osg::Vec4f( 250.6, -222.0, -8.579, 0.175));
    points -> push_back(osg::Vec4f( 251.2, -223.2, -8.789, 0.175));
    points -> push_back(osg::Vec4f( 251.5, -223.3, -8.789, 0.175));
    points -> push_back(osg::Vec4f( 251.9, -223.7, -8.789, 0.175));
    points -> push_back(osg::Vec4f( 252.6, -223.9, -8.789, 0.175));
    points -> push_back(osg::Vec4f( 252.7, -223.8, -8.789, 0.175));
    points -> push_back(osg::Vec4f( 253.2, -224.1, -9.009, 0.175));
    points -> push_back(osg::Vec4f( 254.2, -224.6, -9.009, 0.175));
    points -> push_back(osg::Vec4f( 254.9, -225.5, -9.009, 0.175));
    points -> push_back(osg::Vec4f( 255.5, -226.1, -9.279, 0.175));
    points -> push_back(osg::Vec4f( 256.1, -226.9, -9.279, 0.175));
    points -> push_back(osg::Vec4f( 256.7, -227.5, -9.279, 0.175));
    points -> push_back(osg::Vec4f( 256.9, -227.7, -9.279, 0.175));
    points -> push_back(osg::Vec4f( 257.3, -228.3, -9.099, 0.175));
    points -> push_back(osg::Vec4f( 257.4, -228.4, -9.099, 0.175));
    points -> push_back(osg::Vec4f( 257.2, -229.1, -9.099, 0.175));
    points -> push_back(osg::Vec4f( 257.3, -230.2, -9.099, 0.175));
    points -> push_back(osg::Vec4f( 257.4, -230.4, -9.099, 0.175));
    points -> push_back(osg::Vec4f( 257.9, -230.9, -9.099, 0.175));
    points -> push_back(osg::Vec4f( 258.3, -230.9, -9.099, 0.175));
    points -> push_back(osg::Vec4f( 259.0, -231.3, -9.099, 0.175));
    points -> push_back(osg::Vec4f( 259.1, -231.3, -9.099, 0.175));
    points -> push_back(osg::Vec4f( 260.6, -231.6, -9.799, 0.175));
    points -> push_back(osg::Vec4f( 261.8, -232.0, -9.799, 0.175));
    points -> push_back(osg::Vec4f( 261.9, -232.0, -9.799, 0.175));
    points -> push_back(osg::Vec4f( 263.0, -232.4, -9.799, 0.175));
    points -> push_back(osg::Vec4f( 267.5, -234.6, -9.799, 0.175));
    points -> push_back(osg::Vec4f( 268.3, -234.9, -9.799, 0.175));
    points -> push_back(osg::Vec4f( 269.9, -234.8, -9.799, 0.175));
    points -> push_back(osg::Vec4f( 270.9, -235.1, -9.799, 0.175));
    */
    // points -> push_back(osg::Vec4f( 272.0, -235.2, -10.51, 0.175));
    // points -> push_back(osg::Vec4f( 273.4, -236.4, -10.66, 0.175));
    // points -> push_back(osg::Vec4f( 274.5, -237.1, -10.66, 0.175));
    // points -> push_back(osg::Vec4f( 275.3, -237.6, -11.34, 0.175));
    // points -> push_back(osg::Vec4f( 276.2, -237.7, -11.34, 0.175));

    // points -> push_back(osg::Vec4f( 276.5, -238.0, -11.34, 0.175));
    // points -> push_back(osg::Vec4f( 277.2, -239.0, -11.34, 0.175));
    // points -> push_back(osg::Vec4f( 277.6, -239.5, -11.34, 0.175));
    // points -> push_back(osg::Vec4f(163.3, -85.71, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(162.3, -87.51, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(162.2, -87.58, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(160.4, -89.52, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(160.3, -89.67, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(158.9, -90.24, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(157.8, -90.60, -8.303, 0.415));
    // points -> push_back(osg::Vec4f(156.8, -90.36, -8.303, 0.415));
    // points -> push_back(osg::Vec4f(156.6, -90.34, -8.303, 0.415));
    // points -> push_back(osg::Vec4f(155.5, -89.84, -8.303, 0.415));
    // points -> push_back(osg::Vec4f(155.3, -89.73, -8.303, 0.415));
    // points -> push_back(osg::Vec4f(154.1, -88.80, -8.303, 0.415));
    // points -> push_back(osg::Vec4f(154.1, -88.70, -8.303, 0.415));
    // points -> push_back(osg::Vec4f(153.5, -87.62, -8.303, 0.415));
    // points -> push_back(osg::Vec4f(152.0, -87.41, -8.303, 0.415));
    // points -> push_back(osg::Vec4f(152.0, -87.91, -7.563, 0.295));
    // points -> push_back(osg::Vec4f(151.8, -88.05, -7.563, 0.295));
    // points -> push_back(osg::Vec4f(150.6, -88.81, -7.563, 0.295));
    // points -> push_back(osg::Vec4f(150.3, -88.86, -7.563, 0.295));
    // points -> push_back(osg::Vec4f(148.8, -89.66, -7.563, 0.295));
    // points -> push_back(osg::Vec4f(148.6, -89.71, -7.563, 0.295));
    // points -> push_back(osg::Vec4f(147.9, -89.36, -7.563, 0.295));
    // points -> push_back(osg::Vec4f(147.4, -88.95, -7.593, 0.295));
    // points -> push_back(osg::Vec4f(146.8, -88.52, -7.593, 0.295));
    // points -> push_back(osg::Vec4f(146.4, -88.81, -7.593, 0.295));
    // points -> push_back(osg::Vec4f(145.8, -89.40, -7.593, 0.295));
    // points -> push_back(osg::Vec4f(145.4, -89.43, -7.593, 0.295));
    // points -> push_back(osg::Vec4f(144.6, -89.31, -7.563, 0.295));
    // points -> push_back(osg::Vec4f(144.4, -89.29, -7.593, 0.295));
    // points -> push_back(osg::Vec4f(144.0, -89.48, -7.193, 0.295));
    // points -> push_back(osg::Vec4f(143.9, -89.55, -7.193, 0.295));
    // points -> push_back(osg::Vec4f(142.3, -90.27, -7.163, 0.295));
    // points -> push_back(osg::Vec4f(141.4, -89.62, -7.163, 0.295));
    // points -> push_back(osg::Vec4f(141.3, -89.43, -7.163 ,0.295));
    // points -> push_back(osg::Vec4f(140.7, -89.27, -7.223, 0.295));

    // points -> clear();
    // points -> push_back(osg::Vec4f(0.0f,0.0f,-25.0f, 2.0f));
    // points -> push_back(osg::Vec4f(0.0f,0.0f,-15.0f, 2.0f));
    // points -> push_back(osg::Vec4f(0.0f,0.0f,-10.0f, 2.0f));
    // points -> push_back(osg::Vec4f(0.0f, 0.0f, 0.0f, 2.0f));
    // points -> push_back(osg::Vec4f(5.0f, 5.0f, 0.0f, 2.0f));
    // points -> push_back(osg::Vec4f(5.0f, 5.0f, -10.0f, 2.0f));
    // points -> push_back(osg::Vec4f(5.0f, 5.0f, -15.0f, 2.0f));
    // points -> push_back(osg::Vec4f(5.0f, 5.0f, -25.0f, 2.0f));


    // points -> clear();
    // points -> push_back(osg::Vec4f( 268.3-200, 200-234.9, -9.799, 0.175));
    // points -> push_back(osg::Vec4f( 269.9-200, 200-234.8, -9.799, 0.175));
    // points -> push_back(osg::Vec4f( 270.9-200, 200-235.1, -9.799, 0.175));

    // points -> push_back(osg::Vec4f( 272.0-200, 200-235.2, -10.51, 0.175));
    // points -> push_back(osg::Vec4f( 273.4-200, 200-236.4, -10.66, 0.175));
    // points -> push_back(osg::Vec4f( 274.5-200, 200-237.1, -10.66, 0.175));
    // points -> push_back(osg::Vec4f( 275.3-200, 200-237.6, -11.34, 0.175));
    // points -> push_back(osg::Vec4f( 276.2-200, 200-237.7, -11.34, 0.175));

    // points -> push_back(osg::Vec4f(276.5-200, 200-238.0, -11.34, 0.175));
    // points -> push_back(osg::Vec4f(277.2-200, 200-239.0, -11.34, 0.175));
    // points -> push_back(osg::Vec4f(277.6-200, 200-239.5, -11.34, 0.175));
    // points -> push_back(osg::Vec4f(163.3-200, 200-85.71, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(162.3-200, 200-87.51, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(162.2-200, 200-87.58, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(160.4-200, 200-89.52, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(160.3-200, 200-89.67, -9.503, 1.120));
    // points -> push_back(osg::Vec4f(158.9-200, 200-90.24, -9.503, 1.120));




    // points -> clear();
    // points -> push_back(osg::Vec4f(10.0f, 0.0f, 0.0f, 0.175f));
    // points -> push_back(osg::Vec4f(5.0f, 0.0f, 0.0f, 0.175f));
    // points -> push_back(osg::Vec4f(2.0f, 0.0f, 0.0f, 0.175f));
    // points -> push_back(osg::Vec4f(0.1f, 0.0f, 0.0f, 0.175f));
    // points -> push_back(osg::Vec4f(3.0f, 3.0f, 0.0f, 0.175f));
    // points -> push_back(osg::Vec4f(0.0f, 3.0f, 0.0f, 0.175f));
    // points -> push_back(osg::Vec4f(0.0f, 3.0f, 1.0f, 0.175f));
    // points -> push_back(osg::Vec4f(0.0f, 3.0f, 5.0f, 0.175f));
    // points -> push_back(osg::Vec4f(0.0f, 3.0f, 10.0f, 0.175f));


    G0 = create_geometry_ring( osg::Vec3f( (*points)[0][0]
                                         , (*points)[0][1]
                                         , (*points)[0][2]
                                         )
                             , osg::Vec3f( (*points)[1][0]
                                         , (*points)[1][1]
                                         , (*points)[1][2]
                                         )
                             , (*points)[1][3]
                             , axial_segments
                             , radial_segments
                             );
    // geode -> addDrawable(G0);
    G0 -> setName(std::to_string(1));

    group -> addChild( mark( G0
                           , (*points)[1][3]
                           , axial_segments
                           , radial_segments
                           )
                     );


    G1 = create_geometry_ring( osg::Vec3f( (*points)[1][0]
                                         , (*points)[1][1]
                                         , (*points)[1][2]
                                         )
                             , osg::Vec3f( (*points)[2][0]
                                         , (*points)[2][1]
                                         , (*points)[2][2]
                                         )
                             , (*points)[2][3]
                             , axial_segments
                             , radial_segments
                             );
    G1 -> setName(std::to_string(2));

    // geode -> addDrawable(G1);
    twist( G0
         , G1
         , osg::Vec3f( (*points)[0][0]
                                         , (*points)[0][1]
                                         , (*points)[0][2]
                                         )
         , osg::Vec3f( (*points)[1][0]
                                         , (*points)[1][1]
                                         , (*points)[1][2]
                                         )
         , osg::Vec3f( (*points)[2][0]
                                         , (*points)[2][1]
                                         , (*points)[2][2]
                                         )
         , axial_segments
         , radial_segments
         );

    group -> addChild( mark( G1
                           , (*points)[2][3]
                           , axial_segments
                           , radial_segments
                           )
                     );


    G2 = create_geometry_ring( osg::Vec3f( (*points)[2][0]
                                         , (*points)[2][1]
                                         , (*points)[2][2]
                                         )
                             , osg::Vec3f( (*points)[3][0]
                                         , (*points)[3][1]
                                         , (*points)[3][2]
                                         )
                             , (*points)[3][3]
                             , axial_segments
                             , radial_segments
                             );
    G2 -> setName(std::to_string(3));
    twist( G1
         , G2
         , osg::Vec3f( (*points)[1][0]
                                         , (*points)[1][1]
                                         , (*points)[1][2]
                                         )
         , osg::Vec3f( (*points)[2][0]
                                         , (*points)[2][1]
                                         , (*points)[2][2]
                                         )
         , osg::Vec3f( (*points)[3][0]
                                         , (*points)[3][1]
                                         , (*points)[3][2]
                                         )
         , axial_segments
         , radial_segments
         );

    group -> addChild( mark( G2
                           , (*points)[3][3]
                           , axial_segments
                           , radial_segments
                           )
                     );

    G3 = create_geometry_ring( osg::Vec3f( (*points)[3][0]
                                         , (*points)[3][1]
                                         , (*points)[3][2]
                                         )
                             , osg::Vec3f( (*points)[4][0]
                                         , (*points)[4][1]
                                         , (*points)[4][2]
                                         )
                             , (*points)[4][3]
                             , axial_segments
                             , radial_segments
                             );
    G3 -> setName(std::to_string(4));

    twist( G2
         , G3
         , osg::Vec3f( (*points)[2][0]
                                         , (*points)[2][1]
                                         , (*points)[2][2]
                                         )
         , osg::Vec3f( (*points)[3][0]
                                         , (*points)[3][1]
                                         , (*points)[3][2]
                                         )
         , osg::Vec3f( (*points)[4][0]
                                         , (*points)[4][1]
                                         , (*points)[4][2]
                                         )
         , axial_segments
         , radial_segments
         );

    group -> addChild( mark( G3
                           , (*points)[4][3]
                           , axial_segments
                           , radial_segments
                           )
                     );
    for(uint i = 5; i < points -> size(); ++i)
    {
        osg::Vec3f parent_proximal  = osg::Vec3f( (*points)[i - 2][0]
                                                , (*points)[i - 2][1]
                                                , (*points)[i - 2][2]
                                                );
        osg::Vec3f parent_distal    = osg::Vec3f( (*points)[i - 1][0]
                                                , (*points)[i - 1][1]
                                                , (*points)[i - 1][2]
                                                );
        osg::Vec3f child_proximal   = parent_distal;
        osg::Vec3f child_distal     = osg::Vec3f( (*points)[i][0]
                                                , (*points)[i][1]
                                                , (*points)[i][2]
                                                );
        float r0 = (*points)[i - 2][3];
        float r1 = (*points)[i - 1][3];
        float r2 = (*points)[i    ][3];
        osg::Vec3f parent_direction = parent_distal - parent_proximal;
        float parent_length = parent_direction.normalize();
        osg::Vec3f child_direction  = child_distal  - child_proximal;
        float child_length = child_direction.normalize();

        float angle = acos(parent_direction * child_direction);
        //osg::notify(osg::DEBUG) << "Angle between " << i - 2 << ", " << i - 1 << ", " << i << " is " << angle;
        float diff = osg::PI / 12.0f;
        if(angle >= osg::PI/2.0f) // - diff && angle <= osg::PI/2.0 + diff)
        {
            RECORD_ERROR("Reaching here because angle is " + std::to_string(angle * 180.0 / osg::PI));
            osg::Vec3f plane_normal = parent_direction ^ child_direction; plane_normal.normalize();
            osg::Vec3f D0 = osg::Quat(angle / 3.0f, plane_normal) * parent_direction; D0.normalize();
            osg::Vec3f D1 = osg::Quat(2 * angle / 3.0f, plane_normal) * parent_direction; D1.normalize();
            std::cerr << "Checking angle => " << acos(parent_direction * D0) << std::endl;
            std::cerr << "Checking angle => " << acos(D0 * D1) << std::endl;
            std::cerr << "Checking angle => " << acos(D1 * child_direction) << std::endl;

            osg::Vec3f temp = osg::Quat(angle, plane_normal) * parent_direction; temp.normalize();
            std::cerr << "D0 " <<  D0.x() << ", " << D0.y() << ", " << D0.z() << std::endl;
            std::cerr << "D1 " << D1.x() << ", " << D1.y() << ", " << D1.z() << std::endl;
            std::cerr << temp.x() << ", " << temp.y() << ", " << temp.z() << std::endl;
            std::cerr << "Child Direction " << child_direction.x() << ", " << child_direction.y() << ", " << child_direction.z() << std::endl;
            osg::Vec3f C0;
            osg::Vec3f C1;
            if(parent_length < child_length)
            {
                C0 = (parent_proximal + parent_distal) / 2.0f;
                C1 = child_proximal + child_direction * parent_length / 2.0f;
            }
            else
            {
                C1 = (child_proximal + child_distal) / 2.0f;
                C0 = parent_distal - parent_direction * child_length / 2.0f;
            }
           // Point of intersection between ->
            // P = C0 + t * D0
            // P = C1 + u * D1
            // C0 X D1 + t * D0 X D1 = C1 X D1
            // t = ||(C1 - C0) X D1|| / ||D0 X D1||

            osg::Vec3f numerator = (C1 - C0) ^ D1;
            osg::Vec3f denominator = D0 ^ D1;
            float n = numerator.length();
            float d = denominator.length();
            float t = n / d;
            osg::Vec3f C01 = C0 + D0 * t;

          // osg::Vec4f C01( (C0.x() * D1.x() - C1.x() * D0.x()) / (D1.x() - D0.x())
          //               , (C0.y() * D1.y() - C1.y() * D0.y()) / (D1.y() - D0.y())
          //               , (C0.z() * D1.z() - C1.z() * D0.z()) / (D1.z() - D0.z())
          //               , r1
          //               );
          osg::Vec3f DTemp = C1 - C01; DTemp.normalize();
          std::cerr << "DTemp " << DTemp.x() << ", " << DTemp.y() << ", " << DTemp.z() << std::endl;
          std::cerr << "Sin angle between them => " << vector_angle(parent_proximal, C0, C01) * 180.0 / osg::PI << std::endl;
          std::cerr << "Another angle " << acos((C01 - C0) * D1 / (C01 - C0).length()) * 180.0 / osg::PI << std::endl;
          std::cerr << "Sin angle between them => " << vector_angle(C0, C01, C1) * 180.0 / osg::PI << std::endl;
          std::cerr << "Sin angle between them => " << vector_angle(C01, C1, child_distal) * 180.0 / osg::PI << std::endl;
          RECORD_ERROR("Hmmm!");
          points -> insert(points -> begin() + i - 1, osg::Vec4f(C0, r0));
          points -> insert(points -> begin() + i    , osg::Vec4f(C01, r1));
          (*points)[i + 1] = osg::Vec4f(C1, r2);
          RECORD_ERROR("Waaat ??");
          std::cerr << (*points)[i - 2].x() << ","<< (*points)[i - 2].y() << ","<< (*points)[i - 2].z() << ","<< (*points)[i - 2].w() << std::endl;
          std::cerr << (*points)[i - 1].x() << ","<< (*points)[i - 1].y() << ","<< (*points)[i - 1].z() << ","<< (*points)[i - 1].w() << std::endl;
          std::cerr << (*points)[i    ].x() << ","<< (*points)[i    ].y() << ","<< (*points)[i    ].z() << ","<< (*points)[i    ].w() << std::endl;
          std::cerr << (*points)[i + 1].x() << ","<< (*points)[i + 1].y() << ","<< (*points)[i + 1].z() << ","<< (*points)[i + 1].w() << std::endl;
          std::cerr << (*points)[i + 2].x() << ","<< (*points)[i + 2].y() << ","<< (*points)[i + 2].z() << ","<< (*points)[i + 2].w() << std::endl;

          osg::Vec3f L0 = convert((*points)[i - 1] - (*points)[i - 2]);
          osg::Vec3f L1 = convert((*points)[i    ] - (*points)[i - 1]);
          osg::Vec3f L2 = convert((*points)[i + 1] - (*points)[i    ]);
          osg::Vec3f L3 = convert((*points)[i + 2] - (*points)[i + 1]);

          std::cerr << "A10 => " << acos(L0 * L1 / L0.length() / L1.length()) * 180.0 / osg::PI << std::endl;
          std::cerr << "A21 => " << acos(L1 * L2 / L1.length() / L2.length()) * 180.0 / osg::PI << std::endl;
          std::cerr << "A32 => " << acos(L3 * L2 / L3.length() / L2.length()) * 180.0 / osg::PI << std::endl;
          i = i - 1;
          ++runs;
          continue;
        }
        //if(test == 3) { exit(0); }
        osg::Vec3f center = (parent_distal + child_distal) / 2.0f;
        osg::Vec3f direction = (parent_distal - child_distal);
        float height = direction.normalize();
        G4 = create_geometry_ring( parent_distal
                                 , child_distal
                                 , (*points)[i][3]
                                 , axial_segments
                                 , radial_segments
                                 );
        RECORD_ERROR("\n");
        RECORD_ERROR(std::to_string(i));
        twist( G3
             , G4
             , parent_proximal
             , parent_distal
             , child_distal
             , axial_segments
             , radial_segments
             );

        G4 -> setName(std::to_string(i));
        catmull_rom_bounded_compartment( G0
                                       , G1
                                       , G2
                                       , G3
                                       , G4
                                       , 0.25f
                                       , axial_segments
                                       , radial_segments
                                       );
        group -> addChild( mark( G4
                               , (*points)[i][3]
                               , axial_segments
                               , radial_segments
                               )
                         );

        //group -> addChild(coordinate_system(center, height, direction, (*points)[i][3]));

        geode -> addDrawable(G2);
        G0 = G1;
        G1 = G2;
        G2 = G3;
        G3 = G4;
        // break;
    }
    group -> addChild(geode);


    return group;
}

osg::Group *
catmull_rom_segment_2()
{
    osg::Group * group = new osg::Group();
    osg::Geode * geode = new osg::Geode();
    group -> addChild(geode);
    osg::Geometry * g1 = create_geometry_ring( osg::Vec3f(10.0f , 0.0f, 0.0f)
                                             , osg::Vec3f(-10.0f  , 0.0f, 0.0f)
                                             , 1.0f
                                             , 10
                                             , 20
                                             );
    osg::Geometry * g2 = create_geometry_ring( osg::Vec3f(-10.0f , -10.0f, 0.0f)
                                             , osg::Vec3f(+10.0f  , +10.0f, 0.0f)
                                             , 1.0f
                                             , 10
                                             , 20
                                             );
    twist( g1
         , g2
         , osg::Vec3f(10.0f , 0.0f, 0.0f)
         , osg::Vec3f(-10.0f , 0.0f, 0.0f)
         , osg::Vec3f(-10.0f , -10.0f,0.0f)
         , osg::Vec3f(+10.0f , +10.0f,0.0f)
         , 10
         , 20
         );

    geode -> addDrawable(g1);
    geode -> addDrawable(g2);
    group -> addChild( mark( g1
                           , 1.0f
                           , 10
                           , 20
                           )
                     );
    group -> addChild( mark( g2
                           , 1.0f
                           , 10
                           , 20
                           )
                     );
    return group;
}

osg::LightSource *
create_light_source( const osg::Vec4f & diffuse
            , const osg::Vec4f & ambient
            , const osg::Vec3f & center
            , const osg::Vec3f & direction
            , const float radius
            , const uint index
            )
{
    osg::Vec4f position(center + direction * radius,0.0f);
    // position[3] = 0.0;
    osg::Light * light = new osg::Light();
    osg::LightSource * light_source = new osg::LightSource();
    light->setDiffuse(diffuse);
    light->setAmbient(ambient);
    light->setDirection(direction);
    light->setPosition(position);
    light->setLightNum(index);
    light_source -> setLight(light);
    return light_source;
}

void
add_lighting(osg::Group * group)
{

    Light * light;
    LightSource * light_source;
    const osg::BoundingSphere & bounding_sphere = group -> getBound();
    osg::Vec3f center                           = bounding_sphere.center();
    float radius                                = bounding_sphere.radius();

    osg::Vec4f ambient(0.0f, 0.0f, 0.0f, 1.0f);
    osg::Vec4f diffuse(1.0f, 1.0f, 1.0f, 0.0f);

    group -> addChild(create_light_source( diffuse
                                         , ambient
                                         , center
                                         , osg::X_AXIS
                                         , radius
                                         , 1
                                         )
                     );
    group ->getOrCreateStateSet()->setMode( GL_LIGHT1, osg::StateAttribute::ON );

    group -> addChild(create_light_source( diffuse
                                         , ambient
                                         , center
                                         , osg::Vec3f(-1.0f, 0.0f, 0.0f)
                                         , radius
                                         , 2
                                         )
                     );
    group ->getOrCreateStateSet()->setMode( GL_LIGHT2, osg::StateAttribute::ON );

    group -> addChild(create_light_source( diffuse
                                         , ambient
                                         , center
                                         , osg::Vec3f(0.0f, 1.0f, 0.0f)
                                         , radius
                                         , 3
                                         )
                     );
    group ->getOrCreateStateSet()->setMode( GL_LIGHT3, osg::StateAttribute::ON );

    group -> addChild(create_light_source( diffuse
                                         , ambient
                                         , center
                                         , osg::Vec3f(0.0f, -1.0f, 0.0f)
                                         , radius
                                         , 4
                                         )
                     );
    group ->getOrCreateStateSet()->setMode( GL_LIGHT4, osg::StateAttribute::ON );

    group -> addChild(create_light_source( diffuse
                                         , ambient
                                         , center
                                         , osg::Vec3f(0.0f, 0.0f, 1.0f)
                                         , radius
                                         , 5
                                         )
                     );
    group ->getOrCreateStateSet()->setMode( GL_LIGHT5, osg::StateAttribute::ON );

    group -> addChild(create_light_source( diffuse
                                         , ambient
                                         , center
                                         , osg::Vec3f(0.0f, 0.0f, -1.0f)
                                         , radius
                                         , 6
                                         )
                     );
    group ->getOrCreateStateSet()->setMode( GL_LIGHT6, osg::StateAttribute::ON );

}


int main(int argc, char const *argv[])
{
    osg::Group * group = catmull_rom_segment();
    osgViewer::Viewer viewer;

    osg::StateSet* stateSet = group -> getOrCreateStateSet();
    osg::Material* material = new osg::Material;

    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAlpha( osg::Material::FRONT_AND_BACK
                      , 1.0
                      );
    material->setShininess( osg::Material::FRONT_AND_BACK
                          , 10.0
                          );
    material->setAmbient( osg::Material::FRONT_AND_BACK
                        , osg::Vec4(1.0, 1.0, 1.0, 1.0)
                        );
    material->setDiffuse( osg::Material::FRONT_AND_BACK
                        , osg::Vec4(1.0, 1.0, 1.0, 1.0)
                        );
    material->setColorMode( osg::Material::EMISSION);
    material->setEmission( osg::Material::FRONT_AND_BACK
                         , osg::Vec4(0.01, 0.01, 0.01, 0.1)
                         );
    // material->setShininess(osg::Material::FRONT_AND_BACK, 128.0);
    // material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));
    // material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));

    stateSet -> setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet -> setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    stateSet -> setMode( GL_BLEND, osg::StateAttribute::ON );
    stateSet -> setMode( GL_ALPHA_TEST, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);// just added this now as a test but still no luck
    osg::ShadeModel * shade_model = new osg::ShadeModel(osg::ShadeModel::SMOOTH);
    stateSet -> setAttributeAndModes(shade_model,osg::StateAttribute::ON);
    stateSet -> setRenderingHint( osg::StateSet::TRANSPARENT_BIN );


    /*
    add_lighting(group);
*/
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    viewer.setCameraManipulator( new osgGA::TrackballManipulator() );
    // viewer.setEventHandler(new osgViewer::ScreenCaptureHandler());
    viewer.getCamera() -> setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    // viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);
    viewer.setSceneData(group);
    viewer.addEventHandler(new GeometrySelector());
    viewer.realize();
    viewer.run();
    std::cerr << "Ran => " << runs << std::endl;
    return 0;
}
