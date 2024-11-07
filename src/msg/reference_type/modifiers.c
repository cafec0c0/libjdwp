#include "modifiers.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError ref_type_modifiers_serialize(uint8_t **buf, size_t *len,
                                          void *command, JdwpCommandType type,
                                          IdSizes *id_sizes, uint32_t id) {

  JdwpReferenceTypeModifiersCommand *cmd = command;

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

JdwpLibError ref_type_modifiers_deserialize(JdwpReply **reply, size_t *len,
                                            uint8_t *bytes,
                                            JdwpCommandType type,
                                            IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpReferenceTypeModifiersData)

  ReplyHeader header;
  reply_read_header(&header, bytes);

  REPLY_POPULATE(rep, header.error, header.id, type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = bytes + 11;
  data->mod_bits = serde_read_uint32(ptr);

cleanup:
  *reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void ref_type_modifiers_free(JdwpReply *reply) {
  JdwpReferenceTypeModifiersData *data = reply->data;
  free(data);
  free(reply);
}