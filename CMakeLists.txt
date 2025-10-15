cmake_minimum_required(VERSION 3.15)
project(SkiaPlot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(BUILD_EXAMPLES "Build example programs" ON)

# Find Skia
# Users can set SKIA_DIR to point to their Skia installation
if(DEFINED ENV{SKIA_DIR})
    set(SKIA_DIR $ENV{SKIA_DIR})
endif()

if(SKIA_DIR)
    message(STATUS "Using Skia from: ${SKIA_DIR}")
    set(SKIA_INCLUDE_DIRS ${SKIA_DIR}/include)
    find_library(SKIA_LIBRARY 
        NAMES skia 
        PATHS ${SKIA_DIR}/out/Release ${SKIA_DIR}/out/Debug
        NO_DEFAULT_PATH)
    
    if(SKIA_LIBRARY)
        message(STATUS "Found Skia library: ${SKIA_LIBRARY}")
    else()
        message(WARNING "Skia library not found. Please build Skia first.")
    endif()
else()
    message(STATUS "SKIA_DIR not set. Please set SKIA_DIR environment variable or CMake variable.")
    message(STATUS "You can download and build Skia from: https://skia.org/docs/user/build/")
endif()

# SkiaPlot library
add_library(skiaplot INTERFACE)
target_include_directories(skiaplot INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(SKIA_INCLUDE_DIRS)
    target_include_directories(skiaplot INTERFACE ${SKIA_INCLUDE_DIRS})
endif()

if(SKIA_LIBRARY)
    target_link_libraries(skiaplot INTERFACE ${SKIA_LIBRARY})
    # Add common system libraries that Skia depends on
    if(UNIX AND NOT APPLE)
        target_link_libraries(skiaplot INTERFACE 
            pthread
            fontconfig
            freetype
            GL
        )
    elseif(APPLE)
        target_link_libraries(skiaplot INTERFACE
            "-framework CoreFoundation"
            "-framework CoreGraphics"
            "-framework CoreText"
            "-framework OpenGL"
        )
    endif()
endif()

# Build examples
if(BUILD_EXAMPLES AND SKIA_LIBRARY)
    add_subdirectory(examples)
endif()
