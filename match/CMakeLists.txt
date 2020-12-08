cmake_minimum_required(VERSION 3.1)

project(match)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS, "-Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "My multi config types" FORCE)
elseif(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RELEASE)
endif()

find_package(Boost 1.58.0 REQUIRED program_options filesystem)
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
file(GLOB SRCS "./*.cpp" "./*.h")
add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
