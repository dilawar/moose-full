CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
## Now setting up project.
# A dummy target to add dependencies later.
project(moose-all)

add_custom_target(_moose_static_dependencies ALL)
# This variable should point to the source code.
set(MOOSE_CORE_DIRNAME moose-core)
set(MOOSE_GUI_DIRNAME moose-gui)
set(MOOSE_VERSION "3.0.2")

## CMAKE options
option(WITH_DOC "Build documentation as well" OFF)

## Project related macros
set(MOOSE_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${MOOSE_CORE_DIRNAME})

set(CMAKE_MODULE_PATH "${MOOSE_SOURCE_DIRECTORY}/cmake_modules")
message(STATUS "CMAKE_MODULE_PATH := ${CMAKE_MODULE_PATH}")

# This is a modification for packaging purpose only. This script should never be
# used for any other purpose.

##############
# Architecture.
MESSAGE("++ Testing for architecture: 64 bit or 32 bit")
if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    MESSAGE("++ 64 bit architecture")
    set(LIBSUFFIX "64")
    set(PKGARCH "amd64")
    set(RPMPKGARCH "x86_64")
else()
    MESSAGE("++ 32 bit architecture")
    set(LIBSUFFIX "")
    set(PKGARCH "i386")
endif()

################
# Definately building release type ##########################
set(CMAKE_BUILD_TYPE Release)
# This is essential to remove the RPATH.
set(CMAKE_INSTALL_RPATH "")

################
# Which local libraries need to be built, Here we just set the
# the flags.
find_package(GSL 1.16)
# If installed version of gsl is not 1.16 then download and build gsl locally.
# THIS IS USEFULL TO BUILD MOOSE FOR VARIOUS DISTRIBUTIONS.
if(NOT GSL_FOUND)
    MESSAGE("++ No GSL found. Using local gsl")
    set(LOCAL_GSL 1)
else()
    message("++ GSL_VERSION ${GSL_VERSION}")
    if(${GSL_VERSION} VERSION_LESS 1.16)
        MESSAGE("++ Trouble. Needed version 1.16. I'll now build GSL locally and
        link it with MOOSE")
        SET(LOCAL_GSL 1)
    endif()
endif()

set(ALL_STATIC_LIBS "")
if(LOCAL_GSL)
    include(${CMAKE_CURRENT_SOURCE_DIR}/build_static_gsl.cmake)
endif()

## SBML
find_package(LIBSBML)
if(NOT LIBSBML_FOUND)
    MESSAGE("++ LIBSBML NOT FOUND. Using local SBML")
    SET(LOCAL_SBML 1)
else()
    SET(LOCAL_SBML 0)
    MESSAGE("++ SBML found")
endif()

if(LOCAL_SBML)
    message("++ Building LOCAL SBML")
    include(${CMAKE_CURRENT_SOURCE_DIR}/build_static_sbml.cmake)
endif() 


## Now build MOOSE
SET(MOOSE_CORE_BUILD_DIR ${CMAKE_BINARY_DIR}/_moose-core)
FILE(MAKE_DIRECTORY ${MOOSE_CORE_BUILD_DIR})
## Dont use external project. The environment variables are lost.
message(STATUS "CMAKE_INSTALL_PREFIX : ${CMAKE_INSTALL_PREFIX}")
add_custom_command(
    OUTPUT ${MOOSE_CORE_BUILD_DIR}/moose.bin ${MOOSE_CORE_BUILD_DIR}/moose-${MOOSE_VERSION}.tar.gz
    # Pass the environment variable so local installation gets searched.
    COMMAND GSL_HOME=${GSL_INSTALL_DIR} LIBSBML_DIR=${SBML_INSTALL_DIR}
    ${CMAKE_COMMAND} 
        -DDEBUG:BOOL=OFF 
        -DWITH_DOC:BOOL=${WITH_DOC}
        -DLOCAL_SBML:BOOL=${LOCAL_SBML} -DLOCAL_GSL:BOOL=${LOCAL_GSL}
        -DSTATIC_GSL_LIBRARY=${STATIC_GSL_LIBRARY}
        -DSTATIC_GSLBLAS_LIBRARY=${STATIC_GSLBLAS_LIBRARY}
        -DSTATIC_SBML_LIBRARY=${STATIC_SBML_LIBRARY}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_SKIP_RPATH=ON
        ${MOOSE_SOURCE_DIRECTORY}
    COMMAND $(MAKE) VERBOSE=1
    # Not running neuroml test since it requires matplotlib.
    #COMMAND ctest -E neuroml --output-on-failure
    # Build and test, do not install here.
    WORKING_DIRECTORY ${MOOSE_CORE_BUILD_DIR}
    VERBATIM
    )

add_custom_target(_build_moose_core ALL
    DEPENDS ${MOOSE_CORE_BUILD_DIR}/moose.bin 
            ${MOOSE_CORE_BUILD_DIR}/libmoose.so
            )

add_dependencies(_build_moose_core _moose_static_dependencies)

# install moose-core 
install(CODE
    "EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -P cmake_install.cmake
        WORKING_DIRECTORY ${MOOSE_CORE_BUILD_DIR}
        )
    ")

# Copy the gui folder. 
install(DIRECTORY ${CMAKE_SOURCE_DIR}/moose-gui/
    DESTINATION lib/moose/gui
    COMPONENT moose-gui
    )

install(PROGRAMS ${CMAKE_SOURCE_DIR}/package_data/moosegui
    DESTINATION bin
    COMPONENT moose-gui
    )

# Keep the examples outside the gui directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/moose-examples/
    DESTINATION lib/moose/moose-examples
    COMPONENT moose-gui
    PATTERN "travis" EXCLUDE
    PATTERN "CVS" EXCLUDE
    PATTERN ".git*" EXCLUDE
    )

## Install the icon
install(FILES ${CMAKE_SOURCE_DIR}/moose-gui/icons/moose_icon.png 
    DESTINATION share/icons/moose
    COMPONENT moose-gui
    RENAME moose.png
    )

# And the desktop file.
install(FILES ${CMAKE_SOURCE_DIR}/moose-core/scripts/moose.desktop
    DESTINATION share/applications
    COMPONENT moose-gui
    )

### Tests.
enable_testing()
add_test(NAME moose-core 
    COMMAND ctest --output-on-failure
    WORKING_DIRECTORY ${MOOSE_CORE_BUILD_DIR}
    )
