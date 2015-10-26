cmake_minimum_required(VERSION 3.3)
project(Bubulles)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Agar)
add_executable(Bubulles ${SOURCE_FILES}/main.cpp
        ${SOURCE_FILES}/Cell.cpp
        ${SOURCE_FILES}/Cell.h
        ${SOURCE_FILES}/Entity.cpp
        ${SOURCE_FILES}/Entity.h
        ${SOURCE_FILES}/Food.cpp
        ${SOURCE_FILES}/Food.h
        ${SOURCE_FILES}/Global.h
        ${SOURCE_FILES}/Player.cpp
        ${SOURCE_FILES}/Player.h)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Bubulles ${SFML_LIBRARIES})
endif()