# Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>

add_subdirectory(shared)
if(FLYWORK_CLIENT)
  add_subdirectory(client)
endif(FLYWORK_CLIENT)

if(FLYWORK_SERVER)
  add_subdirectory(server)
endif(FLYWORK_SERVER)
