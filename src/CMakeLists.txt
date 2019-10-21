
include_directories (${PROJECT_SOURCE_DIR}/src/) 

include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/vmp)
include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/rpc)
include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/module)
include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/module/web)

set(SMM_SOURCES
module/cache/cache.c
module/cache/cache.h
module/cache/cache_table.c
module/cache/cache_table.h
module/cache/cache_typedef.h
module/dao/dao_server_node.c
module/dao/dao_server_node.h
module/model/model_server.h
module/module_support.h
module/support/globals.c
module/support/globals.h
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
vmp/vmp_object.h
vmp/vmp_typedef.h
vmp/vpk_darray.c
vmp/vpk_darray.h
vmp/vpk_darray_list.c
vmp/vpk_darray_list.h
vmp/vpk_list.h
vmp/vpk_typedef.h

)

if (${PBC_RPC_ENABLED})
  LIST(APPEND SMM_SOURCES rpc/clnt_demo.c
    rpc/clnt_demo.h
    rpc/clnt_node_state.c
    rpc/clnt_node_state.h
    rpc/clnt_vmon.c
    rpc/clnt_vmon.h
    rpc/pbc/list.h
    rpc/pbc/pbrpc-clnt.c
    rpc/pbc/pbrpc-clnt.h
    rpc/pbc/pbrpc.pb-c.c
    rpc/pbc/pbrpc.pb-c.h
    rpc/pbc/pbrpc-svc.c
    rpc/pbc/pbrpc-svc.h
    rpc/pbrpc_client.c
    rpc/pbrpc_client.h
    rpc/pbrpc_server.c
    rpc/pbrpc_server.h
    rpc/rpc_clnt.c
    rpc/rpc_clnt.h
    rpc/rpc_svc.c
    rpc/rpc_svc.h
    rpc/rpc_typedef.h
    rpc/service-demo.c
    rpc/service-demo.h
    rpc/service-node-state.c
    rpc/service-node-state.h
    rpc/service/pbc-demo-clnt.c
    rpc/service/pbc-demo.pb-c.c
    rpc/service/pbc-demo.pb-c.h
    rpc/service/pbc-demo-service.c
    rpc/service/pbc-nstatus.pb-c.c
    rpc/service/pbc-nstatus.pb-c.h
    rpc/service/pbc-registry.pb-c.c
    rpc/service/pbc-registry.pb-c.h
    rpc/service/pbc-vmon.pb-c.c
    rpc/service/pbc-vmon.pb-c.h
    rpc/service-server-registry.c
    rpc/service-server-registry.h
    rpc/service-typedef.h
    rpc/service-vmon.c
    rpc/service-vmon.h
    smm/smm_rpc_task.c
    smm/smm_rpc_task.h)
#	LIST(APPEND INCLUDES sessions_sqlite3.h)
endif(${PBC_RPC_ENABLED})

SET(SMM_INCLUDES smm.h smm_typedef.h smm_core.h vmp/vmp.h vmp/vmp_log.h vmp/vmp_typedef.h)
MESSAGE(STATUS "FOUND include files ${SMM_INCLUDES}")

add_library(smp STATIC ${SMM_SOURCES})

if (${ZLOG_ENABLED})
target_link_libraries(smp ${ZLOG_LIBRARIES})
endif (${ZLOG_ENABLED})

if (${JSON_ENABLED})
target_link_libraries(smp ${JSON_LIBRARIES})
endif (${JSON_ENABLED})

if (${PBC_RPC_ENABLED})
target_link_libraries(smp ${PROTOBUFC_LIBRARIES})
endif (${PBC_RPC_ENABLED})

install(FILES ${SMM_INCLUDES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )
