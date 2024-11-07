//
// Created by rogue on 07/11/24.
//

#ifndef ALL_CLASSES_WITH_GENERIC_H
#define ALL_CLASSES_WITH_GENERIC_H

#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError all_classes_with_generic_serialize(uint8_t **buf, size_t *len,
                                                void *command,
                                                JdwpCommandType type,
                                                IdSizes *id_sizes, uint32_t id);
JdwpLibError all_classes_with_generic_deserialize(JdwpReply **reply,
                                                  size_t *len, uint8_t *bytes,
                                                  JdwpCommandType type,
                                                  IdSizes *id_sizes);
void all_classes_with_generic_free(JdwpReply *reply);

#endif // ALL_CLASSES_WITH_GENERIC_H
