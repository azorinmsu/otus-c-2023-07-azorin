cmake_minimum_required(VERSION 3.18)

project(main)

add_executable(${PROJECT_NAME} main.c parser.c file_reader.c)

target_link_libraries(${PROJECT_NAME} ${CONFUSE_LINK_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CONFUSE_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${CONFUSE_CFLAGS_OTHER})
