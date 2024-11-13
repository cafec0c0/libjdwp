#include "resume.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError resume_serialize(uint8_t **buf, size_t *len, void *command,
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

JdwpLibError resume_deserialize(DeserializationContext *ctx) {
  REPLY_NEW(rep, JdwpVirtualMachineResumeData)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
  }

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void resume_free(JdwpReply *reply) {
  JdwpVirtualMachineResumeData *data = reply->data;
  free(data);
  free(reply);
}