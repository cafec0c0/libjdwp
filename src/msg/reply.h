#ifndef REPLY_H
#define REPLY_H
#include "serde.h"

#include <jdwp.h>
#include <stddef.h>
#include <stdint.h>

#define REPLY_NEW(name, reply_struct)                                          \
  JdwpReply *name = malloc(sizeof(JdwpReply));                                 \
  if (!name)                                                                   \
    return JDWP_LIB_ERR_MALLOC;                                                \
                                                                               \
  reply_struct *data = malloc(sizeof(reply_struct));                           \
  if (!data) {                                                                 \
    free(name);                                                                \
    return JDWP_LIB_ERR_MALLOC;                                                \
  }

#define REPLY_POPULATE(reply, error_val, id_val, type_val)                     \
  reply->data = data;                                                          \
  reply->error = error_val;                                                    \
  reply->id = id_val;                                                          \
  reply->type = type_val;

typedef struct {
  uint32_t len;
  uint32_t id;
  uint8_t flags;
  uint16_t error;
} ReplyHeader;

typedef struct {
  JdwpReply **reply;
  size_t *len;
  uint8_t *bytes;
  JdwpCommandType type;
  IdSizes *id_sizes;
} DeserializationContext;

void reply_read_header(ReplyHeader *header, uint8_t *buf);

JdwpLibError reply_deserialize(DeserializationContext *ctx);

#endif // REPLY_H
