set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0004 OLD)

project(moose)
set(MOOSE_VERSION "3.0.2")

# This snippet is from LLVM project.
# Sanity check our source directory to make sure that we are not trying to
# generate an in-tree build (unless on MSVC_IDE, where it is ok), and to make
# sure that we don't have any stray generated files lying around in the tree
# (which would end up getting picked up by header search, instead of the correct
# versions).

MESSAGE("++ CMAKE_INSTALL_PREFIX : ${CMAKE_INSTALL_PREFIX}")
if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE )
    message(FATAL_ERROR 
        "In-source builds are not allowed.
        CMake would overwrite the makefiles distributed with Moose.
        Please create a directory and run cmake from there, passing the path
        to this source directory as the last argument.
        This process created the file `CMakeCache.txt' and the directory `CMakeFiles'.
        Please delete them."
        )
endif()

## NOTE: PyMOOSE might break with c++11 support. Disable it.
#set(CMAKE_CXX_COMPILER "clang++")
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
    )

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )

# This is for testing purpose.
link_directories(${CMAKE_CURRENT_BINARY_DIR})

################################# OS Specific ##################################
MESSAGE(STATUS "Operating system: ${CMAKE_SYSTEM_NAME}")
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    CMAKE_POLICY(SET CMP0042 NEW)
    set(MACOSX TRUE)
else()
    set(MACOSX FALSE)
endif()

################################# CMAKE OPTIONS ##################################

option(WITH_DOC "Build documentation using python-sphinx and doxygen" OFF)

option(VERBOSITY "SET MOOSE VERBOSITY LEVEL" 0)

# Whether to notify the user after 1 second of simulation.
option(NOTIFY_PROGRESS "Show progress of simulation" OFF)

# STATS INSIDE MOOSE. TODO: Remove it and add support for gprof
option(ENABLE_LOGGER  "Enable MOOSE logger [Deprecated]" OFF)

## Unit testing and debug mode.
option(DEBUG "Building with debugging support" OFF)
option(GPROF "Build of gprof support" OFF)
option(ENABLE_UNIT_TESTS "ENABLE UNIT TESTS IN MOOSE" ON)
option(WITH_PYTHON "Build native python extension of MOOSE" ON)
option(WITH_MPI  "Enable OPENMPI support" OFF)
option(WITH_CURSES "Enable ncurses" OFF)
# If GSL_STATIC_HOME is set, we use it to search for static gsl libs.
option(GSL_STATIC_HOME 
    "installation prefix where static gsl library can be found" 
    OFF
    )
option(SBML_STATIC_HOME 
    "installation prefix where static sbml library can be found"
    OFF
    )

################################# CMKAE MACROS #################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

########################### COMPILER MACROS #####################################

# Turn warning to error: Not all of the options may be supported on all
# versions of compilers. be careful here.
add_definitions(-Wall
    #-Wno-return-type-c-linkage
    -Wno-unused-variable
    -Wno-unused-function
    #-Wno-unused-private-field
    )
add_definitions(-fPIC)

## Enable/Disable 2011 stupport.
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

############################ BUILD CONFIGURATION #################################
set(VERIFY 1)
if(VERIFY)
    add_definitions(-DSANITY_CHECK -DARGS_CHECK -DRESULT_CHECK -DVALUE_CHECK)
endif(VERIFY)

