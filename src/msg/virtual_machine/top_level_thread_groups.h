#ifndef TOP_LEVEL_THREAD_GROUPS_H
#define TOP_LEVEL_THREAD_GROUPS_H

#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError top_level_thread_groups_serialize(uint8_t **buf, size_t *len,
                                               void *command,
                                               JdwpCommandType type,
                                               IdSizes *id_sizes, uint32_t id);

JdwpLibError top_level_thread_groups_deserialize(JdwpReply **reply, size_t *len,
                                                 uint8_t *bytes,
                                                 JdwpCommandType type,
                                                 IdSizes *id_sizes);
void top_level_thread_groups_free(JdwpReply *reply);

#endif // TOP_LEVEL_THREAD_GROUPS_H
