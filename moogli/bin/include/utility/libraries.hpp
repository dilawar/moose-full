#ifndef __LIBRARIES_HPP__
#define __LIBRARIES_HPP__


#undef ANY
/******************************************************************************/
/* C++ STANDARD LIBRARY HEADERS                                               */
/******************************************************************************/
#include <cmath>
#include <cfloat>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <utility>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

/******************************************************************************/
/* OSG HEADERS                                                                */
/******************************************************************************/
#include <osg/Shape>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Billboard>
#include <osg/Vec3f>
#include <osg/Vec4f>
#include <osg/Vec3d>
#include <osg/Vec4d>
#include <osg/ref_ptr>
#include <osgViewer/Viewer>
#include <osgText/FadeText>
#include <osg/Plane>
#include <osg/MatrixTransform>
#include <osg/AutoTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/DisplaySettings>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/LineWidth>
#include <osgText/Text3D>
#include <osgGA/EventQueue>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/PolytopeIntersector>

#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>

#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/CompositeViewer>
#include <osgViewer/GraphicsWindow>

#include <osgQt/GraphicsWindowQt>

#include <osgGA/TrackballManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/OrbitManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/TerrainManipulator>

#include <osg/ShadeModel>
#include <osg/Material>
#include <osg/LightSource>
#include <osg/Light>
#include <osg/StateSet>
#include <osg/Depth>
#include <osgFX/Outline>
#include <osgFX/Cartoon>
#include <osgFX/Scribe>
#include <osgFX/SpecularHighlights>
#include <osgDB/WriteFile>
#include <osgSim/ScalarBar>

/******************************************************************************/
/* QT HEADERS                                                                 */
/******************************************************************************/
#include <QTimer>
#include <QtOpenGL>
#include <QApplication>
#include <QGridLayout>
#include <QScrollArea>
#include <QMenu>
#include <QCursor>
#include <QDrag>
#include <QToolBar>
#include <QPushButton>
#include <QDebug>
#include <QKeyEvent>
#include <QWheelEvent>

/******************************************************************************/
/* UTILITY HEADERS                                                            */
/******************************************************************************/
#include "utility/record.hpp"
#include "utility/conversions.hpp"
#include "utility/globals.hpp"

#include "Python.h"
#include <sip.h>

#define ANY void
#endif  /* __LIBRARIES_HPP__ */



