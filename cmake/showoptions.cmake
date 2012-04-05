# Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>

# output generic information about the core and buildtype chosen
message("")
message("* FlyWork rev. hash      : ${rev_hash}")
message("* FlyWork rev. date      : ${rev_date}")
if( UNIX )
  message("* FlyWork buildtype      : ${CMAKE_BUILD_TYPE}")
endif()
message("")

# output information about installation-directories and locations

message("* Install application to : ${CMAKE_INSTALL_PREFIX}")
if( UNIX )
  message("* Install libraries to   : ${LIBSDIR}")
  message("* Install configs to     : ${CONF_DIR}")
endif()
message("")

# Show infomation about the options selected during configuration

if( FLYWORK_SERVER )
  message("* Build FlyWork Server   : Yes (default)")
else()
  message("* Build FlyWork Server   : No")
endif()

if( FLYWORK_CLIENT )
  message("* Build FlyWork Client   : Yes (default)")
else()
  message("* Build FlyWork Client   : No")
endif()

if( WITH_WARNINGS )
  message("* Show all warnings      : Yes")
else()
  message("* Show compile-warnings  : No  (default)")
endif()

if( WITH_COREDEBUG )
  message("* Use coreside debug     : Yes")
  add_definitions(-DFLYCORE_DEBUG)
else()
  message("* Use coreside debug     : No  (default)")
endif()

message("")
