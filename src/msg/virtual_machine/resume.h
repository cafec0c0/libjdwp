//
// Created by rogue on 06/11/24.
//

#ifndef RESUME_H
#define RESUME_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError resume_serialize(uint8_t **buf, size_t *len, void *command,
                              JdwpCommandType type, IdSizes *id_sizes,
                              uint32_t id);

JdwpLibError resume_deserialize(DeserializationContext *ctx);
void resume_free(JdwpReply *reply);
#endif // RESUME_H
