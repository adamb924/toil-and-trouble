cmake_minimum_required(VERSION 3.16)

project(toil-and-trouble LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_POSTFIX d)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Xml)

add_library(toil-and-trouble SHARED
    toilandtrouble.cpp 
    adjudicator/preferredstemidcriterion.cpp 
    parser.cpp 
    adjudicator/abstractadjudicationcriterion.cpp 
    adjudicator/parsingadjudicator.cpp 
    adjudicator/preferredmorphemesequencecriterion.cpp 
    adjudicator/preferredstemcriterion.cpp 
)

# Use generator expressions to add the "d" suffix in Debug mode
find_library(MORTALENGINE_LIB_RELEASE NAMES mortalengine)
find_library(MORTALENGINE_LIB_DEBUG NAMES mortalengined)

message(STATUS "MORTALENGINE_LIB_RELEASE = ${MORTALENGINE_LIB_RELEASE}")
message(STATUS "MORTALENGINE_LIB_DEBUG = ${MORTALENGINE_LIB_DEBUG}")

# Use generator expressions to add the "d" suffix in Debug mode
find_library(KNOTTYENTRAILS_LIB_RELEASE NAMES knotty-entrails)
find_library(KNOTTYENTRAILS_LIB_DEBUG NAMES knotty-entrailsd)

message(STATUS "KNOTTYENTRAILS_LIB_RELEASE = ${KNOTTYENTRAILS_LIB_RELEASE}")
message(STATUS "KNOTTYENTRAILS_LIB_DEBUG = ${KNOTTYENTRAILS_LIB_DEBUG}")

target_include_directories(toil-and-trouble
    PUBLIC
    "."
    ${CMAKE_PREFIX_PATH}/include/mortal-engine
    ${CMAKE_PREFIX_PATH}/include/knotty-entrails
)

target_link_libraries(toil-and-trouble PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Xml
    $<$<CONFIG:Debug>:${MORTALENGINE_LIB_DEBUG}>
    $<$<NOT:$<CONFIG:Debug>>:${MORTALENGINE_LIB_RELEASE}> 
    $<$<CONFIG:Debug>:${KNOTTYENTRAILS_LIB_DEBUG}>
    $<$<NOT:$<CONFIG:Debug>>:${KNOTTYENTRAILS_LIB_RELEASE}> 
)

target_compile_definitions(toil-and-trouble PRIVATE TOILANDTROUBLE_LIBRARY)

install(TARGETS toil-and-trouble
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

# this installs some non-public headers, but so be it
install(DIRECTORY "${CMAKE_SOURCE_DIR}"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN ".git" EXCLUDE
)
