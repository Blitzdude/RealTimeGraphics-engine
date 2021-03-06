cmake_minimum_required(VERSION 3.0)
project(RTG_proj)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(RTG_proj/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(RTG_proj/Vendor/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(RTG_proj/Vendor/bullet)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(RTG_proj/Headers/
					RTG_proj/Headers/tests/
                    RTG_proj/Vendor/assimp/include/
                    RTG_proj/Vendor/bullet/src/
                    RTG_proj/Vendor/glad/include/
                    RTG_proj/Vendor/glfw/include/
                    RTG_proj/Vendor/glm/
					RTG_proj/Vendor/imgui/
                    RTG_proj/Vendor/stb/)

file(GLOB VENDORS_SOURCES RTG_proj/Vendor/glad/src/glad.c
						  RTG_proj/Vendor/imgui/*.cpp)
file(GLOB PROJECT_HEADERS RTG_proj/Headers/*.hpp)
file(GLOB PROJECT_SOURCES RTG_proj/Sources/*.cpp)
file(GLOB PROJECT_SHADERS RTG_proj/Shaders/*.comp
                          RTG_proj/Shaders/*.frag
                          RTG_proj/Shaders/*.geom
                          RTG_proj/Shaders/*.vert
						  RTG_proj/Shaders/*.shader)
file(GLOB TEST_FILES      RTG_proj/Sources/tests/*.cpp
						  RTG_proj/Headers/tests/*.hpp)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})
source_group("Tests"   FILES ${TEST_FILES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES} ${TEST_FILES})
target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
