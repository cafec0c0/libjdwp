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

JdwpLibError set_default_stratum_deserialize(JdwpReply **reply, size_t *len,
                                             uint8_t *bytes,
                                             JdwpCommandType type,
                                             IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpVirtualMachineSetDefaultStratumData)

  ReplyHeader header;
  reply_read_header(&header, bytes);

  REPLY_POPULATE(rep, header.error, header.id, type)

  if (header.error) {
    free(data);
    rep->data = NULL;
  }

  *reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void set_default_stratum_free(JdwpReply *reply) {
  JdwpVirtualMachineSetDefaultStratumData *data = reply->data;
  free(data);
  free(reply);
}