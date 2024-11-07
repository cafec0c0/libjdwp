//
// Created by rogue on 05/11/24.
//

#ifndef CLASSES_BY_SIGNATURE_H
#define CLASSES_BY_SIGNATURE_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError classes_by_signature_serialize(uint8_t **buf, size_t *len,
                                            void *command, JdwpCommandType type,
                                            IdSizes *id_sizes, uint32_t id);
JdwpLibError classes_by_signature_deserialize(JdwpReply **reply, size_t *len,
                                              uint8_t *bytes,
                                              JdwpCommandType type,
                                              IdSizes *id_sizes);
void classes_by_signature_free(JdwpReply *reply);

#endif // CLASSES_BY_SIGNATURE_H
