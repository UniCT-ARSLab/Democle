add_executable (test_coins test_coins.cpp)

target_link_libraries (test_coins democle)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options( test_coins PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries( test_coins "${CMAKE_THREAD_LIBS_INIT}")
endif()

#install (TARGETS test RUNTIME DESTINATION bin)
