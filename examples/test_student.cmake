add_executable (test_student test_student.cpp)

target_link_libraries (test_student democle)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options( test_student PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries( test_student "${CMAKE_THREAD_LIBS_INIT}")
endif()

#install (TARGETS test RUNTIME DESTINATION bin)
