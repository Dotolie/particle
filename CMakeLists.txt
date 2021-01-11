CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)

PROJECT (particle)


ADD_DEFINITIONS(-Wall -O2 -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC)

SET(BINDIR  ${CMAKE_SOURCE_DIR}/bin)
SET(AUTHOR_NAME "ywkim")

SET ( PROJECT_VERSION_MAJOR 0 )
SET ( PROJECT_VERSION_MINOR 1 )
CONFIGURE_FILE ( ${CMAKE_SOURCE_DIR}/version.h.in  ${CMAKE_SOURCE_DIR}/inc/version.h )

INCLUDE_DIRECTORIES ( 	
	${CMAKE_SOURCE_DIR}/inc

)

#LINK_DIRECTORIES ( ${CMAKE_SOURCE_DIR}/lib )

FILE(GLOB SRCS
	src/main.c
	src/sgp30.c
        src/sensirion_common.c
        src/sensirion_hw_i2c_implementation.c
       	src/cm1109.c
      	src/pm5000.c
       	src/htu31d.c
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread rt zmq stdc++ )

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})

