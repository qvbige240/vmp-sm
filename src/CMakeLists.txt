
include_directories (${PROJECT_SOURCE_DIR}/src/) 

include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/vmp)
include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/module)
include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/module/web)

set(SMM_SOURCES
module/dao/dao_server_node.c
module/dao/dao_server_node.h
module/model/model_server.h
module/web/api/api_demo.h
module/web/api/api_node_info.h
module/web/api/api_node_list.h
module/web/api/api_typedef.h
module/web/web_demo.c
module/web/web_handler.h
module/web/web_node_info.c
module/web/web_node_list.c
module/web/web_register.h
module/web/web_server.c
module/web/web_server.h
smm/demo.c
smm/smm.c
smm/smm_core.c
smm/smm_core.h
smm/smm_demo.c
smm/smm_demo.h
smm/smm_node_info.c
smm/smm_node_info.h
smm/smm.h
smm/smm_typedef.h
vmp/vmp.h
vmp/vmp_log.h
vmp/vmp_typedef.h
vmp/vpk_darray.c
vmp/vpk_darray.h
vmp/vpk_darray_list.c
vmp/vpk_darray_list.h
vmp/vpk_list.h
vmp/vpk_typedef.h

)

SET(SMM_INCLUDES smm.h smm_typedef.h smm_core.h vmp/vmp.h vmp/vmp_log.h vmp/vmp_typedef.h)
MESSAGE(STATUS "FOUND include files ${SMM_INCLUDES}")

add_library(smp STATIC ${SMM_SOURCES})

if (${ZLOG_ENABLED})
target_link_libraries(smp ${ZLOG_LIBRARIES})
endif (${ZLOG_ENABLED})

if (${JSON_ENABLED})
target_link_libraries(smp ${JSON_LIBRARIES})
endif (${JSON_ENABLED})

install(FILES ${SMM_INCLUDES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )
