get_filename_component(PROJECT_NAME_STR ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME_STR})

cmake_minimum_required(VERSION 2.8)
project(${PROJECT_NAME_STR})

#----------------------------------------
# set compiler
#----------------------------------------
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(-std=c++14)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions(-Wall -std=c++14)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

#----------------------------------------
# set Boost
#----------------------------------------
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.60.0 COMPONENTS serialization)
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()

#if (WIN32 AND __COMPILER_GNU)
#    # mingw-gcc fails to link boost::thread
#    add_definitions(-DBOOST_THREAD_USE_LIB)
#endif (WIN32 AND __COMPILER_GNU)

#----------------------------------------
# set Threads
#----------------------------------------
find_package(Threads REQUIRED)

#----------------------------------------
# Application
#----------------------------------------
aux_source_directory(./src SRC_LIST)

# Headers
file(GLOB HEADERS_LIST "./src/*.h" "./src/*.hpp" "./di/*.hpp")

add_executable(${PROJECT_NAME} main.cpp ${SRC_LIST} ${HEADERS_LIST})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})


if(Boost_FOUND)
    target_link_libraries(${PROJECT_NAME})
endif()

#----------------------------------------
# Test
#----------------------------------------
enable_testing(true)

set(CATCH_SINGLE_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/catch)
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_tests)

include_directories(${CATCH_SINGLE_INCLUDE_DIRECTORY} ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
file(GLOB_RECURSE TEST_HEADERS_FILES ${PROJECT_SOURCE_DIR}/tests/*.hpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES}  ${TEST_HEADERS_FILES} ${SRC_LIST})
target_link_libraries(${PROJECT_TEST_NAME} ${Boost_LIBRARIES})

add_test(unit_tests ${PROJECT_TEST_NAME})
