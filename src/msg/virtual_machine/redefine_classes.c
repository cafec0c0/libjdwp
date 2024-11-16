#include "redefine_classes.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError redefine_classes_serialize(uint8_t **buf, size_t *len,
                                        void *command, JdwpCommandType type,
                                        IdSizes *id_sizes, uint32_t id) {

  JdwpVirtualMachineRedefineClassesCommand *cmd = command;

  if (!cmd->classes_data)
    return JDWP_LIB_ERR_NULL_POINTER;

  // calculate size up front (to prevent excessive reallocs)
  uint32_t length = 11 + 4;
  for (int i = 0; i < cmd->classes; i++) {
    length += id_sizes->reference_type_id_size + 4;
    length += cmd->classes_data[i].classfile;
  }

  uint8_t *buffer = malloc(length);

  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, length, type, id);

  uint8_t *ptr = buffer + 11;
  serde_write_uint32_adv(&ptr, cmd->classes);
  for (size_t i = 0; i < cmd->classes; i++) {
    serde_write_variable_adv(&ptr, cmd->classes_data[i].ref_type,
                             id_sizes->reference_type_id_size);
    serde_write_uint32_adv(&ptr, cmd->classes_data[i].classfile);

    for (size_t j = 0; j < cmd->classes_data[i].classfile; j++)
      serde_write_uint8_adv(&ptr, cmd->classes_data[i].classfile_data[j]);
  }

  *buf = buffer;
  *len = length;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError redefine_classes_deserialize(DeserializationContext *ctx) {
  REPLY_NEW_EMPTY(rep)

  ReplyHeader header;
  reply_read_header(&header, ctx->bytes);

  REPLY_POPULATE(rep, header.error, header.id, ctx->type)

  *ctx->reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void redefine_classes_free(JdwpReply *reply) { free(reply); }