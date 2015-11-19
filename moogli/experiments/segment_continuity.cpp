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

std::tuple<osg::Vec3f, osg::Vec3f, float>
interpolate( const osg::Vec3f & p0
           , const osg::Vec3f & p1
           , const osg::Vec3f & control
           , float radius0
           , float radius1
           , float t
           )
{
    float u  = 1.0f - t;
    float uu = u * u;
    osg::Vec3f position( p0 * uu
                       + control * 2 * u * t
                       + p1 * t * t
                       );
    osg::Vec3f direction( (control - p0) * 2.0f * u
                        + (p1 - control) * 2.0f * t
                        );
    float radius = radius0 + t * (radius1 - radius0);
    return std::tuple<osg::Vec3f, osg::Vec3f, float>( position
                                                         , direction
                                                         , radius
                                                         );
}

osg::Vec3f
quadratic_spline( const osg::Vec3f & p0
                , const osg::Vec3f & p1
                , const osg::Vec3f & control
                , float t
                )
{
    float u  = 1.0f - t;
    float uu = u * u;
    return ( p0 * uu
           + control * 2 * u * t
           + p1 * t * t
           );
}

osg::Vec3f
cubic_bezier_interpolate( const osg::Vec3f & p0
                        , const osg::Vec3f & p1
                        , const osg::Vec3f & p2
                        , const osg::Vec3f & p3
                        , float t
                        )
{
    float s = 1.0f - t;
    float ss = s * s;
    float sss = ss * s;
    float tt = t * t;
    float ttt = t * t * t;

    return ( p0 * sss
           + p1 * 3 * ss * t
           + p2 * 3 * s * tt
           + p3 * ttt
           );
}

void
generate_ring( const osg::Vec3Array * p0s
             , const osg::Vec3Array * p1s
             , const osg::Vec3Array * p2s
             , const osg::Vec3Array * p3s
             , float t
             , osg::Vec3Array * vertices
             , osg::Vec3Array * normals
             , size_t index
             )
{
    size_t size = p0s -> size();
    for(size_t i = 0; i < size; ++i)
    {
        osg::Vec3f mid(((*p1s)[i] + (*p2s)[i]) / 2.0f);
        (*vertices)[index + i] = cubic_bezier_interpolate( (*p0s)[i]
                                                         , (*p1s)[i] //mid //(*p1s)[i]
                                                         , (*p2s)[i] //mid // (*p2s)[i]
                                                         , (*p3s)[i]
                                                         , t
                                                         );
    }
    // std::cerr << "Here";
}


void
generate_normals( osg::Vec3Array * vertices
                , osg::Vec3Array * normals
                , size_t row_count
                , size_t col_count
                )
{
    for(size_t row = 0; row < row_count; ++row)
    {
        for(size_t col = 0; col < col_count / 2; ++col)
        {
            size_t current_index    = row * col_count + col;
            size_t opposite_index   = current_index + col_count / 2;
            (*normals)[current_index] = ((*vertices)[current_index] - (*vertices)[opposite_index]);
            (*normals)[current_index].normalize();
            (*normals)[opposite_index] = (*normals)[current_index] * -1.0f;
        }
    }

}

void
transform_ring( osg::Vec3Array *    ring
              , float               scale
              , osg::Vec3f          direction
              , osg::Vec3f          center
              , osg::Vec3Array *    vertices
              , size_t              index
              )
{
    osg::Quat rotate;
    rotate.makeRotate(osg::Vec3f(0.0f, 0.0f, 1.0f), direction);
    for(size_t i = 0; i < ring -> size(); ++i)
    {
        (*vertices)[index + i] = rotate * ((*ring)[i] * scale) + center;
    }
}


void
hide_vertices( osg::Vec3Array * vertices
             , size_t axial_segments
             , size_t radial_segments
             , size_t index
             )
{
    size_t row, col;
    size_t rows = axial_segments / 2 + 2;
    size_t cols = radial_segments;
    for(row = 0; row < rows; ++row)
    {
        for(col = 0; col < cols; ++col)
        {
            if(col == index)
            {
                (*vertices)[row * radial_segments + col].set(0.0f, 0.0f, 0.0f);
            }
        }
    }
}

