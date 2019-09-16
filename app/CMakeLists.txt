include_directories (${PROJECT_SOURCE_DIR}/src/)
include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/smm)
include_directories (${PROJECT_SOURCE_DIR}/src/smp/src/vmp)

add_executable(smp-demo smp-demo.c)
target_link_libraries(smp-demo smp onion_static)
