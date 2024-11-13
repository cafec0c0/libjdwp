#include "all_classes.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError all_classes_serialize(uint8_t **buf, size_t *len, void *command,
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

JdwpLibError all_classes_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpVirtualMachineAllClassesData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->classes = serde_read_uint32_adv(&ptr);
  data->classes_data =
      calloc(data->classes, sizeof(JdwpVirtualMachineAllClassesClassData));

  for (int i = 0; i < data->classes; i++) {
    data->classes_data[i].ref_type_tag = serde_read_uint8_adv(&ptr);
    data->classes_data[i].type_id =
        serde_read_variable_adv(&ptr, ctx->id_sizes->object_id_size);
    data->classes_data[i].signature = serde_read_string_adv(&ptr);
    data->classes_data[i].status = serde_read_uint32_adv(&ptr);
  }

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void all_classes_free(JdwpReply *reply) {
  JdwpVirtualMachineAllClassesData *data = reply->data;
  for (uint32_t i = 0; i < data->classes; i++)
    free(data->classes_data[i].signature);
  free(data->classes_data);
  free(data);
  free(reply);
}