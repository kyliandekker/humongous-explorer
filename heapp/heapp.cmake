# Gather all heapp files
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# heapp executable
add_executable(heapp WIN32 ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

# C++ standard
set_target_properties(heapp PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Include directories
target_include_directories(heapp PUBLIC 
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/external
	${CMAKE_SOURCE_DIR}/external/imgui
	${CMAKE_CURRENT_LIST_DIR}/src
)

# Link libraries
target_link_libraries(heapp PRIVATE dxgi.lib d3dcompiler.lib d3d11.lib winmm.lib Shcore.lib helib imgui)

# Link directories
target_link_directories(heapp PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../$<CONFIG>")