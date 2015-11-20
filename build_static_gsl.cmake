MESSAGE("++ Building local GSL")

SET(GSL_SRC_DIR ${CMAKE_SOURCE_DIR}/dependencies/gsl-2.0)
SET(GSL_INSTALL_DIR ${CMAKE_BINARY_DIR}/__gsl_install)
FILE(MAKE_DIRECTORY ${GSL_INSTALL_DIR})
MESSAGE("++ GSL_SRC_DIR := ${GSL_SRC_DIR}")

set(STATIC_GSL_LIBRARY ${GSL_INSTALL_DIR}/lib/libgsl.a)
set(STATIC_GSLBLAS_LIBRARY ${GSL_INSTALL_DIR}/lib/libgslcblas.a)

SET(GSL_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/__gsl_build__)
FILE(MAKE_DIRECTORY ${GSL_BUILD_DIR})

# NOTE: Set WORKING_DIRECTORY to GSL_SRC_DIRECTORY else the build might fail
# because of missing texinfo and texi2html package.
ADD_CUSTOM_COMMAND(
    OUTPUT ${STATIC_GSLBLAS_LIBRARY} ${STATIC_GSL_LIBRARY}
    COMMAND ${GSL_SRC_DIR}/configure --prefix=${GSL_INSTALL_DIR} --with-pic --enable-static --disable-shared
    COMMAND $(MAKE) 
    COMMAND $(MAKE) install
    WORKING_DIRECTORY ${GSL_SRC_DIR}
    VERBATIM  # needed to handle escape characters.
    )

ADD_CUSTOM_TARGET(_libgsl ALL 
    DEPENDS ${STATIC_GSL_LIBRARY} ${STATIC_GSLBLAS_LIBRARY}
    )

set(GSL_INCLUDE_DIR ${GSL_INSTALL_DIR}/include)
message("+++ GSL static libs ${STATIC_GSL_LIBRARY} ${STATIC_GSLBLAS_LIBRARY}")
# Next time moose module must be able to find it.
set(ENV{GSL_HOME} ${GSL_INSTALL_DIR})
LIST(APPEND ALL_STATIC_LIBS ${STATIC_GSLBLAS_LIBRARY} ${STATIC_GSL_LIBRARY})

## Ofcourse, moose-full depends on it.
add_dependencies(_moose_static_dependencies _libgsl)
