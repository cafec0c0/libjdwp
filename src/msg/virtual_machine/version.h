#ifndef VERSION_H
#define VERSION_H

#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError version_serialize(uint8_t **buf, size_t *len, void *command,
                               JdwpCommandType type, IdSizes *id_sizes,
                               uint32_t id);

JdwpLibError version_deserialize(JdwpReply **reply, size_t *len, uint8_t *bytes,
                                 JdwpCommandType type, IdSizes *id_sizes);
void version_free(JdwpReply *reply);

#endif // VERSION_H
