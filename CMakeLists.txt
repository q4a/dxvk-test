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

if (NOT WIN32)
    option(USE_OLD_DXVK "Use old DXVK Native for DXGI" OFF)
    if (USE_OLD_DXVK)
        message("Using DXVK Native for D3D11 API")
        set(DXVK_TYPE "native")
        set(DXVK_GIT_REPOSITORY "https://github.com/q4a/dxvk-native")
        set(DXVK_GIT_TAG "master")
        set(DXVK_EXTRA_PARAM "")
    else()
        message("Using DXVK master for DXGI")
        set(DXVK_TYPE "upstream")
        set(DXVK_GIT_REPOSITORY "https://github.com/doitsujin/dxvk")
        set(DXVK_GIT_TAG "master")
        set(DXVK_EXTRA_PARAM "-Denable_d3d8=false")
    endif()
    include(ExternalProject)
    set(DXVK_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/dxvk-src-${DXVK_TYPE}")
    set(DXVK_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/dxvk-bin-${DXVK_TYPE}")
    set(DIRECTX_INCLUDE_DIRS
        "${DXVK_SOURCE_DIR}/include/native/directx"
        "${DXVK_SOURCE_DIR}/include/native/windows"
    )
    set(DIRECTX_LIBS
        "${DXVK_BINARY_DIR}/src/d3d11/libdxvk_d3d11.so"
        "${DXVK_BINARY_DIR}/src/dxgi/libdxvk_dxgi.so"
    )
    ExternalProject_Add(dxvk
        GIT_REPOSITORY    ${DXVK_GIT_REPOSITORY}
        GIT_TAG           ${DXVK_GIT_TAG}
        GIT_SHALLOW       OFF
        BUILD_ALWAYS      OFF
        SOURCE_DIR        ${DXVK_SOURCE_DIR}
        BINARY_DIR        ${DXVK_BINARY_DIR}
        CONFIGURE_HANDLED_BY_BUILD ON
        CONFIGURE_COMMAND meson setup ../dxvk-src-${DXVK_TYPE} --buildtype=debug ${DXVK_EXTRA_PARAM} -Denable_d3d9=false -Denable_d3d10=false -Denable_d3d11=true -Denable_dxgi=true
        BUILD_COMMAND     ninja
        INSTALL_COMMAND   ""
        BUILD_BYPRODUCTS  ${DIRECTX_LIBS}
    )
    find_package(SDL2 CONFIG REQUIRED)
endif()

set(SRC_FILES source.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

if(NOT WIN32)
add_dependencies(${PROJECT_NAME} dxvk)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${DIRECTX_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    ${DIRECTX_LIBS}
)
endif()
