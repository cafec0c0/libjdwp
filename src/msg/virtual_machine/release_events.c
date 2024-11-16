#include "release_events.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError release_events_serialize(uint8_t **buf, size_t *len, void *command,
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

JdwpLibError release_events_deserialize(DeserializationContext *ctx) {
  REPLY_NEW_EMPTY(rep)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

cleanup:
  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void release_events_free(JdwpReply *reply) { free(reply); }