# VERBOSITY OF OUTPUT
if(VERBOSITY)
    message("++ Verbosity of output is ${VERBOSITY}. This is not implemented
    yet!")
    add_definitions(-DVERBOSITY=${VERBOSITY})
else()
    message("++ Moose will be quiet")
    add_definitions(-DQUIET_MODE)
endif()
if(ENABLE_LOGGER)
    message("++ LOGGER ENABLED")
    add_definitions(-DENABLE_LOGGER)
endif()

# Default macros
add_definitions(-DUSE_GENESIS_PARSER)

if(ENABLE_UNIT_TESTS AND DEBUG)
    MESSAGE("++ Building for Debug/ Unit testing")
    add_definitions(-DDO_UNIT_TESTS)
    set(CMAKE_BUILD_TYPE Debug)
else()
    MESSAGE("++ Build for Relase/ No unit tests.")
    set(CMAKE_BUILD_TYPE Release)
    add_definitions(-UDO_UNIT_TESTS -O3)
endif()

if(GPROF AND DEBUG)
    message(STATUS "Compiling with GPROF")
    add_definitions(-pg)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-pg")
endif()

# DEPENDENCIES
find_package(LIBSBML)
if(SBML_STATIC_HOME)
    MESSAGE("++ Using local SBML. Support is ON")
    include_directories(${SBML_STATIC_HOME}/include)
    find_library(LIBSBML_LIBRARY
        NAMES libsbml_static.a libsbml.a
        PATHS ${SBML_STATIC_HOME}/lib ${SBML_STATIC_HOME}/lib64
        NO_DEFAULT_PATH
        )
    MESSAGE("++ Using static sbml library: ${LIBSBML_LIBRARY}")
    IF(LIBSBML_LIBRARY)
        set(LIBSBML_FOUND ON)
    ENDIF()
endif()

if(LIBSBML_FOUND)
    MESSAGE("++ SBML found. Support is ON: ${LIBSBML_LIBRARY}")
    include_directories(${LIBSBML_INCLUDE_DIR})
    find_package(BZip2 REQUIRED)
    find_package(LibXML2 REQUIRED)
    find_package(ZLIB REQUIRED)
endif()

################################### SETUP BUILD ################################
include_directories(msg basecode)
add_executable(moose.bin basecode/main.cpp)

add_library(libmoose SHARED basecode/main.cpp)
set_target_properties(libmoose PROPERTIES COMPILE_FLAGS  "-DMOOSE_LIB")
set_target_properties(libmoose PROPERTIES PREFIX "")

## GSL
add_definitions(-DUSE_GSL)

# If GSL_STATIC_HOME is set, we use it to search for static gsl libs.
IF(GSL_STATIC_HOME)
    SET(GSL_INCLUDE_DIR "${GSL_STATIC_HOME}/include")
    find_library(GSL_LIBRARY 
        NAME libgsl.a 
        PATHS ${GSL_STATIC_HOME}/lib ${GSL_STATIC_HOME}/lib64
        NO_DEFAULT_PATH
        )
    find_library(GSLCBLAS_LIBRARY
        NAME libgslcblas.a 
        PATHS ${GSL_STATIC_HOME}/lib ${GSL_STATIC_HOME}/lib64 
        NO_DEFAULT_PATH
        )
    SET(GSL_LIBRARIES ${GSL_LIBRARY} ${GSLCBLAS_LIBRARY})
    set(GSL_INCLUDE_DIR ${GSL_STATIC_HOME}/include)
    MESSAGE("++ STATIC GSL_LIBRARIES ${GSL_LIBRARIES}")
ELSE()
    find_package(GSL 1.16 REQUIRED)
ENDIF()

include_directories(${GSL_INCLUDE_DIR})

## Setup hdf5
find_package(HDF5)
if(FOUND_HDF5)
    add_definitions(-DUSE_HDF5)
    include_directories(${HDF5_INCLUDE_DIR})
else(FOUND_HDF5)
    message("++ HDF5 not found. Disabling support")
endif(FOUND_HDF5)

find_package(Termcap)
find_package(Readline)

if(READLINE_FOUND AND TERMCAP_FOUND)
    add_definitions(-DUSE_READLINE)
    include_directories(${Readline_INCLUDE_DIR})
endif()

# Openmpi
if(WITH_MPI)
    find_package(MPI REQUIRED)
    if(MPI_CXX_FOUND)
        message("++ Using MPI from ${MPI_CXX_INCLUDE_PATH}")
        include_directories(${MPI_CXX_INCLUDE_PATH})
        set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
        add_definitions(-DUSE_MPI)
        SET(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
        SET(CMAKE_C_COMPILER ${MPI_C_COMPILER})
    else()
        message("++ Cound not find MPI")
        add_definitions(-UUSE_MPI)
    endif()
endif(WITH_MPI)

# Add subdirectroeis
add_subdirectory(basecode)
add_subdirectory(msg)
add_subdirectory(shell)
add_subdirectory(randnum)
add_subdirectory(scheduling)
add_subdirectory(biophysics)
add_subdirectory(builtins)
add_subdirectory(utility)
add_subdirectory(external/muparser)
#add_subdirectory(external/debug)
add_subdirectory(external/tinyxml)
add_subdirectory(mesh)
add_subdirectory(sbml)
add_subdirectory(mpi)
add_subdirectory(signeur)
add_subdirectory(ksolve)
add_subdirectory(hsolve)
add_subdirectory(diffusion)
add_subdirectory(device)
add_subdirectory(benchmarks)
add_subdirectory(kinetics)
add_subdirectory(synapse)
add_subdirectory(intfire)
add_subdirectory(examples)

# These are always shared libraries.
set(SYSTEM_SHARED_LIBS ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES} 
    ${LibXML2_LIBRARIES} ${BZIP2_LIBRARIES}
    )

# These libraries could be static of dynamic.
# When building packages on OBS or launchpad, we build static library of a
# dependencies if a correct version is not found. 
# Here we make sure that the correct static version is linked; if a correct
# shared version is found then link the shared version in SYSTEM_SHARED_LIBS.

# If CMAKE_STATIC_LIBRARIES are mentiond on command line, use it as list.
set(STATIC_LIBRARIES  "")

if(FOUND_HDF5)
    list(APPEND SYSTEM_SHARED_LIBS ${HDF5_LIBRARIES})
endif()

LIST(APPEND STATIC_LIBRARIES moose_sbml)
IF(SBML_STATIC_HOME)
    list(APPEND STATIC_LIBRARIES ${LIBSBML_LIBRARY})
ELSEIF()
    list(APPEND SYSTEM_SHARED_LIBS ${LIBSBML_LIBRARY})
ENDIF()


if(GSL_STATIC_HOME)
    MESSAGE(STATUS "Using STATIC gsl libraries: ${GSL_LIBRARIES}")
    list(APPEND STATIC_LIBRARIES ${GSL_LIBRARIES})
else()
    MESSAGE(STATUS "Using system gsl : ${GSL_LIBRARIES}")
    list(APPEND SYSTEM_SHARED_LIBS ${GSL_LIBRARIES})
endif()

if(READLINE_FOUND AND TERMCAP_FOUND)
    list(APPEND SYSTEM_SHARED_LIBS ${Readline_LIBRARY} ${TERMCAP_LIBRARY})
endif()

if(WITH_MPI)
    if(MPI_CXX_FOUND)
        list(APPEND SYSTEM_SHARED_LIBS ${MPI_CXX_LIBRARIES})
    endif()
endif(WITH_MPI)

###################################### LINKING #################################
set(MOOSE_LIBRARIES
    moose_builtins
    msg
    benchmarks
    shell
    randnum
    scheduling
    moose_mpi
    biophysics 
    utility 
    muparser
    kinetics 
    synapse
    intfire
    hsolve 
    mesh
    signeur
    tinyxml
    diffusion 
    ksolve
    device
    basecode
    )


# MAC linker does not understand many of gnu-ld options.
IF(MACOSX)
    target_link_libraries(libmoose
        "-Wl,-all_load"
        ${MOOSE_LIBRARIES}
        ${STATIC_LIBRARIES}
        )

    target_link_libraries(libmoose
        ${SYSTEM_SHARED_LIBS}
        )

ELSE(MACOSX)
    target_link_libraries(libmoose 
        "-Wl,--whole-archive"
        ${MOOSE_LIBRARIES}
        ${STATIC_LIBRARIES}
        "-Wl,--no-whole-archive"
        ${SYSTEM_SHARED_LIBS}
        )
ENDIF(MACOSX)

add_dependencies(moose.bin libmoose)
target_link_libraries(moose.bin
    LINK_PUBLIC moose
    )

IF(WITH_DOC)
    FIND_PACKAGE(Doxygen REQUIRED)
    add_custom_command(TARGET libmoose POST_BUILD
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.full
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Building developer documentation"
        VERBATIM
        )
ENDIF(WITH_DOC)

######################### BUILD PYMOOSE ########################################
# Root of all python module.
SET(PROJECT_PYTHON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/python)
if(WITH_PYTHON)
    set(Python_ADDITIONAL_VERSION 2.6 2.7)
    message("++ Building native python extension of MOOSE")
    set(PYMOOSE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/python/moose")

    find_package(NumpyHeaders)
    if(PYTHON_NUMPY_FOUND)
        add_definitions(-DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION)
        include_directories(${PYTHON_NUMPY_INCLUDE_DIR})
        add_definitions(-DUSE_NUMPY)
    else()
        message(WARNING "Could not find numpy/arrayobject.h in usual places")
        add_definitions(-UUSE_NUMPY)
    endif()

    find_package(PythonDev REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})
    add_subdirectory(pymoose)
    add_library(_moose MODULE pymoose/moosemodule.cpp)

    set_target_properties(_moose PROPERTIES COMPILE_DEFINITIONS "PYMOOSE")
    set_target_properties(_moose PROPERTIES 
        LIBRARY_OUTPUT_DIRECTORY ${PYMOOSE_OUTPUT_DIRECTORY}
        PREFIX ""
        )

if(MACOSX)
    set(CMAKE_MODULE_LINKER_FLAGS "-undefined dynamic_lookup")
    MESSAGE(STATUS "ADDING some linker flags ${CMAKE_EXE_LINKER_FLAGS}")
endif(MACOSX)

IF(MACOSX)
    target_link_libraries( _moose 
        moosepython  
        "-Wl,-all_load"
        ${MOOSE_LIBRARIES}
        ${STATIC_LIBRARIES}
        )
    target_link_libraries(_moose
        ${SYSTEM_SHARED_LIBS}
        )
ELSE(MACOSX)
    target_link_libraries(
        _moose 
        moosepython  
        "-Wl,--whole-archive"
        ${MOOSE_LIBRARIES}
        ${STATIC_LIBRARIES}
        "-Wl,--no-whole-archive"
        ${SYSTEM_SHARED_LIBS}
        )
ENDIF(MACOSX)

    add_custom_command(TARGET _moose POST_BUILD
        COMMAND ${PYTHON_EXECUTABLE} setup.py sdist --dist-dir
        ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${PROJECT_PYTHON_PATH}
        COMMENT "Generating moose source distribution using setuptools"
        VERBATIM
        )

IF(WITH_DOC)
    # Also generate the documents
    FIND_PROGRAM(Sphinx REQUIRED)
    add_custom_command(TARGET _moose
        POST_BUILD
        COMMAND make html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Docs/user/py
        COMMENT "Building user doc using python-sphinx"
        VERBATIM
        )
endif(WITH_DOC)
endif(WITH_PYTHON)

######################### CYMOOSE #############################################
option(BUILD_CYMOOSE "Build python extension using Cython. Development version." OFF)
if(BUILD_CYMOOSE)
    message("++ Building cython extension of MOOSE. Unstable and experimental.")
    add_library(cymoose SHARED cymoose/cymoose_methods.cxx)
    set_target_properties(cymoose PROPERTIES COMPILE_DEFINITIONS "CYMOOSE")
    if(DEBUG)
        add_definitions("-g -Wall")
    endif()

    set_target_properties(cymoose PROPERTIES 
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cymoose"
            )

IF(MACOSX)
    target_link_libraries(cymoose 
        "-Wl,-all_load"
        ${MOOSE_LIBRARIES}
        ${LIBRARIES}
        "-Wl,-noall_load"
        ${SYSTEM_LIBRARIES}
        )
ELSE(MACOSX)
    target_link_libraries(cymoose 
        "-Wl,--whole-archive"
        ${MOOSE_LIBRARIES}
        ${LIBRARIES}
        "-Wl,--no-whole-archive"
        ${SYSTEM_LIBRARIES}
        )
ENDIF(MACOSX)

endif(BUILD_CYMOOSE)

## Moose documentation
option(WITH_DOC "Build documentation using python-sphinx and doxygen" OFF)
if(WITH_DOC)
    FIND_PACKAGE(Sphinx REQUIRED)
    MESSAGE(STATUS "Build documentation.")
    set(USER_DOC_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Docs/user/py/_build/html)
    set(DEVELOPER_DOC_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Docs/developer/html)
    ADD_CUSTOM_TARGET(docs ALL
        COMMAND ./docgen
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating html doc using sphinx and doxygen"
        )

endif(WITH_DOC)

######################### INSTALL ##############################################

install(TARGETS moose.bin
    DESTINATION bin
    )

install(TARGETS libmoose
    DESTINATION lib
    )

install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/moose
    DESTINATION bin
    )

