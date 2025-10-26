cmake_minimum_required(VERSION 3.18)
project(dxvk-test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True) # Require the specified standard
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

### Set up output paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)

set(SRC_FILES source.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

if (NOT WIN32)
    include(ExternalProject)
    include(ProcessorCount)
    ProcessorCount(N)
    message("ProcessorCount = ${N}")
    set(VKD3D_CONFIG "./configure")
    #list(APPEND VKD3D_CONFIG "CPPFLAGS=-O3 -DNDEBUG -DVKD3D_NO_TRACE_MESSAGES")
    #list(APPEND VKD3D_CONFIG   "CFLAGS=-O3 -DNDEBUG -DVKD3D_NO_TRACE_MESSAGES")
    set(VKD3D_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/vkd3d-src")
    ExternalProject_Add(vkd3d
        GIT_REPOSITORY    https://gitlab.winehq.org/wine/vkd3d
        GIT_TAG           vkd3d-1.17
        GIT_SHALLOW       ON
        BUILD_ALWAYS      OFF
        SOURCE_DIR        ${VKD3D_SOURCE_DIR}
        BUILD_IN_SOURCE   ON
        CONFIGURE_HANDLED_BY_BUILD ON
        # result comman will be: ./configure "CPPFLAGS=-O3 -DNDEBUG -DVKD3D_NO_TRACE_MESSAGES" "CFLAGS=-O3 -DNDEBUG -DVKD3D_NO_TRACE_MESSAGES"
        CONFIGURE_COMMAND ./autogen.sh && ${CMAKE_COMMAND} -E env ${VKD3D_CONFIG}
        BUILD_COMMAND     make -j${N}
        BUILD_BYPRODUCTS  ${VKD3D_SOURCE_DIR}/.libs/libvkd3d.so ${VKD3D_SOURCE_DIR}/.libs/libvkd3d-shader.so ${VKD3D_SOURCE_DIR}/.libs/libvkd3d-utils.so
        INSTALL_COMMAND   ""
    )
    set(DIRECTX_INCLUDE_DIRS "${VKD3D_SOURCE_DIR}/include")
    set(DIRECTX_LIBS
        "${VKD3D_SOURCE_DIR}/.libs/libvkd3d.so"
        "${VKD3D_SOURCE_DIR}/.libs/libvkd3d-shader.so"
        "${VKD3D_SOURCE_DIR}/.libs/libvkd3d-utils.so"
    )
    #target_link_options(${PROJECT_NAME} PRIVATE -v -flto)
    add_dependencies(${PROJECT_NAME} vkd3d)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${DIRECTX_INCLUDE_DIRS}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${DIRECTX_LIBS}
    )
endif()
