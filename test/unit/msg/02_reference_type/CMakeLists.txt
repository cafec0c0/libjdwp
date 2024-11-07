set(REFERENCE_TYPE_UNIT_TEST_SOURCES
        01_signature.c
        02_class_loader.c
        03_modifiers.c
        04_fields.c
        05_methods.c
)

set(REFERENCE_TYPE_UNIT_TEST_PREFIX "unit_test_reference_type_02")

foreach (file ${REFERENCE_TYPE_UNIT_TEST_SOURCES})
    get_filename_component(STRIPPED_FILE ${file} NAME_WE)
    set(EXE_NAME "${REFERENCE_TYPE_UNIT_TEST_PREFIX}_${STRIPPED_FILE}")
    add_executable(${EXE_NAME} ${file})
    target_include_directories(${EXE_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${EXE_NAME} PRIVATE cmocka jdwp)
    add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})
endforeach ()