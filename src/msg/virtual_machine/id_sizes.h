#ifndef ID_SIZES_H
#define ID_SIZES_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError id_sizes_serialize(uint8_t **buf, size_t *len, void *command,
                                JdwpCommandType type, IdSizes *id_sizes,
                                uint32_t id);

JdwpLibError id_sizes_deserialize(JdwpReply **reply, size_t *len,
                                  uint8_t *bytes, JdwpCommandType type,
                                  IdSizes *id_sizes);
void id_sizes_free(JdwpReply *reply);

#endif // ID_SIZES_H
