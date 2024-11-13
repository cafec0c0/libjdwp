#ifndef MSG_VIRTUAL_MACHINE_H
#define MSG_VIRTUAL_MACHINE_H

#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError vm_command_serialize(uint8_t **buf, size_t *len, void *command,
                                  JdwpCommandType type, IdSizes *id_sizes,
                                  uint32_t id);

JdwpLibError vm_command_deserialize(DeserializationContext *ctx);
void vm_reply_free(JdwpReply *reply);

#endif // MSG_VIRTUAL_MACHINE_H
