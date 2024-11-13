//
// Created by rogue on 07/11/24.
//

#ifndef FIELDS_H
#define FIELDS_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError ref_type_fields_serialize(uint8_t **buf, size_t *len,
                                       void *command, JdwpCommandType type,
                                       IdSizes *id_sizes, uint32_t id);
JdwpLibError ref_type_fields_deserialize(DeserializationContext *ctx);
void ref_type_fields_free(JdwpReply *reply);
#endif // FIELDS_H
