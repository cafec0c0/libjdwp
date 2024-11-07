#include "id_sizes.h"

#include <stdlib.h>

JdwpLibError id_sizes_serialize(uint8_t **buf, size_t *len, void *command,
                                JdwpCommandType type, IdSizes *id_sizes,
                                uint32_t id) {
  uint8_t *buffer = malloc(11);
  if (!buf)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11, type, id);

  *buf = buffer;
  *len = 11;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError id_sizes_deserialize(JdwpReply **reply, size_t *len,
                                  uint8_t *bytes, JdwpCommandType type,
                                  IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpVirtualMachineIdSizesData)

  ReplyHeader header;
  reply_read_header(&header, bytes);

  REPLY_POPULATE(rep, header.error, header.id, type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = bytes + 11;
  data->field_id_size = serde_read_uint32_adv(&ptr);
  data->method_id_size = serde_read_uint32_adv(&ptr);
  data->object_id_size = serde_read_uint32_adv(&ptr);
  data->reference_type_id_size = serde_read_uint32_adv(&ptr);
  data->frame_id_size = serde_read_uint32_adv(&ptr);

cleanup:
  *reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void id_sizes_free(JdwpReply *reply) {
  JdwpVirtualMachineIdSizesData *data = reply->data;
  free(data);
  free(reply);
}