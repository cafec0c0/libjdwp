set(REFERENCE_TYPE_E2E_TEST_SOURCES
        01_signature.c
        02_class_loader.c
        03_modifiers.c
        04_fields.c
        05_methods.c
)

set(REFERENCE_TYPE_E2E_TEST_PREFIX "e2e_test_reference_type_02")

foreach (file ${REFERENCE_TYPE_E2E_TEST_SOURCES})
    get_filename_component(STRIPPED_FILE ${file} NAME_WE)
    set(EXE_NAME "${REFERENCE_TYPE_E2E_TEST_PREFIX}_${STRIPPED_FILE}")
    add_executable(${EXE_NAME} ${file})
    target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/test/e2e/)
    target_link_libraries(${EXE_NAME} PRIVATE cmocka jdwp util)
    add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})

    if (JDWP_ENABLE_FIXTURES)
        set_tests_properties(${EXE_NAME} PROPERTIES FIXTURES_REQUIRED JVM)
    endif ()
endforeach ()