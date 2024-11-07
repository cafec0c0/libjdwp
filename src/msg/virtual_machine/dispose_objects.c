#include "dispose_objects.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError dispose_objects_serialize(uint8_t **buf, size_t *len,
                                       void *command, JdwpCommandType type,
                                       IdSizes *id_sizes, uint32_t id) {

  JdwpVirtualMachineDisposeObjectsCommand *cmd = command;

  if (!cmd->requests_data)
    return JDWP_LIB_ERR_NULL_POINTER;

  // calculate size up front (to prevent excessive reallocs)
  uint32_t length = 11 + 4 + ((4 + id_sizes->object_id_size) * cmd->requests);

  uint8_t *buffer = malloc(length);

  if (!buf)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, length, type, id);

  uint8_t *ptr = buffer + 11;
  serde_write_uint32_adv(&ptr, cmd->requests);
  for (size_t i = 0; i < cmd->requests; i++) {
    serde_write_variable_adv(&ptr, cmd->requests_data[i].object,
                             id_sizes->object_id_size);
    serde_write_uint32_adv(&ptr, cmd->requests_data[i].ref_cnt);
  }

  *buf = buffer;
  *len = length;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError dispose_objects_deserialize(JdwpReply **reply, size_t *len,
                                         uint8_t *bytes, JdwpCommandType type,
                                         IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpVirtualMachineDisposeObjectsData)

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

void dispose_objects_free(JdwpReply *reply) {
  JdwpVirtualMachineDisposeObjectsData *data = reply->data;
  free(data);
  free(reply);
}