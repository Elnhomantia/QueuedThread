cmake_minimum_required(VERSION 3.16)

project(QueuedThread LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES_FILES
    include/queuedthread.h
)

set(SOURCES_FILES
    src/queuedthread.cpp
)

add_library(QueuedThread STATIC ${INCLUDES_FILES} ${SOURCES_FILES})

target_include_directories(QueuedThread PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/Elth/queuedthread>
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Elth/queuedthread)

install(TARGETS QueuedThread
    EXPORT QueuedThreadTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(EXPORT QueuedThreadTargets
    FILE QueuedThreadTargets.cmake
    NAMESPACE Elth::
    DESTINATION lib/cmake/QueuedThread
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/QueuedThreadConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/QueuedThreadConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QueuedThreadConfig.cmake"
    INSTALL_DESTINATION lib/cmake/QueuedThread
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/QueuedThreadConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/QueuedThreadConfigVersion.cmake"
    DESTINATION lib/cmake/QueuedThread
)
