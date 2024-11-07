#include "create_string.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError create_string_serialize(uint8_t **buf, size_t *len, void *command,
                                     JdwpCommandType type, IdSizes *id_sizes,
                                     uint32_t id) {

  JdwpVirtualMachineCreateStringCommand *cmd = command;

  size_t utf_len = strlen(cmd->utf);
  uint8_t *buffer = malloc(11 + 4 + utf_len);

  if (!buf)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11 + 4 + utf_len, type, id);

  serde_write_string(buffer + 11, cmd->utf);

  *buf = buffer;
  *len = 11 + 4 + utf_len;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError create_string_deserialize(JdwpReply **reply, size_t *len,
                                       uint8_t *bytes, JdwpCommandType type,
                                       IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpVirtualMachineCreateStringData)

  ReplyHeader header;
  reply_read_header(&header, bytes);

  REPLY_POPULATE(rep, header.error, header.id, type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  data->string_object =
      serde_read_variable(bytes + 11, id_sizes->object_id_size);

cleanup:
  *reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void create_string_free(JdwpReply *reply) {
  JdwpVirtualMachineCreateStringData *data = reply->data;
  free(data);
  free(reply);
}