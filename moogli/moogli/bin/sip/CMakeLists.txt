cmake_minimum_required(VERSION 2.8)
FILE(GLOB SIP_SRC *.sip)

SET(GENERATED_SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_mooglicmodule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliColorBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliFrustum.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliMeshView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliQuat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliMoogliShape.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliMoogliSphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliVec2f.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliVec2i.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliVec3f.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliVec3i.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliVec4f.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliVec4i.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sip_moogliViewer.cpp
    )

SET(SIP_INCLUDES ${PYQT4_SIP_DIR})

MESSAGE("++ Using SIP includes: ${SIP_INCLUDES}")
add_custom_command(OUTPUT ${GENERATED_SRCS}
    COMMAND ${SIP_EXECUTABLE} -c ${CMAKE_CURRENT_SOURCE_DIR} -I ${SIP_INCLUDES} 
        ${SIP_EXTRA_OPTIONS}
        ${CMAKE_CURRENT_SOURCE_DIR}/moogli.sip
    DEPENDS ${SIP_SRC}
    COMMENT "Processing ${CMAKE_CURRENT_SOURCE_DIR}/moogli.sip"
    VERBATIM
    )

ADD_LIBRARY(sip  ${GENERATED_SRCS})
