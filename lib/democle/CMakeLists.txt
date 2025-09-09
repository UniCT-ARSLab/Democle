add_library (democle SHARED
  agent.cpp
  agent.h
  condition.cpp
  condition.h
  context.cpp
  context.h
  debug.h
  democle.cpp
  democle.h
  context_collector.cpp
  context_collector.h
  democle_types.cpp
  democle_types.h
  engine.cpp
  engine.h
  errors.h
  flexi_type.cpp
  flexi_type.h
  goal.cpp
  goal.h
  intention.h
  messaging.cpp
  plan.cpp
  plan.h
  predicates.h
  sensor.cpp
  sensor.h
  sensor_lib.cpp
  sensor_lib.h
  protocol.h
  tcp_protocol.cpp
  binary_stream.h
  packet.h
  packet.cpp
  url.cpp
  url.h
  tsqueue.h
  )

install(TARGETS democle DESTINATION lib)

file (GLOB HEADERS
  agent.h
  condition.h
  context.h
  debug.h
  democle.h
  democle_types.h
  engine.h
  errors.h
  flexi_type.h
  goal.h
  intention.h
  plan.h
  predicates.h
  sensor.h
  sensor_lib.h
  tsqueue.h
  protocol.h
  binary_stream.h
  packet.h
  url.h
  )
install(FILES ${HEADERS} DESTINATION include)

#add_compile_options(-g)

include_directories (${CMAKE_SOURCE_DIR})
