add_executable (test_max test_max.cpp)

include_directories (../lib)
link_directories (${CMAKE_SOURCE_DIR}/lib)

target_link_libraries (test_max democle)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options( test_max PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries( test_max "${CMAKE_THREAD_LIBS_INIT}")
endif()

#install (TARGETS test_max RUNTIME DESTINATION bin)
