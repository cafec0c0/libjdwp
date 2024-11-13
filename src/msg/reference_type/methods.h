//
// Created by rogue on 07/11/24.
//

#ifndef METHODS_H
#define METHODS_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError ref_type_methods_serialize(uint8_t **buf, size_t *len,
                                        void *command, JdwpCommandType type,
                                        IdSizes *id_sizes, uint32_t id);
JdwpLibError ref_type_methods_deserialize(DeserializationContext *ctx);
void ref_type_methods_free(JdwpReply *reply);
#endif // METHODS_H
