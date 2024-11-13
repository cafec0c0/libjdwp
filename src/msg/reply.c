#include "reply.h"

#include "reference_type/reference_type.h"
#include "virtual_machine/virtual_machine.h"

#include <stdlib.h>

typedef struct {
  JdwpCommandType t;
  JdwpLibError (*f)(DeserializationContext *);
} Deserializer;

typedef struct {
  JdwpCommandType t;
  void (*f)(JdwpReply *reply);
} FreeFunction;

void reply_read_header(ReplyHeader *header, uint8_t *buf) {
  uint8_t *ptr = buf;
  header->len = serde_read_uint32_adv(&ptr);
  header->id = serde_read_uint32_adv(&ptr);
  header->flags = serde_read_uint8_adv(&ptr);
  header->error = serde_read_uint16_adv(&ptr);
}

JdwpLibError reply_deserialize(DeserializationContext *ctx) {
  switch (ctx->type & 0xFF00) {
  case 0x0100:
    return vm_command_deserialize(ctx);
  case 0x200:
    return ref_type_command_deserialize(ctx);
  default:
    return JDWP_LIB_ERR_UNKNOWN_COMMAND_SET;
  }
}

void jdwp_reply_free(JdwpReply **reply) {
  if (!*reply) {
    return;
  }

  switch ((*reply)->type & 0xFF00) {
  case 0x0100:
    vm_reply_free(*reply);
    break;
  case 0x0200:
    ref_type_reply_free(*reply);
  default:
  }

  *reply = NULL;
}