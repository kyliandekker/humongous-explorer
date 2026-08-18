# Gather all helib sources
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# Engine library
add_library(helib STATIC ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(helib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# C++ standard
set_target_properties(helib PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)