//
// Created by rogue on 07/11/24.
//

#ifndef REDEFINE_CLASSES_H
#define REDEFINE_CLASSES_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError redefine_classes_serialize(uint8_t **buf, size_t *len,
                                        void *command, JdwpCommandType type,
                                        IdSizes *id_sizes, uint32_t id);

JdwpLibError redefine_classes_deserialize(JdwpReply **reply, size_t *len,
                                          uint8_t *bytes, JdwpCommandType type,
                                          IdSizes *id_sizes);
void redefine_classes_free(JdwpReply *reply);
#endif // REDEFINE_CLASSES_H
