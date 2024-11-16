#include "exit.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError exit_serialize(uint8_t **buf, size_t *len, void *command,
                            JdwpCommandType type, IdSizes *id_sizes,
                            uint32_t id) {
  (void)id_sizes;

  JdwpVirtualMachineExitCommand *cmd = command;

  uint8_t *buffer = malloc(11 + 4);

  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11 + 4, type, id);

  serde_write_uint32(buffer + 11, cmd->exit_code);

  *buf = buffer;
  *len = 11 + 4;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError exit_deserialize(DeserializationContext *ctx) {
  REPLY_NEW_EMPTY(rep)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void exit_free(JdwpReply *reply) { free(reply); }