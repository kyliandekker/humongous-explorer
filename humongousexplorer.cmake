project(humongousexplorer)

# Gather all humongousexplorer files
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# humongousexplorer executable
add_executable(humongousexplorer WIN32 ${HEADERS} ${ICON} ${SOURCES})

source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})
source_group("Resource Files" FILES ${ICON})

# Preprocessor definitions
target_compile_definitions(humongousexplorer PRIVATE
    _EDITOR
    "$<$<CONFIG:${DEBUG}>:${PREDEFINITIONS_EDITOR_DEBUG}>"
    "$<$<CONFIG:${RELEASE}>:${PREDEFINITIONS_EDITOR_RELEASE}>"
)

# Include directories
target_include_directories(humongousexplorer PUBLIC 
	${CMAKE_SOURCE_DIR}/src 
	${CMAKE_SOURCE_DIR}/external
)

# C++ standard
set_target_properties(humongousexplorer PROPERTIES CXX_STANDARD 20)

# Link libraries
target_link_libraries(humongousexplorer PRIVATE Shcore.lib dxgi.lib d3dcompiler.lib Shlwapi.lib d3d11.lib dxgi.lib d3dcompiler.lib winmm.lib imgui)

# Link directories
target_link_directories(humongousexplorer PRIVATE "${CMAKE_SOURCE_DIR}/../$<CONFIG>")