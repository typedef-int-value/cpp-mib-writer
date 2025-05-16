cmake_minimum_required (VERSION 3.5)
project (mib-writer C CXX)

# build type
if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# enable the compiler options for gnucxx
if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options (-std=c++11 -fpermissive -Wno-unused-result -Wno-narrowing -Werror=return-type)
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# deps: boost
if(CMAKE_COMPILER_IS_GNUCXX)
# linux
include_directories("/mnt/d/linux/boost_1_70_0/")
link_directories("/mnt/d/linux/boost_1_70_0/stage/lib")
else()
# windows
include_directories("D:/Git/cpptest/deps/boost_1_80_0")
link_directories("D:/Git/cpptest/deps/boost_1_80_0/stage/win_lib")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE MIB_WRITER_SOURCES CONFIGURE_DEPENDS 
  snmp/*.*
  snmp/detail/*.*
  snmp/detail/syntax/*.*
)

add_library(mib-writer ${MIB_WRITER_SOURCES})
set_target_properties(mib-writer PROPERTIES LINKER_LANGUAGE CXX)

if(true)
add_executable (mib-writer-example mib_writer_example.cpp)
endif()