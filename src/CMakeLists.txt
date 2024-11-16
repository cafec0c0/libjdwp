add_subdirectory(msg)

target_sources(jdwp PRIVATE
        serde.c serde.h
)

target_compile_options(jdwp PRIVATE -Wall -Wextra -Werror -Wpedantic)
target_include_directories(jdwp PRIVATE .)