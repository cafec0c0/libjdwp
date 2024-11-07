#!/usr/bin/env sh

@Java_JAVA_EXECUTABLE@ -agentlib:jdwp=transport=dt_socket,server=y,address=8000,suspend=n -jar @CMAKE_CURRENT_BINARY_DIR@/simple-server.jar 2>/dev/null 1>/dev/null &
echo "$!" > simple-server.pid
# wait for the port to become available
sleep 1