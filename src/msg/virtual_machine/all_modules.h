//
// Created by rogue on 07/11/24.
//

#ifndef ALL_MODULES_H
#define ALL_MODULES_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError all_modules_serialize(uint8_t **buf, size_t *len, void *command,
                                   JdwpCommandType type, IdSizes *id_sizes,
                                   uint32_t id);

JdwpLibError all_modules_deserialize(DeserializationContext *ctx);
void all_modules_free(JdwpReply *reply);
#endif // ALL_MODULES_H
