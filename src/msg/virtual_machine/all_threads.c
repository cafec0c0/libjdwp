#include "all_threads.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError all_threads_serialize(uint8_t **buf, size_t *len, void *command,
                                   JdwpCommandType type, IdSizes *id_sizes,
                                   uint32_t id) {
  (void)command;
  (void)id_sizes;

  uint8_t *buffer = malloc(11);

  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11, type, id);

  *buf = buffer;
  *len = 11;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError all_threads_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpVirtualMachineAllThreadsData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = ctx->bytes + 11;
  data->threads = serde_read_uint32_adv(&ptr);
  data->threads_data =
      calloc(data->threads, sizeof(JdwpVirtualMachineAllThreadsThread));

  for (size_t i = 0; i < data->threads; i++) {
    data->threads_data[i].thread =
        serde_read_variable_adv(&ptr, ctx->id_sizes->object_id_size);
  }

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void all_threads_free(JdwpReply *reply) {
  JdwpVirtualMachineAllThreadsData *data = reply->data;
  free(data->threads_data);
  free(data);
  free(reply);
}