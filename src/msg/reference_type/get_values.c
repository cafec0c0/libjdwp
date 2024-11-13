#include "get_values.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError ref_type_get_values_serialize(uint8_t **buf, size_t *len,
                                           void *command, JdwpCommandType type,
                                           IdSizes *id_sizes, uint32_t id) {

  JdwpReferenceTypeGetValuesCommand *cmd = command;

  uint32_t length = 11 + id_sizes->reference_type_id_size + 4 +
                    (cmd->fields * id_sizes->field_id_size);
  uint8_t *buffer = malloc(length);

  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, length, type, id);

  uint8_t *ptr = buffer + 11;
  serde_write_variable_adv(&ptr, cmd->ref_type,
                           id_sizes->reference_type_id_size);
  serde_write_uint32_adv(&ptr, cmd->fields);
  for (size_t i = 0; i < cmd->fields; i++) {
    serde_write_variable_adv(&ptr, cmd->fields_data[i],
                             id_sizes->field_id_size);
  }

  *buf = buffer;
  *len = length;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError ref_type_get_values_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpReferenceTypeGetValuesData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    // goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->values = serde_read_uint32_adv(&ptr);
  data->values_data =
      calloc(data->values, sizeof(JdwpReferenceTypeGetValuesValueData));

  for (size_t i = 0; i < data->values; i++) {
    data->values_data[i].value = NULL; // TODO: implement
  }

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void ref_type_get_values_free(JdwpReply *reply) {
  // JdwpReferenceTypeGetValuesData *data = reply->data;
  // for (size_t i = 0; i < data->declared; i++) {
  //   free(data->declared_data[i].name);
  //   free(data->declared_data[i].signature);
  // }
  // free(data->declared_data);
  // free(data);
  // free(reply);
}