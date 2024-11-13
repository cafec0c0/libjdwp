//
// Created by rogue on 07/11/24.
//

#ifndef INSTANCE_COUNTS_H
#define INSTANCE_COUNTS_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError instance_counts_serialize(uint8_t **buf, size_t *len,
                                       void *command, JdwpCommandType type,
                                       IdSizes *id_sizes, uint32_t id);
JdwpLibError instance_counts_deserialize(DeserializationContext *ctx);
void instance_counts_free(JdwpReply *reply);

#endif // INSTANCE_COUNTS_H
