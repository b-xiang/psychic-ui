if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/../external/glfw/.git")
  message(FATAL_ERROR "Psychic UI dependency missing! Use \"git submodule update --init --recursive\" to download dependencies.")
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL " " FORCE)
set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)
set(GLFW_USE_CHDIR OFF CACHE BOOL " " FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../external/glfw" "${CMAKE_CURRENT_BINARY_DIR}/build")

set(GLFW_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/../external/glfw/include PARENT_SCOPE)
set(GLFW_LIBRARIES ${CMAKE_CURRENT_BINARY_DIR}/build/src/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX} PARENT_SCOPE)
