# Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>

# find headers files
file(GLOB_RECURSE headers_FlyGroup FlyGroup/*.h)
file(GLOB_RECURSE headers_FlyNetwork FlyNetwork/*.h)
file(GLOB_RECURSE headers_FlyTask FlyTask/*.h)
file(GLOB_RECURSE headers_FlyUser FlyUser/*.h)
file(GLOB_RECURSE headers_FlyWork FlyWork/*.h)
file(GLOB headers_localdir *.h)

# find cpp files
file(GLOB_RECURSE sources_FlyGroup FlyGroup/*.cpp)
file(GLOB_RECURSE sources_FlyNetwork FlyNetwork/*.cpp)
file(GLOB_RECURSE sources_FlyTask FlyTask/*.cpp)
file(GLOB_RECURSE sources_FlyUser FlyUser/*.cpp)
file(GLOB_RECURSE sources_FlyWork FlyWork/*.cpp)
file(GLOB sources_localdir *.cpp)

set(shared_HEADERS
  ${headers_FlyGroup}
  ${headers_FlyNetwork}
  ${headers_FlyTask}
  ${headers_FlyUser}
  ${headers_FlyWork}
  ${headers_localdir}
)

set(shared_SOURCES
  ${sources_FlyGroup}
  ${sources_FlyNetwork}
  ${sources_FlyTask}
  ${sources_FlyUser}
  ${sources_FlyWork}
  ${sources_localdir}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QT_INCLUDE_DIR}
  ${QT_QTNETWORK_INCLUDE_DIR}
)

qt4_wrap_cpp(MOC_SHARED ${shared_HEADERS})

add_library(shared STATIC ${shared_HEADERS} ${shared_SOURCES} ${MOC_SHARED})
target_link_libraries(shared ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARY})
