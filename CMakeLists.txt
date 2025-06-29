cmake_minimum_required(VERSION 3.16)

project(queued_thread LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES_FILES
    queuedthread.h
)

set(SOURCES_FILES
    queuedthread.cpp
)

add_executable(queued_thread main.cpp ${INCLUDES_FILES} ${SOURCES_FILES})

include(GNUInstallDirs)
install(TARGETS queued_thread
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
