add_executable (ping_pong ping_pong.cpp)

include_directories (../lib)
link_directories (${CMAKE_SOURCE_DIR}/lib)

target_link_libraries (ping_pong democle)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options( ping_pong PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries( ping_pong "${CMAKE_THREAD_LIBS_INIT}")
endif()

#install (TARGETS sieve RUNTIME DESTINATION bin)
