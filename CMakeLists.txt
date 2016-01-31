cmake_minimum_required(VERSION 3.3)
project(Libparc)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if( UNIX )
  link_libraries(m)
endif( UNIX )

include( CTest )
include( version )
include( detectCacheSize )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

link_directories($ENV{CCNX_DEPENDENCIES}/lib)
include_directories($ENV{CCNX_DEPENDENCIES}/include)
set(OPENSSL_ROOT_DIR $ENV{CCNX_DEPENDENCIES})

find_package( LongBow REQUIRED )
include_directories(${LONGBOW_INCLUDE_DIRS})

find_package( LibEvent REQUIRED )
include_directories(${LIBEVENT_INCLUDE_DIRS})

find_package ( Threads REQUIRED )

find_package ( OpenSSL REQUIRED )

include_directories(${PROJECT_BINARY_DIR}/parc ${PROJECT_SOURCE_DIR})

find_package( Doxygen )

set(PARC_BIN_LIBRARIES
  parc
  ${LONGBOW_LIBRARIES}
  ${LIBEVENT_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  )

macro(AddTest testFile)
  add_executable(${ARGV0} ${ARGV0}.c)
  target_link_libraries(${ARGV0} ${PARC_BIN_LIBRARIES})
  add_test(${ARGV0} ${ARGV0})
endmacro(AddTest)



add_subdirectory(parc)
#add_subdirectory(documentation)
