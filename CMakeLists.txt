cmake_minimum_required(VERSION 4.3.2)

project(ming
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Slightly advanced containers for C++"
    HOMEPAGE_URL "https://github.com/poyea/ming")

if(PROJECT_IS_TOP_LEVEL AND NOT BENCHMARKS)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy-22")
    if(CLANG_TIDY_EXE)
        message("clang-tidy-22 is detected...")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
        set(CMAKE_CXX_CLANG_TIDY_EXPORT_FIXES_DIR "${CMAKE_BINARY_DIR}/__clang-tidy-fixes")
    else()
        message(WARNING "clang-tidy not found, continuing without linting.")
    endif()
endif()

if(MSVC)
    add_compile_options(/permissive- /W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