void
connect_vertices( osg::Vec3Array * vertices
                , osg::DrawElementsUShort * indices
                , size_t row_count
                , size_t col_count
                )
{
    size_t triangle_index = 0;
    size_t row, col;
    for(row = 0; row < row_count - 1; ++row)
    {
        for(col = 0 ; col < col_count; ++col)
        {
            /*************************************

            d---------------c
            |               |
            |               |
            |               |
            |               |
            |               |
            |               |
            a---------------b

            ***************************************/
            ushort a = row * col_count + col;
            ushort b = a + 1;
            ushort d = a + col_count;
            ushort c = d + 1;
            // std::cerr << a <<" " << b << " " << c << " " << d << std::endl;
            (*indices)[triangle_index    ] = a;
            (*indices)[triangle_index + 1] = b;
            (*indices)[triangle_index + 2] = d;

            (*indices)[triangle_index + 3] = d;
            (*indices)[triangle_index + 4] = b;
            (*indices)[triangle_index + 5] = c;

            // (*indices)[triangle_index    ] = a;
            // (*indices)[triangle_index + 1] = b;
            // // (*indices)[triangle_index + 2] = b; //
            // // (*indices)[triangle_index + 3] = b + 1; //
            // (*indices)[triangle_index + 2] = d;
            // (*indices)[triangle_index + 3] = c;
            // (*indices)[triangle_index + 4] = b;
            // (*indices)[triangle_index + 5] = c;

            triangle_index += 6;
        }

        (*indices)[triangle_index - 1]     = row * col_count + col_count;
        (*indices)[triangle_index - 2]     = row * col_count;
        (*indices)[triangle_index - 5]     = row * col_count;
        // (*indices)[triangle_index - 1]     = row * radial_segments + radial_segments ;
        // (*indices)[triangle_index - 3]     = row * radial_segments;

    }
    std::cerr << "Triangle Index => " << triangle_index << std::endl;
}

