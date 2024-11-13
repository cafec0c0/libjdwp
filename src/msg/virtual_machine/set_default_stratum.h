//
// Created by rogue on 07/11/24.
//

#ifndef SET_DEFAULT_STRATUM_H
#define SET_DEFAULT_STRATUM_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError set_default_stratum_serialize(uint8_t **buf, size_t *len,
                                           void *command, JdwpCommandType type,
                                           IdSizes *id_sizes, uint32_t id);
JdwpLibError set_default_stratum_deserialize(DeserializationContext *ctx);
void set_default_stratum_free(JdwpReply *reply);

#endif // SET_DEFAULT_STRATUM_H
