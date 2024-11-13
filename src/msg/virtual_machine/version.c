#include "version.h"

#include <stdlib.h>

JdwpLibError version_serialize(uint8_t **buf, size_t *len, void *command,
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

JdwpLibError version_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpVirtualMachineVersionData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->description = serde_read_string_adv(&ptr);
  data->jdwp_major = serde_read_uint32_adv(&ptr);
  data->jdwp_minor = serde_read_uint32_adv(&ptr);
  data->vm_version = serde_read_string_adv(&ptr);
  data->vm_name = serde_read_string_adv(&ptr);

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void version_free(JdwpReply *reply) {
  JdwpVirtualMachineVersionData *data = reply->data;
  free(data->description);
  free(data->vm_version);
  free(data->vm_name);
  free(data);
  free(reply);
}