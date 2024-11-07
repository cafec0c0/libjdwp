if (JDWP_ENABLE_FIXTURES)
    message(STATUS "Fixtures are enabled. A valid JDK and runtime will be required.")
    add_subdirectory(fixtures)
endif ()

add_subdirectory(util)
add_subdirectory(msg)
