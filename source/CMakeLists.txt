set(PROJECT_NAME kiritool)
set(SOURCES main.c includes/cJSON/cJSON.c includes/cJSON/cJSON.h)

cmake_minimum_required(VERSION 3.0.0)
project(${PROJECT_NAME} VERSION 1.1.1 LANGUAGES C)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(CURL CONFIG REQUIRED)

# libcurl
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)
# cJSON
target_include_directories(${PROJECT_NAME} PRIVATE includes)
