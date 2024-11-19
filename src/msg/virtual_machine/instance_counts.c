#include "instance_counts.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError instance_counts_serialize(uint8_t **buf, size_t *len,
                                       void *command, JdwpCommandType type,
                                       IdSizes *id_sizes, uint32_t id) {

  JdwpVirtualMachineInstanceCountsCommand *cmd = command;

  if (!cmd->ref_types_data)
    return JDWP_LIB_ERR_NULL_POINTER;

  size_t length =
      11 + 4 + (cmd->ref_types_count * id_sizes->reference_type_id_size);
  uint8_t *buffer = malloc(length);

  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, length, type, id);

  uint8_t *ptr = buffer + 11;
  serde_write_uint32_adv(&ptr, cmd->ref_types_count);
  for (size_t i = 0; i < cmd->ref_types_count; i++) {
    serde_write_variable_adv(&ptr, cmd->ref_types_data[i],
                             id_sizes->reference_type_id_size);
  }

  *buf = buffer;
  *len = length;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError instance_counts_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpVirtualMachineInstanceCountsData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->counts = serde_read_uint32_adv(&ptr);
  data->instance_count_data = calloc(data->counts, sizeof(uint64_t));

  for (uint32_t i = 0; i < data->counts; i++)
    data->instance_count_data[i] = serde_read_uint64_adv(&ptr);

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void instance_counts_free(JdwpReply *reply) {
  JdwpVirtualMachineInstanceCountsData *data = reply->data;
  free(data->instance_count_data);
  free(data);
  free(reply);
}