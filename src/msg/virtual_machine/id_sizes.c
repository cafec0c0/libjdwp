#include "id_sizes.h"

#include <stdlib.h>

JdwpLibError id_sizes_serialize(uint8_t **buf, size_t *len, void *command,
                                JdwpCommandType type, IdSizes *id_sizes,
                                uint32_t id) {
  uint8_t *buffer = malloc(11);
  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11, type, id);

  *buf = buffer;
  *len = 11;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError id_sizes_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpVirtualMachineIdSizesData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->field_id_size = serde_read_uint32_adv(&ptr);
  data->method_id_size = serde_read_uint32_adv(&ptr);
  data->object_id_size = serde_read_uint32_adv(&ptr);
  data->reference_type_id_size = serde_read_uint32_adv(&ptr);
  data->frame_id_size = serde_read_uint32_adv(&ptr);

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void id_sizes_free(JdwpReply *reply) {
  JdwpVirtualMachineIdSizesData *data = reply->data;
  free(data);
  free(reply);
}