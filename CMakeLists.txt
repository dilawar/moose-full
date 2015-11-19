CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(moogli)


# A phony target to setup correct depedencies. This is always out-of-date.
add_custom_target(_moogli_all ALL)

set(MOOGLI_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/moogli)
add_custom_target(_build_moogli ALL
    DEPENDS ${MOOGLI_SRC_DIR}/core/_moogli.so
    )

# Check if OpenSceneGraph available.
FIND_PACKAGE(OpenSceneGraph 3.2.0)
IF(NOT OPENSCENEGRAPH_FOUND)
    MESSAGE(STATUS "Building OSG locally")
    SET(OSG_LOCAL ON)
    SET(BUILD_STATIC ON)
else()
    SET(BUILD_STATIC OFF)
    MESSAGE(STATUS "OSG Found: ${OPENSCENEGRAPH_VERSION}")
ENDIF()

IF(OSG_LOCAL)
    include(build_static_osg.cmake)
ENDIF(OSG_LOCAL)

# Now build MOOGLI. Don't use external project here. There is no simple way to
# honour environment variable in ExternalProject. 
set(MOOGLI_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/_moogli_)
file(MAKE_DIRECTORY ${MOOGLI_BUILD_DIR})
add_custom_command(
    OUTPUT ${MOOGLI_SRC_DIR}/core/_moogli.so
    COMMAND OSGDIR=${OSG_INSTALL_DIR}
        ${CMAKE_COMMAND} -DBUILD_STATIC=${BUILD_STATIC} ${MOOGLI_SRC_DIR}
    COMMAND $(MAKE) VERBOSE=1
    WORKING_DIRECTORY ${MOOGLI_BUILD_DIR}
    VERBATIM
    )
add_dependencies(_moogli_all _build_moogli)


# Install using python setup.py
FIND_PACKAGE(PythonInterp REQUIRED)
install(CODE
    "EXECUTE_PROCESS(
    COMMAND ${PYTHON_EXECUTABLE} moogli/cmake_modules/setup.py install
        --prefix=${CMAKE_INSTALL_PREFIX}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ")

enable_testing()
add_test(NAME moogli_import 
    COMMAND ${PYTHON_EXECUTABLE} -c "import moogli"
    WORKING_DIRECTORY ${MOOGLI_SRC_DIR}/
    )

