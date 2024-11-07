#include "classes_by_signature.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError classes_by_signature_serialize(uint8_t **buf, size_t *len,
                                            void *command, JdwpCommandType type,
                                            IdSizes *id_sizes, uint32_t id) {

  JdwpVirtualMachineClassesBySignatureCommand *cmd = command;

  if (!cmd->signature)
    return JDWP_LIB_ERR_NULL_POINTER;

  size_t signature_len = strlen(cmd->signature);
  uint8_t *buffer = malloc(11 + 4 + signature_len);

  if (!buf)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11 + 4 + signature_len, type, id);

  serde_write_string(buffer + 11, cmd->signature);

  *buf = buffer;
  *len = 11 + 4 + signature_len;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError classes_by_signature_deserialize(JdwpReply **reply, size_t *len,
                                              uint8_t *bytes,
                                              JdwpCommandType type,
                                              IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpVirtualMachineClassesBySignatureData)

  ReplyHeader header;
  reply_read_header(&header, bytes);

  REPLY_POPULATE(rep, header.error, header.id, type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = bytes + 11;
  data->classes = serde_read_uint32_adv(&ptr);
  data->classes_data = calloc(
      data->classes, sizeof(JdwpVirtualMachineClassesBySignatureClassData));

  for (int i = 0; i < data->classes; i++) {
    data->classes_data[i].ref_type_tag = serde_read_uint8_adv(&ptr);
    data->classes_data[i].type_id =
        serde_read_variable_adv(&ptr, id_sizes->reference_type_id_size);
    data->classes_data[i].status = serde_read_uint32_adv(&ptr);
  }

cleanup:
  *reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void classes_by_signature_free(JdwpReply *reply) {
  JdwpVirtualMachineClassesBySignatureData *data = reply->data;
  free(data->classes_data);
  free(data);
  free(reply);
}