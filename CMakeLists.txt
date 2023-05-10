cmake_minimum_required(VERSION 3.16)
project(LogiXpr VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif()

include_directories(include)

set(SOURCES
    src/main.cpp
    src/parser.cpp
    src/ast.cpp
    src/equivalence_engine.cpp
    src/proof_search.cpp
)

set(HEADERS
    include/ast.h
    include/parser.h
    include/equivalence_engine.h
    include/proof_search.h
    include/logic_laws.h
)

add_executable(logixpr ${SOURCES} ${HEADERS})

# Google Test setup
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Create test executable
set(TEST_SOURCES
    tests/test_proof_search.cpp
    tests/test_logic_laws.cpp
    src/parser.cpp
    src/ast.cpp
    src/equivalence_engine.cpp
    src/proof_search.cpp
)

add_executable(logixpr_test ${TEST_SOURCES})
target_link_libraries(logixpr_test gtest gtest_main)

# Add tests
add_test(NAME logixpr_test COMMAND logixpr_test)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    
    add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()