#ifndef CREATE_STRING_H
#define CREATE_STRING_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError create_string_serialize(uint8_t **buf, size_t *len, void *command,
                                     JdwpCommandType type, IdSizes *id_sizes,
                                     uint32_t id);
JdwpLibError create_string_deserialize(DeserializationContext *ctx);
void create_string_free(JdwpReply *reply);
#endif // CREATE_STRING_H
