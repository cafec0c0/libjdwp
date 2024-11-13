#ifndef MSG_REFERENCE_TYPE_H
#define MSG_REFERENCE_TYPE_H

#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError ref_type_command_serialize(uint8_t **buf, size_t *len,
                                        void *command, JdwpCommandType type,
                                        IdSizes *id_sizes, uint32_t id);

JdwpLibError ref_type_command_deserialize(DeserializationContext *ctx);
void ref_type_reply_free(JdwpReply *reply);

#endif // MSG_REFERENCE_TYPE_H
