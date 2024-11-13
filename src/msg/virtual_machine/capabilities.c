#include "capabilities.h"

#include <stdlib.h>

JdwpLibError capabilities_serialize(uint8_t **buf, size_t *len, void *command,
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

JdwpLibError capabilities_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpVirtualMachineCapabilitiesData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->can_watch_field_modification = serde_read_uint8_adv(&ptr);
  data->can_watch_field_access = serde_read_uint8_adv(&ptr);
  data->can_get_bytecodes = serde_read_uint8_adv(&ptr);
  data->can_get_synthetic_attribute = serde_read_uint8_adv(&ptr);
  data->can_get_owned_monitor_info = serde_read_uint8_adv(&ptr);
  data->can_get_current_contended_monitor = serde_read_uint8_adv(&ptr);
  data->can_get_monitor_info = serde_read_uint8_adv(&ptr);

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void capabilities_free(JdwpReply *reply) {
  JdwpVirtualMachineCapabilitiesData *data = reply->data;
  free(data);
  free(reply);
}