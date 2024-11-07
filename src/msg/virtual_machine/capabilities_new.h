//
// Created by rogue on 07/11/24.
//

#ifndef CAPABILITIES_NEW_H
#define CAPABILITIES_NEW_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError capabilities_new_serialize(uint8_t **buf, size_t *len,
                                        void *command, JdwpCommandType type,
                                        IdSizes *capabilities_new, uint32_t id);

JdwpLibError capabilities_new_deserialize(JdwpReply **reply, size_t *len,
                                          uint8_t *bytes, JdwpCommandType type,
                                          IdSizes *capabilities_new);
void capabilities_new_free(JdwpReply *reply);

#endif // CAPABILITIES_NEW_H
