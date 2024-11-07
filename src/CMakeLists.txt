add_subdirectory(msg)

target_sources(jdwp PRIVATE
        serde.c serde.h
)

target_include_directories(jdwp PRIVATE .)