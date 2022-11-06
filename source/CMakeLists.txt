cmake_minimum_required(VERSION 3.0.0)
project(kiritool VERSION 1.1.1 LANGUAGES C)

set(SOURCES main.c includes/cJSON/cJSON.c includes/cJSON/cJSON.h)
add_executable(kiritool ${SOURCES})

find_package(CURL CONFIG REQUIRED)
# find_package(cJSON CONFIG REQUIRED)

# libcurl
target_include_directories(kiritool PUBLIC "C:/vcpkg/installed/x64-windows")
target_link_libraries(kiritool PUBLIC CURL::libcurl)
# cJSON
target_include_directories(kiritool PRIVATE includes)
