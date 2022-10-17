cmake_minimum_required(VERSION 3.22)
project(theta2)

include(FetchContent)

set(BUILD_SHARED_LIBS FALSE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE STRING "don't update every time")
set(CPPWINRT_REQUIRED_COMPILER_FLAGS "/std:c++latest /await")

option(BUILD_DOCS "Build doxygen documentation" OFF)

add_subdirectory(deps/fmod)
add_subdirectory(deps/eos)

include("cmake/common_deps.cmake")

if (BUILD_DOCS)
    include("cmake/build_docs.cmake")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include("cmake/platform_macos.cmake")
elseif (DEFINED ENV{VITASDK})
    include("cmake/platform_vita.cmake")
else()
    include("cmake/platform_windows.cmake")
endif()

target_link_libraries(theta2 PRIVATE ${game_libs})
target_compile_definitions(theta2 PRIVATE _CRT_SECURE_NO_WARNINGS=1)

# copy game data only in release
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(theta2 PRIVATE DEBUG=1)
else()
    add_custom_command(TARGET theta2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:theta2>/data
    COMMENT "Copying assets")
endif()