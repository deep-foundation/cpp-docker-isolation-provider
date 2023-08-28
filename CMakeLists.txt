cmake_minimum_required(VERSION 3.12)
project(cpp-docker-isolation-provider)

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

INCLUDE_DIRECTORIES(
  "${CMAKE_SOURCE_DIR}/python"
)

# Find Boost package
find_package(Boost REQUIRED COMPONENTS python)

# Add the Boost include directory
include_directories(${Boost_INCLUDE_DIRS})

find_package(PythonLibs REQUIRED)

add_executable(cpp-docker-isolation-provider
        httplib.h
        docker-isolation-provider.cpp
)


# Link against Boost libraries
TARGET_LINK_LIBRARIES(cpp-docker-isolation-provider ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})


set_target_properties(cpp-docker-isolation-provider
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  PREFIX ""
)