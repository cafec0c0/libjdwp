//
// Created by rogue on 06/11/24.
//

#ifndef DISPOSE_OBJECTS_H
#define DISPOSE_OBJECTS_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError dispose_objects_serialize(uint8_t **buf, size_t *len,
                                       void *command, JdwpCommandType type,
                                       IdSizes *dispose_objects, uint32_t id);

JdwpLibError dispose_objects_deserialize(DeserializationContext *ctx);
void dispose_objects_free(JdwpReply *reply);
#endif // DISPOSE_OBJECTS_H
