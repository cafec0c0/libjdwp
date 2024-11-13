//
// Created by rogue on 07/11/24.
//

#ifndef HOLD_EVENTS_H
#define HOLD_EVENTS_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError hold_events_serialize(uint8_t **buf, size_t *len, void *command,
                                   JdwpCommandType type, IdSizes *id_sizes,
                                   uint32_t id);

JdwpLibError hold_events_deserialize(DeserializationContext *ctx);
void hold_events_free(JdwpReply *reply);
#endif // HOLD_EVENTS_H
