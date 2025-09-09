add_executable (test_timer test_timer.cpp)

include_directories (../lib)
link_directories (${CMAKE_SOURCE_DIR}/lib)

target_link_libraries (test_timer democle)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options( test_timer PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries( test_timer "${CMAKE_THREAD_LIBS_INIT}")
endif()

#install (TARGETS test_timer RUNTIME DESTINATION bin)
