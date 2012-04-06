# Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>

# find headers files
file(GLOB_RECURSE headers_AdminWindow AdminWindow/*.h)
file(GLOB_RECURSE headers_AuthDialog AuthDialog/*.h)
file(GLOB_RECURSE headers_MainWindow MainWindow/*.h)
file(GLOB_RECURSE headers_WaitWidget WaitWidget/*.h)
file(GLOB_RECURSE headers_WorkWidget WorkWidget/*.h)
file(GLOB headers_localdir *.h)

# find cpp files
file(GLOB_RECURSE sources_AdminWindow AdminWindow/*.cpp)
file(GLOB_RECURSE sources_AuthDialog AuthDialog/*.cpp)
file(GLOB_RECURSE sources_MainWindow MainWindow/*.cpp)
file(GLOB_RECURSE sources_WaitWidget WaitWidget/*.cpp)
file(GLOB_RECURSE sources_WorkWidget WorkWidget/*.cpp)
file(GLOB sources_localdir *.cpp)

# find forms files
file(GLOB_RECURSE forms_AdminWindow AdminWindow/*.ui)
file(GLOB_RECURSE forms_AuthDialog AuthDialog/*.ui)
file(GLOB_RECURSE forms_MainWindow MainWindow/*.ui)
file(GLOB_RECURSE forms_WaitWidget WaitWidget/*.ui)
file(GLOB_RECURSE forms_WorkWidget WorkWidget/*.ui)

# find resources
file(GLOB_RECURSE resources_Client ${CMAKE_SOURCE_DIR}/resources/*.qrc)

# set headers files
set(client_HEADERS
  ${headers_AdminWindow}
  ${headers_AuthDialog}
  ${headers_MainWindow}
  ${headers_WaitWidget}
  ${headers_WorkWidget}
  ${headers_localdir}
)

# set cpp files
set(client_SOURCES
  ${sources_AdminWindow}
  ${sources_AuthDialog}
  ${sources_MainWindow}
  ${sources_WaitWidget}
  ${sources_WorkWidget}
  ${sources_localdir}
)

# set forms
set(client_FORMS
  ${forms_AdminWindow}
  ${forms_AuthDialog}
  ${forms_MainWindow}
  ${forms_WaitWidget}
  ${forms_WorkWidget}
)

# set resources
set(client_RESOURCES
  ${resources_Client}
)

qt4_add_resources(QRC_CLIENT ${client_RESOURCES})
qt4_wrap_ui(UI_CLIENT ${client_FORMS})
qt4_wrap_cpp(MOC_CLIENT ${client_HEADERS})

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/FlyAnswer
  ${CMAKE_SOURCE_DIR}/src/shared/FlyGroup
  ${CMAKE_SOURCE_DIR}/src/shared/FlyNetwork
  ${CMAKE_SOURCE_DIR}/src/shared/FlyTask
  ${CMAKE_SOURCE_DIR}/src/shared/FlyUser
  ${CMAKE_SOURCE_DIR}/src/shared/FlyWork
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/AdminWindow
  ${CMAKE_CURRENT_SOURCE_DIR}/AuthDialog
  ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow
  ${CMAKE_CURRENT_SOURCE_DIR}/WaitWidget
  ${CMAKE_CURRENT_SOURCE_DIR}/WorkWidget
  ${QT_INCLUDE_DIR}
  ${QT_QTNETWORK_INCLUDE_DIR}
)

add_executable(client WIN32 ${client_HEADERS} ${client_SOURCES} ${MOC_CLIENT} ${QRC_CLIENT} ${UI_CLIENT})
add_dependencies(client revision.h)
target_link_libraries(client shared ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY} ${QT_QTNETWORK_LIBRARY})

if( UNIX )
  install(TARGETS client DESTINATION bin)
elseif( WIN32 )
  install(TARGETS client DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
