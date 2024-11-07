#include "top_level_thread_groups.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError top_level_thread_groups_serialize(uint8_t **buf, size_t *len,
                                               void *command,
                                               JdwpCommandType type,
                                               IdSizes *id_sizes, uint32_t id) {
  uint8_t *buffer = malloc(11);

  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11, type, id);

  *buf = buffer;
  *len = 11;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError top_level_thread_groups_deserialize(JdwpReply **reply, size_t *len,
                                                 uint8_t *bytes,
                                                 JdwpCommandType type,
                                                 IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpVirtualMachineTopLevelThreadGroupsData)

  ReplyHeader header;
  reply_read_header(&header, bytes);

  REPLY_POPULATE(rep, header.error, header.id, type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = bytes + 11;
  data->groups = serde_read_uint32_adv(&ptr);
  data->groups_data =
      calloc(data->groups, sizeof(JdwpVirtualMachineTopLevelThreadGroupsGroup));

  for (int i = 0; i < data->groups; i++) {
    data->groups_data[i].group =
        serde_read_variable_adv(&ptr, id_sizes->object_id_size);
  }

cleanup:
  *reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void top_level_thread_groups_free(JdwpReply *reply) {
  JdwpVirtualMachineTopLevelThreadGroupsData *data = reply->data;
  free(data->groups_data);
  free(data);
  free(reply);
}