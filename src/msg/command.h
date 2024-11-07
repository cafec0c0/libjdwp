#ifndef COMMAND_H
#define COMMAND_H
#include "serde.h"

#include <jdwp.h>
#include <stdint.h>

typedef struct {
  int8_t is_occupied;
  uint32_t id;
  JdwpCommandType type;
} CommandAttr;

typedef struct {
  uint8_t **buf;
  size_t *len;
  void *command;
  JdwpCommandType type;
  IdSizes *id_sizes;
  uint32_t id;
} CommandSerializationRequest;

void command_write_header(uint8_t *buf, size_t msg_len, JdwpCommandType type,
                          uint32_t id);

JdwpLibError command_serialize(uint8_t **buf, size_t *len, void *command,
                                   JdwpCommandType type, IdSizes *id_sizes,
                                   uint32_t id);

#endif // COMMAND_H