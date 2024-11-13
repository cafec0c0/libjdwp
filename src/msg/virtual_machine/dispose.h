
#ifndef DISPOSE_H
#define DISPOSE_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError dispose_serialize(uint8_t **buf, size_t *len, void *command,
                               JdwpCommandType type, IdSizes *id_sizes,
                               uint32_t id);

JdwpLibError dispose_deserialize(DeserializationContext *data);
void dispose_free(JdwpReply *reply);

#endif // DISPOSE_H
