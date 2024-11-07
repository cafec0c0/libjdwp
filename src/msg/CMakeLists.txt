target_sources(jdwp PRIVATE
        reply.c reply.h
        command.c command.h
        client.c client.h
)

add_subdirectory(virtual_machine)