CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
# setup SIP variables
PROJECT(moogli)

SET(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")

## Options
option(BUILD_STATIC "Link osg statically. Useful for creating standalone packages" OFF)

MESSAGE("++ Reading module files from ${CMAKE_MODULE_PATH}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_COMPILER "clang++")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_COMPILER "g++")
else()
    message(WARNING "Continuing with unsupported compiler: ${CMAKE_CXX_COMPILER}")
endif()

## Enable/Disable 2011 stupport.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
ADD_DEFINITIONS(-fPIC)


FIND_PACKAGE(PythonDev REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

FIND_PACKAGE(PyQt4 REQUIRED)
MESSAGE("++ Found SIP flags: ${PYQT4_SIP_FLAGS}")
SEPARATE_ARGUMENTS(PYQT4_SIP_FLAGS) # convert space separated values to a list

SET(SIP_CONCAT_PARTS 4)
SET(SIP_EXTRA_OPTIONS ${PYQT4_SIP_FLAGS})

IF("${PYQT4_SIP_FLAGS}" STREQUAL "")
    MESSAGE(FATAL_ERROR "++ No PYQT4_SIP_FLAGS found. Can't continue")
ENDIF()
MESSAGE("++ Using sip flags: ${SIP_EXTRA_OPTIONS}")


IF (NOT BINDINGS_GLOBAL_INSTALL)
    SET(PYTHON_SITE_PACKAGES_DIR ${QGIS_DATA_DIR}/python)
ENDIF (NOT BINDINGS_GLOBAL_INSTALL)
IF (WITH_CUSTOM_WIDGETS)
    SET(PYUIC_WIDGET_PLUGIN_DIRECTORY ${PYQT4_MOD_DIR}/uic/widget-plugins/)
ENDIF (WITH_CUSTOM_WIDGETS)

FIND_PACKAGE(Qt4 REQUIRED COMPONENTS QtCore QtGui QtOpenGL)
#FIND_PACKAGE(osgAnimation REQUIRED)
FIND_PACKAGE(OpenSceneGraph REQUIRED 
    osgFX osgGA
    osgQt osgAnimation osgViewer
    osgManipulator osgSim osgText
    osgDB
    osgUtil
    )

MESSAGE(STATUS "OSG LIBRARIES: ${OPENSCENEGRAPH_LIBRARIES}")

INCLUDE(${QT_USE_FILE})

FIND_PROGRAM(SIP_EXECUTABLE sip)
MESSAGE("++ SIP ${SIP_EXECUTABLE}")

include_directories(./include ${PROJECT_SOURCE_DIR}/moogli/bin/include ".")
include_directories(${OSGQT_INCLUDE_DIR})
include_directories(${SIP_INCLUDES} ${PYTHON_INCLUDE_DIRS})
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
include_directories(${OSGANIMATION_INCLUDE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/moogli/bin/sip)
add_subdirectory(${PROJECT_SOURCE_DIR}/moogli/bin/src)

ADD_LIBRARY(_moogli SHARED 
    ${PROJECT_SOURCE_DIR}/moogli/bin/moc/Viewer.moc.cpp
    )

# Set MOOGLI paths if they are not already set.
IF(NOT MOOGLI_BUILD_PATH)
    SET(MOOGLI_BUILD_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    FILE(MAKE_DIRECTORY ${MOOGLI_BUILD_PATH})
ENDIF()

MESSAGE(STATUS "Building moogli in ${MOOGLI_BUILD_PATH}")
SET_TARGET_PROPERTIES(_moogli PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${MOOGLI_BUILD_PATH}/moogli/core"
    PREFIX ""
    )

## Link the following libraries to moogli.
SET(STATIC_LIBRARIES sip src)

SET(DYNAMIC_LIBRARIES
    QtCore
    QtGui
    QtOpenGL           
    )

IF(BUILD_STATIC)
    LIST(APPEND STATIC_LIBRARIES ${OPENSCENEGRAPH_LIBRARIES})
ELSE(BUILD_STATIC)
    LIST(APPEND DYNAMIC_LIBRARIES ${OPENSCENEGRAPH_LIBRARIES})
ENDIF(BUILD_STATIC)

TARGET_LINK_LIBRARIES(_moogli
    "-Wl,--whole-archive"
    ${STATIC_LIBRARIES}
    "-Wl,--no-whole-archive"
    ${DYNAMIC_LIBRARIES}
    )

# Build python source distribution of moogli
FIND_PACKAGE(PythonInterp REQUIRED)

# Install using python setup.py
install(CODE
    "EXECUTE_PROCESS(
        COMMAND ${PYTHON_EXECUTABLE} cmake_modules/setup.py install
        --prefix=${CMAKE_INSTALL_PREFIX}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ")

enable_testing()
add_test(NAME moogli_import 
    COMMAND ${PYTHON_EXECUTABLE} -c "import moogli"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
