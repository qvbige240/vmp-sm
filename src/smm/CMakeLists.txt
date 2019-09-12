# the lib itself. Two versions, static and shared.
# add_subdirectory(handlers)
include_directories (${PROJECT_SOURCE_DIR}/src/)


#SET(INCLUDES smm.h smm_typedef.h smm_core.h)
#set(SOURCES smm.c smm_core.c)

#set(LIBRARIES)
#LIST(APPEND LIBRARIES rt)

set(SMM smm.c smm_core.c)
add_library(smp_smm STATIC ${SMM})

target_link_libraries(onion_static ${LIBRARIES})

## optional add sources and libraries depeding on features


# library dependencies
if (${SQLITE3_ENABLED})
	LIST(APPEND SOURCES sessions_sqlite3.c)
	LIST(APPEND INCLUDES sessions_sqlite3.h)
endif(${SQLITE3_ENABLED})
if (${REDIS_ENABLED})
        LIST(APPEND SOURCES sessions_redis.c)
        LIST(APPEND INCLUDES sessions_redis.h)
endif(${REDIS_ENABLED})


if (${XML2_ENABLED})
	MESSAGE(STATUS "Compiling WebDAV support")
	include_directories(${LIBXML2_INCLUDE_DIR})
	LIST(APPEND SOURCES handlers/webdav.c)
	LIST(APPEND LIBRARIES ${LIBXML2_LIBRARIES})
endif (${XML2_ENABLED})



IF (${CMAKE_BUILD_TYPE} MATCHES "Fast")
 add_custom_command(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/all-onion.c
   COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && cat ${SOURCES}> ${CMAKE_CURRENT_BINARY_DIR}/all-onion.c
   DEPENDS ${SOURCES}
   )
 include_directories(${CMAKE_CURRENT_SOURCE_DIR})
 include_directories(${CMAKE_CURRENT_SOURCE_DIR}/handlers/)
 add_library(onion SHARED ${CMAKE_CURRENT_BINARY_DIR}/all-onion.c)
 add_library(onion_static STATIC ${CMAKE_CURRENT_BINARY_DIR}/all-onion.c)
ELSE (${CMAKE_BUILD_TYPE} MATCHES "Fast")
 add_library(onion SHARED ${SOURCES})
 add_library(onion_static STATIC  ${SOURCES})
ENDIF (${CMAKE_BUILD_TYPE} MATCHES "Fast")

target_link_libraries(onion ${LIBRARIES})
target_link_libraries(onion_static ${LIBRARIES})

# Only if png by the moment, maybe later other functionalities there.
if (PNG_ENABLED OR JPEG_ENABLED)
add_subdirectory(extras)
endif (PNG_ENABLED OR JPEG_ENABLED)

MESSAGE(STATUS "Found include files ${INCLUDES_ONION}")

SET_TARGET_PROPERTIES(onion onion_static
	PROPERTIES
		SOVERSION 0
		VERSION ${ONION_VERSION}
	)

configure_file(version.h.in version.h)

install(FILES ${INCLUDES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/version.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS onion onion_static EXPORT onion DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (${ARM})
elseif (${ARM})
install(EXPORT onion DESTINATION share/cmake)
endif (${ARM})
