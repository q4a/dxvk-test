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
    option(DISABLE_DXVK "Disable DXVK calls (use only VKD3D)" OFF)
    option(USE_OLD_DXVK "Use old DXVK Native for DXGI" OFF)
    if (DISABLE_DXVK)
        add_definitions(-DDISABLE_DXVK)
    else()
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
    add_dependencies(${PROJECT_NAME} dxvk)
    endif()

    find_package(SDL2 CONFIG REQUIRED)

    include(ProcessorCount)
    ProcessorCount(N)
    message("ProcessorCount = ${N}")
    set(VKD3D_CONFIG "./configure")
    #list(APPEND VKD3D_CONFIG "CPPFLAGS=-O3 -DNDEBUG -DVKD3D_NO_TRACE_MESSAGES")
    #list(APPEND VKD3D_CONFIG   "CFLAGS=-O3 -DNDEBUG -DVKD3D_NO_TRACE_MESSAGES")
    set(VKD3D_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/vkd3d-src")
    ExternalProject_Add(vkd3d
        GIT_REPOSITORY    https://gitlab.winehq.org/wine/vkd3d.git
        GIT_TAG           vkd3d-1.17
        GIT_SHALLOW       ON
        BUILD_ALWAYS      OFF
        SOURCE_DIR        ${VKD3D_SOURCE_DIR}
        BUILD_IN_SOURCE   ON
        CONFIGURE_HANDLED_BY_BUILD ON
        # result comman will be: ./configure "CPPFLAGS=-O3 -DNDEBUG -DVKD3D_NO_TRACE_MESSAGES" "CFLAGS=-O3 -DNDEBUG -DVKD3D_NO_TRACE_MESSAGES"
        CONFIGURE_COMMAND ./autogen.sh && ${CMAKE_COMMAND} -E env ${VKD3D_CONFIG}
        BUILD_COMMAND     make -j${N}
        BUILD_BYPRODUCTS  ${VKD3D_SOURCE_DIR}/.libs/libvkd3d-utils.a
        INSTALL_COMMAND   ""
    )
    ExternalProject_Get_property(vkd3d SOURCE_DIR BINARY_DIR)
    set(DIRECTX_INCLUDE_DIRS ${DIRECTX_INCLUDE_DIRS} "${VKD3D_SOURCE_DIR}/include")
    set(DIRECTX_LIBS ${DIRECTX_LIBS} "${VKD3D_SOURCE_DIR}/.libs/libvkd3d-utils.a")
    add_dependencies(${PROJECT_NAME} vkd3d)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${DIRECTX_INCLUDE_DIRS}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2::SDL2
        ${DIRECTX_LIBS}
    )
endif()
