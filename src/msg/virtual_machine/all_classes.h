#ifndef ALL_CLASSES_H
#define ALL_CLASSES_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError all_classes_serialize(uint8_t **buf, size_t *len, void *command,
                                   JdwpCommandType type, IdSizes *id_sizes,
                                   uint32_t id);

JdwpLibError all_classes_deserialize(DeserializationContext *ctx);
void all_classes_free(JdwpReply *reply);

#endif // ALL_CLASSES_H
