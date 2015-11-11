MESSAGE("++ Building local GSL")

SET(GSL_SRC_DIR ${CMAKE_SOURCE_DIR}/dependencies/gsl-2.0)
SET(GSL_INSTALL_DIR ${CMAKE_BINARY_DIR}/_libgsl)
FILE(MAKE_DIRECTORY ${GSL_INSTALL_DIR})
MESSAGE("++ GSL_SRC_DIR := ${GSL_SRC_DIR}")

set(STATIC_GSL_LIBRARY ${GSL_INSTALL_DIR}/lib/libgsl.a)
set(STATIC_GSLBLAS_LIBRARY ${GSL_INSTALL_DIR}/lib/libgslcblas.a)

ADD_CUSTOM_TARGET(_libgsl ALL 
    DEPENDS __static_gsl_library_was_run__ 
    )

## DO NOT use cmake on GSL. IT does not build gsl-config which is used by 
## cmake to find gsl headers. Nothing will be found for local installtion
## if cmake is used on gsl.
ADD_CUSTOM_COMMAND(OUTPUT __static_gsl_library_was_run__
    COMMAND ${GSL_SRC_DIR}/configure --prefix=${GSL_INSTALL_DIR} --with-pic --enable-static --disable-shared
    COMMAND $(MAKE) 
    COMMAND $(MAKE) install
    WORKING_DIRECTORY ${GSL_INSTALL_DIR}
    VERBATIM  # needed to handle escape characters.
    )

set(GSL_INCLUDE_DIR ${GSL_INSTALL_DIR}/include)
message("+++ GSL static libs ${STATIC_GSL_LIBRARY} ${STATIC_GSLBLAS_LIBRARY}")
# Next time moose module must be able to find it.
set(ENV{GSL_HOME} ${GSL_INSTALL_DIR})
LIST(APPEND ALL_STATIC_LIBS ${STATIC_GSLBLAS_LIBRARY} ${STATIC_GSL_LIBRARY})

## Ofcourse, moose-full depends on it.
add_dependencies(_moose_static_dependencies _libgsl)
