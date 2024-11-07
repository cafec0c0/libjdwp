//
// Created by rogue on 06/11/24.
//

#ifndef EXIT_H
#define EXIT_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError exit_serialize(uint8_t **buf, size_t *len, void *command,
                            JdwpCommandType type, IdSizes *id_sizes,
                            uint32_t id);

JdwpLibError exit_deserialize(JdwpReply **reply, size_t *len, uint8_t *bytes,
                              JdwpCommandType type, IdSizes *id_sizes);
void exit_free(JdwpReply *reply);
#endif // EXIT_H
