
include_directories (${PROJECT_SOURCE_DIR}/src/) 

include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/vmp) 

set(SMM_SOURCES
smm/demo.c
smm/smm.c
smm/smm_core.c
smm/smm_core.h
smm/smm.h
smm/smm_typedef.h
vmp/vmp.h
vmp/vmp_log.h
vmp/vmp_typedef.h
)

SET(SMM_INCLUDES smm.h smm_typedef.h smm_core.h vmp/vmp.h vmp/vmp_log.h vmp/vmp_typedef.h)
MESSAGE(STATUS "FOUND include files ${SMM_INCLUDES}")

add_library(smp STATIC ${SMM_SOURCES})

if (${ZLOG_ENABLED})
target_link_libraries(smp ${ZLOG_LIBRARIES})
endif (${ZLOG_ENABLED})


install(FILES ${SMM_INCLUDES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

add_subdirectory(app)
