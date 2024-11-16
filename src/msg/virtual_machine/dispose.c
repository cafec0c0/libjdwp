#include "dispose.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError dispose_serialize(uint8_t **buf, size_t *len, void *command,
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

JdwpLibError dispose_deserialize(DeserializationContext *ctx) {
  REPLY_NEW_EMPTY(rep)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  if (header.error) {
    free(data);
    rep->data = NULL;
  }

  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void dispose_free(JdwpReply *reply) { free(reply); }