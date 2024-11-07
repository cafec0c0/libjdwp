#ifndef SUSPEND_H
#define SUSPEND_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError suspend_serialize(uint8_t **buf, size_t *len, void *command,
                               JdwpCommandType type, IdSizes *id_sizes,
                               uint32_t id);

JdwpLibError suspend_deserialize(JdwpReply **reply, size_t *len, uint8_t *bytes,
                                 JdwpCommandType type, IdSizes *id_sizes);
void suspend_free(JdwpReply *reply);
#endif // SUSPEND_H
