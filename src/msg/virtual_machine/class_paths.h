//
// Created by rogue on 06/11/24.
//

#ifndef CLASS_PATHS_H
#define CLASS_PATHS_H

#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError class_paths_serialize(uint8_t **buf, size_t *len, void *command,
                                   JdwpCommandType type, IdSizes *id_sizes,
                                   uint32_t id);

JdwpLibError class_paths_deserialize(JdwpReply **reply, size_t *len,
                                     uint8_t *bytes, JdwpCommandType type,
                                     IdSizes *id_sizes);
void class_paths_free(JdwpReply *reply);

#endif // CLASS_PATHS_H
