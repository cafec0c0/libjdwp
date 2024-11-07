set(VIRTUAL_MACHINE_UNIT_TEST_SOURCES
        01_version.c
        02_classes_by_signature.c
        03_all_classes.c
        04_all_threads.c
        05_top_level_thread_groups.c
        06_dispose.c
        07_id_sizes.c
        08_suspend.c
        09_resume.c
        10_exit.c
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
        22_all_modules.c
)

set(VIRTUAL_MACHINE_UNIT_TEST_PREFIX "unit_test_virtual_machine_01")

foreach (file ${VIRTUAL_MACHINE_UNIT_TEST_SOURCES})
    get_filename_component(STRIPPED_FILE ${file} NAME_WE)
    set(EXE_NAME "${VIRTUAL_MACHINE_UNIT_TEST_PREFIX}_${STRIPPED_FILE}")
    add_executable(${EXE_NAME} ${file})
    target_include_directories(${EXE_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${EXE_NAME} PRIVATE cmocka jdwp)
    add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})
endforeach ()