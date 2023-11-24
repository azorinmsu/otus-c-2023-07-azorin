cmake_minimum_required(VERSION 3.18)

project(daemon)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CONFUSE REQUIRED libconfuse)

add_executable(${PROJECT_NAME} daemon.c)

target_link_libraries(${PROJECT_NAME} ${CONFUSE_LINK_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CONFUSE_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${CONFUSE_CFLAGS_OTHER})

