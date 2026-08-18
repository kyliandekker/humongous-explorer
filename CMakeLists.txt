cmake_minimum_required(VERSION 3.20)

project(HumongousExplorer)

if (MSVC)
    add_compile_options(/MP)
endif()

# Configurations
set(RELEASE "Release")
set(DEBUG "Debug")
set(CMAKE_CONFIGURATION_TYPES "${RELEASE};${DEBUG}" CACHE STRING "" FORCE)
set(CMAKE_BUILD_TYPE Debug)

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE STRING "Debug linker flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:REF /OPT:ICF" CACHE STRING "Release linker flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")

# Global settings
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMakePredefinedTargets)

add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)

# Third-party libraries

# IMGUI
file(GLOB_RECURSE IMGUI_SOURCES ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/external/imgui)
set_target_properties(imgui PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON FOLDER "ThirdParty")
source_group(TREE ${CMAKE_SOURCE_DIR}/external/imgui PREFIX "" FILES ${IMGUI_SOURCES})

# Include modules
include(helib/helib.cmake)
include(hetest/hetest.cmake)
include(heconsole/heconsole.cmake)
include(heapp/heapp.cmake)