if(WITH_PYTHON)
    SET(MOOSE_SRC_TAR ${CMAKE_CURRENT_BINARY_DIR}/moose-${MOOSE_VERSION}.tar.gz)

    install(CODE 
        "
        execute_process(
            COMMAND tar xvf ${MOOSE_SRC_TAR} -C /tmp
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            )
        execute_process(
            COMMAND ${PYTHON_EXECUTABLE} setup.py install --prefix=${CMAKE_INSTALL_PREFIX}
            WORKING_DIRECTORY /tmp/moose-${MOOSE_VERSION}
            )
        "
        )

endif(WITH_PYTHON)

if(WITH_DOC)
    message(STATUS "Installing moose doc")
    install(DIRECTORY ${USER_DOC_OUTPUT_DIR}
        DESTINATION share/doc/moose
        )

    install(DIRECTORY ${DEVELOPER_DOC_OUTPUT_DIR}
        DESTINATION share/doc/moose/developer
        )
endif()

## TODO: Use python setup.py install to make sure that it installs with MacPort
## and any Unix-like system.
##if(BUILD_MOOGLI)
##    install(CODE
##        "
##        execute_process(COMMAND easy_install
##            --prefix ${CMAKE_INSTALL_PREFIX}
##            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
##            )
##        "
##        )
##endif(BUILD_MOOGLI)
    

############################ CTEST ######################################

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_moose_ctest.cmake)

############################ CPACK ######################################
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_moose_cpack.cmake)
