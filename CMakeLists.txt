cmake_minimum_required(VERSION 3.10.0)
project(Cypher VERSION 0.1.0 LANGUAGES C)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(Cypher main.c)

