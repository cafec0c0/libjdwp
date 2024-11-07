set(VIRTUAL_MACHINE_E2E_TEST_SOURCES
        01_version.c
        02_classes_by_signature.c
        03_all_classes.c
        04_all_threads.c
        05_top_level_thread_groups.c
        06_dispose.c
        07_id_sizes.c
        08_suspend.c
        09_resume.c
        11_create_string.c
        12_capabilities.c
        13_class_paths.c
        14_dispose_objects.c
        15_hold_events.c
        16_release_events.c
        17_capabilities_new.c
        18_redefine_classes.c
        19_set_default_stratum.c
        20_all_classes_with_generic.c
        21_instance_counts.c
        22_modules.c

        # This must be run last as it kills the VM
        10_exit.c
)

set(VIRTUAL_MACHINE_E2E_TEST_PREFIX "e2e_test_virtual_machine_01")

foreach (file ${VIRTUAL_MACHINE_E2E_TEST_SOURCES})
    get_filename_component(STRIPPED_FILE ${file} NAME_WE)
    set(EXE_NAME "${VIRTUAL_MACHINE_E2E_TEST_PREFIX}_${STRIPPED_FILE}")
    add_executable(${EXE_NAME} ${file})
    target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/test/e2e/)
    target_link_libraries(${EXE_NAME} PRIVATE cmocka jdwp util)
    add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})

    if (JDWP_ENABLE_FIXTURES)
        set_tests_properties(${EXE_NAME} PROPERTIES FIXTURES_REQUIRED JVM)
    endif ()
endforeach ()