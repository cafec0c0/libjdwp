#include "all_modules.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError all_modules_serialize(uint8_t **buf, size_t *len, void *command,
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

JdwpLibError all_modules_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpVirtualMachineAllModulesData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->modules = serde_read_uint32_adv(&ptr);
  data->modules_data = calloc(data->modules, sizeof(uint64_t));

  for (int i = 0; i < data->modules; i++) {
    data->modules_data[i] =
        serde_read_variable_adv(&ptr, ctx->id_sizes->object_id_size);
  }

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void all_modules_free(JdwpReply *reply) {
  JdwpVirtualMachineAllModulesData *data = reply->data;
  free(data->modules_data);
  free(data);
  free(reply);
}