void
smoothen( const osg::Vec3f & parent_proximal
        , const osg::Vec3f & joint
        , const osg::Vec3f & child_distal
        , const float parent_radius
        , const float child_radius
        , float parent_fraction
        , float child_fraction
        , size_t axial_segments
        , size_t radial_segments
        , osg::Geometry * geometry
        )
{
    // axial_segments = axial_segments;
    const size_t row_count              = axial_segments + 1;
    const size_t col_count              = radial_segments;
    const size_t vertex_count           = row_count * radial_segments;
    const size_t triangle_count         = (row_count - 1) * 2 * radial_segments;
    osg::ref_ptr<osg::Vec3Array> vertices         = new osg::Vec3Array(vertex_count);
    osg::ref_ptr<osg::Vec3Array> normals          = new osg::Vec3Array(vertex_count);
    osg::ref_ptr<osg::DrawElementsUShort> indices = new osg::DrawElementsUShort(GL_TRIANGLES, triangle_count * 3);
    osg::ref_ptr<osg::Vec4Array> colors           = new osg::Vec4Array();
    colors -> push_back(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

    osg::Vec3f child_direction(child_distal - joint);
    osg::Vec3f parent_direction(joint - parent_proximal);
    float child_height  = child_direction.normalize();
    float parent_height = parent_direction.normalize();
    // std::cout << "Child Height => "     << child_height     << std::endl;
    // std::cout << "Parent Height => "    << parent_height    << std::endl;
    osg::Vec3f start_point = joint - parent_direction * parent_fraction * parent_height;
    osg::Vec3f end_point   = joint + child_direction  * child_fraction * child_height;
    std::cerr << "b";
    osg::ref_ptr<osg::Vec3Array> unit_ring(new osg::Vec3Array(radial_segments));
    for(size_t i = 0; i < radial_segments; ++i)
    {
        double theta = (i * 2 * osg::PI) / static_cast<double>(radial_segments);
        (*(unit_ring.get()))[i].set( float(cos(theta))
                                   , float(sin(theta))
                                   , 0.0f
                                   );
    }
    std::cerr << "c";
    osg::ref_ptr<osg::Vec3Array> p0s(new osg::Vec3Array(radial_segments));
    transform_ring( unit_ring.get()
                  , parent_radius
                  , parent_direction
                  , start_point
                  , p0s
                  , 0
                  );
    std::cerr << "d";
    osg::Vec3f direction1(parent_direction);// * 3 + child_direction);
    direction1.normalize();
    osg::ref_ptr<osg::Vec3Array> p1s(new osg::Vec3Array(radial_segments));
    transform_ring( unit_ring.get()
                  , parent_radius
                  , direction1
                  , joint - parent_direction * parent_height * 0.4
                  , p1s
                  , 0
                  );
    std::cerr << "e";

    osg::Vec3f direction2(child_direction);// * 3 + parent_direction);
    direction2.normalize();
    osg::ref_ptr<osg::Vec3Array> p2s(new osg::Vec3Array(radial_segments));
    transform_ring( unit_ring.get()
                  , child_radius
                  , direction2
                  , joint + child_direction * parent_height * 0.4
                  , p2s
                  , 0
                  );
    std::cerr << "f";

    osg::ref_ptr<osg::Vec3Array> p3s(new osg::Vec3Array(radial_segments));
    transform_ring( unit_ring.get()
                  , child_radius
                  , child_direction
                  , end_point
                  , p3s
                  , 0
                  );
    std::cerr << "g";

    std::cerr << "h";
    float t;
    size_t i;

    for(i = 0; i <= axial_segments; ++i)
    {
        t = float(i) / float(axial_segments);
        generate_ring( p0s.get()
                     , p1s.get()
                     , p2s.get()
                     , p3s.get()
                     , t
                     , vertices.get()
                     , normals.get()
                     , i * radial_segments
                     );
    }

    connect_vertices( vertices.get()
                    , indices.get()
                    , row_count
                    , col_count
                    );

    generate_normals( vertices.get()
                    , normals.get()
                    , row_count
                    , col_count
                    );

    geometry -> setVertexArray(vertices.get());
    geometry -> addPrimitiveSet(indices.get());

    // geometry -> addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, vertices -> size()));//   parent_indices.get());
    geometry -> setNormalArray( normals.get(), osg::Array::BIND_PER_VERTEX);

    geometry -> setColorArray(colors);
    geometry -> setColorBinding( osg::Geometry::BIND_OVERALL );

    // osgUtil::SmoothingVisitor::smooth(*parent_geometry);
    // osgUtil::SmoothingVisitor::smooth(*child_geometry);
}

void
cylinders( const osg::Vec3f & distal
         , const osg::Vec3f & proximal
         , float radius
         , float parent_fraction
         , float child_fraction
         , size_t radial_segments
         , osg::Geometry * geometry
         )
{

    const size_t row_count              = 2;
    const size_t col_count              = radial_segments;
    const size_t vertex_count           = row_count * radial_segments;
    const size_t triangle_count         = (row_count - 1) * 2 * radial_segments;
    osg::ref_ptr<osg::Vec3Array> vertices         = new osg::Vec3Array(vertex_count);
    osg::ref_ptr<osg::Vec3Array> normals          = new osg::Vec3Array(vertex_count);
    osg::ref_ptr<osg::DrawElementsUShort> indices = new osg::DrawElementsUShort(GL_TRIANGLES, triangle_count * 3);
    osg::ref_ptr<osg::Vec4Array> colors           = new osg::Vec4Array();
    colors -> push_back(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

    osg::Vec3f direction(distal - proximal);
    float height  = direction.normalize();
    // std::cout << "Child Height => "     << child_height     << std::endl;
    // std::cout << "Parent Height => "    << parent_height    << std::endl;
    osg::Vec3f start_point = proximal + direction * child_fraction * height;
    osg::Vec3f end_point   = distal - direction  * parent_fraction * height;

    osg::ref_ptr<osg::Vec3Array> unit_ring(new osg::Vec3Array(radial_segments));
    for(size_t i = 0; i < radial_segments; ++i)
    {
        double theta = (i * 2 * osg::PI) / static_cast<double>(radial_segments);
        (*(unit_ring.get()))[i].set( float(cos(theta))
                                   , float(sin(theta))
                                   , 0.0f
                                   );
    }

    transform_ring( unit_ring.get()
                  , radius
                  , direction
                  , start_point
                  , vertices.get()
                  , 0
                  );

    transform_ring( unit_ring.get()
                  , radius
                  , direction
                  , end_point
                  , vertices.get()
                  , radial_segments
                  );
    connect_vertices( vertices.get()
                    , indices.get()
                    , row_count
                    , col_count
                    );

    generate_normals( vertices.get()
                    , normals.get()
                    , row_count
                    , col_count
                    );

    geometry -> setVertexArray(vertices.get());
    geometry -> addPrimitiveSet(indices.get());

    // geometry -> addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, vertices -> size()));//   parent_indices.get());
    geometry -> setNormalArray( normals.get(), osg::Array::BIND_PER_VERTEX);

    geometry -> setColorArray(colors);
    geometry -> setColorBinding( osg::Geometry::BIND_OVERALL );

}


int
main(int argc, char * argv[])
{

    // osg::ArgumentParser parser(&argc, argv);

    // osgViewer::Viewer viewer(parser);

    // osg::ref_ptr<osg::Group> root = dynamic_cast<osg::Group*>(osgDB::readNodeFiles(psr));
    // if (!root)
    // {
    //     std::cout << psr.getApplicationName() <<": No data loaded" << std::endl;
    //     return 1;
    // }

    // {
    //     osgAnimation::AnimationManagerBase* animationManager = dynamic_cast<osgAnimation::AnimationManagerBase*>(root->getUpdateCallback());
    //     if(!animationManager)
    //     {
    //         osg::notify(osg::FATAL) << "no AnimationManagerBase found, updateCallback need to animate elements" << std::endl;
    //         return 1;
    //     }
    // }


    // osg::ref_ptr<osg::Group> scene = new osg::Group;

    // // add the state manipulator
    // viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );

    // // add the thread model handler
    // viewer.addEventHandler(new osgViewer::ThreadingHandler);

    // // add the window size toggle handler
    // viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    // // add the stats handler
    // viewer.addEventHandler(new osgViewer::StatsHandler);

    // // add the help handler
    // viewer.addEventHandler(new osgViewer::HelpHandler(psr.getApplicationUsage()));

    // // add the LOD Scale handler
    // viewer.addEventHandler(new osgViewer::LODScaleHandler);

    // // add the screen capture handler
    // viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);

    // viewer.setSceneData(scene.get());

    // viewer.realize();

    // double xChar = maxChar;
    // double yChar = xChar * 9.0/16;
    // for (double  i = 0.0; i < xChar; i++) {
    //     for (double  j = 0.0; j < yChar; j++) {

    //         osg::ref_ptr<osg::Group> c = createCharacterInstance(root.get(), hardware);
    //         osg::MatrixTransform* tr = new osg::MatrixTransform;
    //         tr->setMatrix(osg::Matrix::translate( 2.0 * (i - xChar * .5),
    //                                               0.0,
    //                                               2.0 * (j - yChar * .5)));
    //         tr->addChild(c.get());
    //         scene->addChild(tr);
    //     }
    // }
    // std::cout << "created " << xChar * yChar << " instance"  << std::endl;


    // return viewer.run();




    osg::Group * group = new osg::Group();
    osg::Geode * geode = new osg::Geode();
    osg::Geometry * geometry;
    float child_fraction = 0.5;
    float parent_fraction = 0.5;
    size_t axial_segments  = 10;
    size_t radial_segments = 10;
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
points -> push_back(osg::Vec4f( 162.8, -82.65, -9.543, 4.634));
points -> push_back(osg::Vec4f( 162.9, -83.44, -9.533, 3.774));
points -> push_back(osg::Vec4f( 163.1, -84.22, -9.522, 3.365));
points -> push_back(osg::Vec4f( 163.2, -85.01, -9.512, 3.365));
points -> push_back(osg::Vec4f( 163.3, -85.8 ,-9.502 ,3.365));
points -> push_back(osg::Vec4f( 163.5, -86.58, -9.492, 3.365));
points -> push_back(osg::Vec4f(163.4, -86.0, -9.499, 1.24));
points -> push_back(osg::Vec4f(164.2, -87.9, -9.499, 0.59));
points -> push_back(osg::Vec4f(164.5, -89.03, -9.499, 0.415));
points -> push_back(osg::Vec4f(164.6, -90.4, -9.499, 0.415));
points -> push_back(osg::Vec4f(164.8, -91.61, -9.499, 0.415));
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
points -> push_back(osg::Vec4f( 272.0, -235.2, -10.51, 0.175));
points -> push_back(osg::Vec4f( 273.4, -236.4, -10.66, 0.175));
points -> push_back(osg::Vec4f( 274.5, -237.1, -10.66, 0.175));
points -> push_back(osg::Vec4f( 275.3, -237.6, -11.34, 0.175));
points -> push_back(osg::Vec4f( 276.2, -237.7, -11.34, 0.175));
points -> push_back(osg::Vec4f( 276.5, -238.0, -11.34, 0.175));
points -> push_back(osg::Vec4f( 277.2, -239.0, -11.34, 0.175));
points -> push_back(osg::Vec4f( 277.6, -239.5, -11.34, 0.175));
points -> push_back(osg::Vec4f(163.3, -85.71, -9.503, 1.120));
points -> push_back(osg::Vec4f(162.3, -87.51, -9.503, 1.120));
points -> push_back(osg::Vec4f(162.2, -87.58, -9.503, 1.120));
points -> push_back(osg::Vec4f(160.4, -89.52, -9.503, 1.120));
points -> push_back(osg::Vec4f(160.3, -89.67, -9.503, 1.120));
points -> push_back(osg::Vec4f(158.9, -90.24, -9.503, 1.120));
points -> push_back(osg::Vec4f(157.8, -90.60, -8.303, 0.415));
points -> push_back(osg::Vec4f(156.8, -90.36, -8.303, 0.415));
points -> push_back(osg::Vec4f(156.6, -90.34, -8.303, 0.415));
points -> push_back(osg::Vec4f(155.5, -89.84, -8.303, 0.415));
points -> push_back(osg::Vec4f(155.3, -89.73, -8.303, 0.415));
points -> push_back(osg::Vec4f(154.1, -88.80, -8.303, 0.415));
points -> push_back(osg::Vec4f(154.1, -88.70, -8.303, 0.415));
points -> push_back(osg::Vec4f(153.5, -87.62, -8.303, 0.415));
points -> push_back(osg::Vec4f(152.0, -87.41, -8.303, 0.415));
points -> push_back(osg::Vec4f(152.0, -87.91, -7.563, 0.295));
points -> push_back(osg::Vec4f(151.8, -88.05, -7.563, 0.295));
points -> push_back(osg::Vec4f(150.6, -88.81, -7.563, 0.295));
points -> push_back(osg::Vec4f(150.3, -88.86, -7.563, 0.295));
points -> push_back(osg::Vec4f(148.8, -89.66, -7.563, 0.295));
points -> push_back(osg::Vec4f(148.6, -89.71, -7.563, 0.295));
points -> push_back(osg::Vec4f(147.9, -89.36, -7.563, 0.295));
points -> push_back(osg::Vec4f(147.4, -88.95, -7.593, 0.295));
points -> push_back(osg::Vec4f(146.8, -88.52, -7.593, 0.295));
points -> push_back(osg::Vec4f(146.4, -88.81, -7.593, 0.295));
points -> push_back(osg::Vec4f(145.8, -89.40, -7.593, 0.295));
points -> push_back(osg::Vec4f(145.4, -89.43, -7.593, 0.295));
points -> push_back(osg::Vec4f(144.6, -89.31, -7.563, 0.295));
points -> push_back(osg::Vec4f(144.4, -89.29, -7.593, 0.295));
points -> push_back(osg::Vec4f(144.0, -89.48, -7.193, 0.295));
points -> push_back(osg::Vec4f(143.9, -89.55, -7.193, 0.295));
points -> push_back(osg::Vec4f(142.3, -90.27, -7.163, 0.295));
points -> push_back(osg::Vec4f(141.4, -89.62, -7.163, 0.295));
points -> push_back(osg::Vec4f(141.3, -89.43, -7.163, 0.295));
points -> push_back(osg::Vec4f(140.7, -89.27, -7.223, 0.295));


    // points -> push_back(osg::Vec4f(0.0, 0.0, 0.0, 8.88119));
    // points -> push_back(osg::Vec4f(0.67, -4.33, -2.18, 2.165));
    // points -> push_back(osg::Vec4f(-0.09, -6.62, -2.34, 1.84));
    // points -> push_back(osg::Vec4f(-1.76, -8.01, -2.31, 1.405));
    // points -> push_back(osg::Vec4f(-2.97, -8.02, -2.2, 1.405));
    // points -> push_back(osg::Vec4f(-3.39, -8.37, -2.2, 1.405));
    // points -> push_back(osg::Vec4f(-3.48, -8.5, -2.2, 1.405));
    // points -> push_back(osg::Vec4f(-3.53, -8.81, -3.1, 1.19));
    // points -> push_back(osg::Vec4f(-3.61, -9.04, -3.55, 1.19));


    // points -> push_back(osg::Vec4f(-4.27, -8.08, -3.52, 0.11));
    // points -> push_back(osg::Vec4f(-5.31, -7.95, -3.18, 0.11));
    // points -> push_back(osg::Vec4f(-5.47, -8.42, -3.21, 0.54));
    // points -> push_back(osg::Vec4f(-6.69, -8.42, -3.11, 0.54));
    // points -> push_back(osg::Vec4f(-7.0, -7.52, -2.98, 0.54));
    // points -> push_back(osg::Vec4f(-8.21, -7.23, -2.84, 0.54));
    // points -> push_back(osg::Vec4f(-8.66, -7.68, -2.85, 0.54));
    // points -> push_back(osg::Vec4f(-9.28, -8.31, -2.85, 0.54));
    // points -> push_back(osg::Vec4f(-9.73, -9.06, -2.88, 0.54));
    // points -> push_back(osg::Vec4f(-10.03, -10.29, -2.97, 0.54));
    // points -> push_back(osg::Vec4f(-10.03, -10.88, -3.03, 0.54));
    // points -> push_back(osg::Vec4f(-10.18, -11.95, -3.12, 0.54));
    // points -> push_back(osg::Vec4f(-10.63, -12.42, -3.13, 0.54));
    // points -> push_back(osg::Vec4f(-10.94, -13.64, -3.22, 0.54));
    // points -> push_back(osg::Vec4f(-11.39, -14.1, -3.22, 0.54));
    // points -> push_back(osg::Vec4f(-11.68, -15.01, -3.28, 0.54));
    // points -> push_back(osg::Vec4f(-12.22, -15.99, -4.24, 0.65));
    // points -> push_back(osg::Vec4f(-12.53, -16.61, -4.26, 0.65));
    // points -> push_back(osg::Vec4f(-12.98, -17.36, -4.31, 0.65));


    // points -> push_back(osg::Vec4f(-13.66, -18.21, -5.16, 0.54));
    // points -> push_back(osg::Vec4f(-14.88, -18.82, -5.11, 0.54));
    // points -> push_back(osg::Vec4f(-16.85, -19.3 ,-4.97, 0.54));
    // points -> push_back(osg::Vec4f(-17.61, -20.37, -5.0, 0.54));
    // points -> push_back(osg::Vec4f(-17.76, -22.05, -5.16, 0.54));
    // points -> push_back(osg::Vec4f(-17.6 ,-22.81, -5.25, 0.54));
    // points -> push_back(osg::Vec4f(-17.44, -24.17, -5.39, 0.54));
    // points -> push_back(osg::Vec4f(-17.74, -25.1, -5.45, 0.54));
    // points -> push_back(osg::Vec4f(-19.12, -25.86, -5.4, 0.54));
    // points -> push_back(osg::Vec4f(-20.94, -25.87, -5.24, 0.54));
    // points -> push_back(osg::Vec4f(-22.15, -25.9, -5.05, 0.54));
    // points -> push_back(osg::Vec4f(-22.3,  -26.96, -5.14, 0.54));
    // points -> push_back(osg::Vec4f(-22.29, -27.57, -5.19, 0.54));
    // points -> push_back(osg::Vec4f(-22.44, -28.04, -5.22, 0.54));
    // points -> push_back(osg::Vec4f(-22.94, -27.75, -6.06, 0.435));
    // points -> push_back(osg::Vec4f(-23.25, -28.36, -6.09, 0.435));
    // points -> push_back(osg::Vec4f(-23.56, -29.27, -6.23, 0.435));
    // points -> push_back(osg::Vec4f(-24.05, -29.99, -6.72, 0.435));
    // points -> push_back(osg::Vec4f(-24.86, -30.7, -7.31, 0.435));
    // points -> push_back(osg::Vec4f(-26.08, -31.33, -7.26, 0.435));
    // points -> push_back(osg::Vec4f(-27.75, -31.18, -7.09, 0.435));


    for(size_t i = 1; i < points -> size() - 1; ++i)
    {
        osg::Vec3f parent_proximal( (*points)[i - 1][0]
                             , (*points)[i - 1][1]
                             , (*points)[i - 1][2]
                             );
        osg::Vec3f joint( (*points)[i][0]
                   , (*points)[i][1]
                   , (*points)[i][2]
                   );
        osg::Vec3f child_distal( (*points)[i + 1][0]
                          , (*points)[i + 1][1]
                          , (*points)[i + 1][2]
                          );
        float parent_radius = (*points)[i - 1][3];
        float child_radius  = (*points)[i][3];
        geometry = new osg::Geometry();
        smoothen( parent_proximal
                , joint
                , child_distal
                , parent_radius
                , child_radius
                , parent_fraction
                , child_fraction
                , axial_segments
                , radial_segments
                , geometry
                );
        geode -> addDrawable(geometry);

        geometry = new osg::Geometry();
        cylinders( joint
                 , parent_proximal
                 , parent_radius
                 , parent_fraction
                 , child_fraction
                 , radial_segments
                 , geometry
                 );
        geode -> addDrawable(geometry);
    }

    group -> addChild(geode);
    std::cerr << "Export Result : "
              << osgDB::writeNodeFile(*group, "./continous_segment.svg");
    std::cerr << "Export Result : "
              << osgDB::writeNodeFile(*group, "./continous_segment.3ds");
    osgViewer::Viewer viewer;
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    viewer.setCameraManipulator( new osgGA::TrackballManipulator() );
    viewer.setEventHandler(new osgViewer::ScreenCaptureHandler());
    viewer.getCamera() -> setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    // viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);
    viewer.setSceneData(group);
    viewer.realize();
    viewer.run();
    return 0;


    // The fraction has to be based upon the angle and radius difference
    // if the angle is 0.0, then fraction can be quite small
    // if the angle is 90.0 then fraction has to be big
    // plus it can be different for different
}

/**

The Idea

Generate a basic circular ring.

parent_ring => rotate and translate in accordance with parent axis and distal position
child_ring  => circular ring rotated and translated in accordance with the child axis and proximal position

Generate intermediate rings -

Generate intermediate points on the axis using quadratic bezier
Generate intermediate direction vectors
Generate intermediate radii values using quadratic bezier curve
scale translate and rotate the basic circular ring using the above values
Join the rings above
rinse and repeat
*/
