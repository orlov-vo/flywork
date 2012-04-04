# Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>

# Set projectname (must be done AFTER setting configurationtypes)
set(PROJECT FlyWork)
project(${PROJECT})

# CMake policies (can not be handled elsewhere)
cmake_minimum_required (VERSION 2.6)

# build in Release-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "Release")
endif()

# basic packagesearching and setup (further support will be needed, this is a preliminary release!)
find_package (Qt4 REQUIRED)

# add Qt libs to project
include (${QT_USE_FILE})

add_definitions(${QT_DEFINITIONS})

# set default buildoptions and print them
include(cmake/options.cmake)

# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)

# print out the results before continuing
include(cmake/showoptions.cmake)

# add core sources
add_subdirectory(src)

 
if (MSVC)
	add_definitions (/W3)
elseif (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions (-Wall -pedantic)
else ()
	message ("Unknown compiler")
endif ()
 
