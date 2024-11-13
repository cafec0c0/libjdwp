#include "class_paths.h"

#include <stdlib.h>

JdwpLibError class_paths_serialize(uint8_t **buf, size_t *len, void *command,
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

JdwpLibError class_paths_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpVirtualMachineClassPathsData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->base_dir = serde_read_string_adv(&ptr);
  data->class_paths = serde_read_uint32_adv(&ptr);
  data->class_paths_data = calloc(data->class_paths, sizeof(char *));
  for (int i = 0; i < data->class_paths; i++) {
    data->class_paths_data[i] = serde_read_string_adv(&ptr);
  }

  data->boot_class_paths = serde_read_uint32_adv(&ptr);
  data->boot_class_paths_data = calloc(data->boot_class_paths, sizeof(char *));
  for (int i = 0; i < data->boot_class_paths; i++) {
    data->boot_class_paths_data[i] = serde_read_string_adv(&ptr);
  }

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void class_paths_free(JdwpReply *reply) {
  JdwpVirtualMachineClassPathsData *data = reply->data;
  free(data->base_dir);
  for (int i = 0; i < data->class_paths; i++)
    free(data->class_paths_data[i]);
  free(data->class_paths_data);
  for (int i = 0; i < data->boot_class_paths; i++)
    free(data->boot_class_paths_data[i]);
  free(data->boot_class_paths_data);
  free(data);
  free(reply);
}