# Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>

# find headers files
file(GLOB_RECURSE headers_FlyClient FlyClient/*.h)
file(GLOB_RECURSE headers_FlyFactory FlyFactory/*.h)
file(GLOB_RECURSE headers_FlyServer FlyServer/*.h)

# find cpp files
file(GLOB_RECURSE sources_FlyClient FlyClient/*.cpp)
file(GLOB_RECURSE sources_FlyFactory FlyFactory/*.cpp)
file(GLOB_RECURSE sources_FlyServer FlyServer/*.cpp)
file(GLOB sources_localdir *.cpp)

# find resources
file(GLOB_RECURSE resources_Server ${CMAKE_CURRENT_SOURCE_DIR}/resources/*.qrc)

# set headers files
set(server_HEADERS
  ${headers_FlyClient}
  ${headers_FlyFactory}
  ${headers_FlyServer}
  ${headers_localdir}
)

# set cpp files
set(server_SOURCES
  ${sources_FlyClient}
  ${sources_FlyFactory}
  ${sources_FlyServer}
  ${sources_localdir}
)

# set resources
set(server_RESOURCES
  ${resources_Server}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/FlyGroup
  ${CMAKE_SOURCE_DIR}/src/shared/FlyNetwork
  ${CMAKE_SOURCE_DIR}/src/shared/FlyTask
  ${CMAKE_SOURCE_DIR}/src/shared/FlyUser
  ${CMAKE_SOURCE_DIR}/src/shared/FlyWork
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/FlyClient
  ${CMAKE_CURRENT_SOURCE_DIR}/FlyFactory
  ${CMAKE_CURRENT_SOURCE_DIR}/FlyServer
  ${QT_INCLUDE_DIR}
  ${QT_QTNETWORK_INCLUDE_DIR}
)

qt4_add_resources(QRC_SERVER ${server_RESOURCES})
qt4_wrap_cpp(MOC_SERVER ${server_HEADERS})

add_executable(server ${server_HEADERS} ${server_SOURCES} ${MOC_SERVER} ${QRC_SERVER})
add_dependencies(server revision.h)
target_link_libraries(server shared ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARY})

if( UNIX )
  install(TARGETS server DESTINATION bin)
elseif( WIN32 )
  install(TARGETS server DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
