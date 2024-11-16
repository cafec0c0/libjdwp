#include "set_default_stratum.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError set_default_stratum_serialize(uint8_t **buf, size_t *len,
                                           void *command, JdwpCommandType type,
                                           IdSizes *id_sizes, uint32_t id) {

  JdwpVirtualMachineSetDefaultStratumCommand *cmd = command;

  if (!cmd->stratum_id)
    return JDWP_LIB_ERR_NULL_POINTER;

  size_t signature_len = strlen(cmd->stratum_id);
  uint8_t *buffer = malloc(11 + 4 + signature_len);

  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11 + 4 + signature_len, type, id);

  serde_write_string(buffer + 11, cmd->stratum_id);

  *buf = buffer;
  *len = 11 + 4 + signature_len;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError set_default_stratum_deserialize(DeserializationContext *ctx) {
  REPLY_NEW_EMPTY(rep)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void set_default_stratum_free(JdwpReply *reply) { free(reply); }