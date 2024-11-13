#include "fields.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError ref_type_fields_serialize(uint8_t **buf, size_t *len,
                                       void *command, JdwpCommandType type,
                                       IdSizes *id_sizes, uint32_t id) {

  JdwpReferenceTypeFieldsCommand *cmd = command;

  uint8_t *buffer = malloc(11 + id_sizes->reference_type_id_size);

  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11 + id_sizes->reference_type_id_size, type, id);

  serde_write_variable(buffer + 11, cmd->ref_type,
                       id_sizes->reference_type_id_size);

  *buf = buffer;
  *len = 11 + id_sizes->reference_type_id_size;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError ref_type_fields_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpReferenceTypeFieldsData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->declared = serde_read_uint32_adv(&ptr);
  data->declared_data =
      calloc(data->declared, sizeof(JdwpReferenceTypeFieldsFieldData));

  for (size_t i = 0; i < data->declared; i++) {
    data->declared_data[i].field_id =
        serde_read_variable_adv(&ptr, ctx->id_sizes->field_id_size);
    data->declared_data[i].name = serde_read_string_adv(&ptr);
    data->declared_data[i].signature = serde_read_string_adv(&ptr);
    data->declared_data[i].mod_bits = serde_read_uint32_adv(&ptr);
  }

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void ref_type_fields_free(JdwpReply *reply) {
  JdwpReferenceTypeFieldsData *data = reply->data;
  for (size_t i = 0; i < data->declared; i++) {
    free(data->declared_data[i].name);
    free(data->declared_data[i].signature);
  }
  free(data->declared_data);
  free(data);
  free(reply);
}