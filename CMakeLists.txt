cmake_minimum_required(VERSION 3.23)

project(ming
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Slightly advanced containers for C++"
    HOMEPAGE_URL "https://github.com/poyea/ming")

if(MSVC)
    add_compile_options(/permissive- /W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

file(GLOB EXAMPLE_SRC_FILES ${PROJECT_SOURCE_DIR}/examples/*.cpp)
foreach(file ${EXAMPLE_SRC_FILES})
    file(GLOB FILE_SRC ${file})
    get_filename_component(FILE_NAME ${file} NAME_WLE)
    add_executable(example_${FILE_NAME} ${FILE_SRC})
    target_link_libraries(example_${FILE_NAME} ${PROJECT_NAME})
endforeach()

option(BENCHMARKS "Build small benchmarks" OFF)
if(BENCHMARKS)
    add_subdirectory(bench)
endif()
if(TESTS)
    enable_testing()
    message("-- Building Tests")
    add_subdirectory(tests)
endif()
