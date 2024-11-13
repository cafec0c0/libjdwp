//
// Created by rogue on 07/11/24.
//

#ifndef GET_VALUES_H
#define GET_VALUES_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError ref_type_get_values_serialize(uint8_t **buf, size_t *len,
                                           void *command, JdwpCommandType type,
                                           IdSizes *id_sizes, uint32_t id);
JdwpLibError ref_type_get_values_deserialize(DeserializationContext *ctx);
void ref_type_get_values_free(JdwpReply *reply);
#endif // GET_VALUES_H
