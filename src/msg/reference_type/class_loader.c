#include "class_loader.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError ref_type_class_loader_serialize(uint8_t **buf, size_t *len,
                                             void *command,
                                             JdwpCommandType type,
                                             IdSizes *id_sizes, uint32_t id) {

  JdwpReferenceTypeClassLoaderCommand *cmd = command;

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

JdwpLibError ref_type_class_loader_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpReferenceTypeClassLoaderData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->class_loader_id =
      serde_read_variable(ptr, ctx->id_sizes->object_id_size);

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void ref_type_class_loader_free(JdwpReply *reply) {
  JdwpReferenceTypeClassLoaderData *data = reply->data;
  free(data);
  free(reply);
}