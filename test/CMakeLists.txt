if (BUILD_TESTING AND JDWP_UNIT_TEST)
    add_subdirectory(unit)
endif ()

if (BUILD_TESTING AND JDWP_E2E_TEST)
    add_subdirectory(e2e)
endif ()