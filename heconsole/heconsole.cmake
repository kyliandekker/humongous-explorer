# Gather all heconsole files
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# heconsole executable
add_executable(heconsole ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

# C++ standard
set_target_properties(heconsole PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Include directories
target_include_directories(heconsole PUBLIC 
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/external
	${CMAKE_CURRENT_LIST_DIR}/src
)


# Link libraries
target_link_libraries(heconsole PRIVATE helib)

# Link directories
target_link_directories(heconsole PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../$<CONFIG>")