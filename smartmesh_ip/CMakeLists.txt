CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(LIB_SMART_MESH smartmesh_ip)

SET(LIB_SMART_MESH_SOURCES 
  smartmesh_ip.c
  smartmesh_ip_mote.c
  smartmesh_ip_mngr.c
  )

ADD_LIBRARY(${LIB_SMART_MESH} SHARED ${LIB_SMART_MESH_SOURCES})
