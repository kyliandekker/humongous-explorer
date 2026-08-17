project(scripttest)

# Gather all scripttest files
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE FILE_SOURCES ${CMAKE_SOURCE_DIR}/../src/file/*.cpp)
file(GLOB_RECURSE CORE_SOURCES ${CMAKE_SOURCE_DIR}/../src/core/*.cpp)

# scripttest executable
add_executable(scripttest ${HEADERS} ${ICON} ${SOURCES} ${FILE_SOURCES} ${CORE_SOURCES})

source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})
source_group("Resource Files" FILES ${ICON})

# Preprocessor definitions
target_compile_definitions(scripttest PRIVATE
    _EDITOR
    "$<$<CONFIG:${DEBUG}>:${PREDEFINITIONS_EDITOR_DEBUG}>"
    "$<$<CONFIG:${RELEASE}>:${PREDEFINITIONS_EDITOR_RELEASE}>"
)

# Include directories
target_include_directories(scripttest PUBLIC 
	${CMAKE_SOURCE_DIR}/src 
	${CMAKE_SOURCE_DIR}/../src
)

# C++ standard
set_target_properties(scripttest PROPERTIES CXX_STANDARD 20)

# Link directories
target_link_directories(scripttest PRIVATE "${CMAKE_SOURCE_DIR}/../$<CONFIG>")