

include_directories (../lib/democle)
link_directories (${CMAKE_SOURCE_DIR}/lib/democle)

include(test.cmake)
include(test_coins.cmake)
include(test_student.cmake)
include(test_max.cmake)
include(test_timer.cmake)
include(test_url.cmake)
include(sieve.cmake)
include(sieve_remote.cmake)
include(ping_pong.cmake)
