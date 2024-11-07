//
// Created by rogue on 07/11/24.
//

#ifndef MODIFIERS_H
#define MODIFIERS_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError ref_type_modifiers_serialize(uint8_t **buf, size_t *len,
                                          void *command, JdwpCommandType type,
                                          IdSizes *id_sizes, uint32_t id);
JdwpLibError ref_type_modifiers_deserialize(JdwpReply **reply, size_t *len,
                                            uint8_t *bytes,
                                            JdwpCommandType type,
                                            IdSizes *id_sizes);
void ref_type_modifiers_free(JdwpReply *reply);

#endif // MODIFIERS_H
