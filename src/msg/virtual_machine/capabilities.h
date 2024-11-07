//
// Created by rogue on 06/11/24.
//

#ifndef CAPABILITIES_H
#define CAPABILITIES_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError capabilities_serialize(uint8_t **buf, size_t *len, void *command,
                                    JdwpCommandType type, IdSizes *capabilities,
                                    uint32_t id);

JdwpLibError capabilities_deserialize(JdwpReply **reply, size_t *len,
                                      uint8_t *bytes, JdwpCommandType type,
                                      IdSizes *capabilities);
void capabilities_free(JdwpReply *reply);

#endif // CAPABILITIES_H
