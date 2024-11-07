# Create JAR
find_package(Java)
include(UseJava)
add_jar(simple-server SimpleServer.java ENTRY_POINT SimpleServer)

# Scripts for starting and stopping the JVM
configure_file(start-server.sh ${CMAKE_CURRENT_BINARY_DIR}/start-server.sh)
configure_file(stop-server.sh ${CMAKE_CURRENT_BINARY_DIR}/stop-server.sh COPYONLY)

# Create fixture commands
add_test(NAME startServer COMMAND sh start-server.sh)
add_test(NAME stopServer COMMAND sh stop-server.sh)

# Create JVM fixture
set_tests_properties(startServer PROPERTIES FIXTURES_SETUP JVM)
set_tests_properties(stopServer PROPERTIES FIXTURES_CLEANUP JVM)