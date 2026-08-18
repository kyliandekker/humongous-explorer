# Gather all hetest files
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# hetest executable
add_executable(hetest ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

# C++ standard
set_target_properties(hetest PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Include directories
target_include_directories(hetest PUBLIC 
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/external
	${CMAKE_CURRENT_LIST_DIR}/src
)


# Link libraries
target_link_libraries(hetest PRIVATE helib)

# Link directories
target_link_directories(hetest PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../$<CONFIG>")