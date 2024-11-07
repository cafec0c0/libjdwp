#include "all_classes_with_generic.h"

#include <stdlib.h>
#include <string.h>

JdwpLibError all_classes_with_generic_serialize(uint8_t **buf, size_t *len,
                                                void *command,
                                                JdwpCommandType type,
                                                IdSizes *id_sizes,
                                                uint32_t id) {
  uint8_t *buffer = malloc(11);

  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11, type, id);

  *buf = buffer;
  *len = 11;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError all_classes_with_generic_deserialize(JdwpReply **reply,
                                                  size_t *len, uint8_t *bytes,
                                                  JdwpCommandType type,
                                                  IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpVirtualMachineAllClassesWithGenericData)

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
      data->classes, sizeof(JdwpVirtualMachineAllClassesWithGenericClassData));

  for (int i = 0; i < data->classes; i++) {
    data->classes_data[i].ref_type_tag = serde_read_uint8_adv(&ptr);
    data->classes_data[i].type_id =
        serde_read_variable_adv(&ptr, id_sizes->reference_type_id_size);
    data->classes_data[i].signature = serde_read_string_adv(&ptr);
    data->classes_data[i].generic_signature = serde_read_string_adv(&ptr);
    data->classes_data[i].status = serde_read_uint32_adv(&ptr);
  }

cleanup:
  *reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void all_classes_with_generic_free(JdwpReply *reply) {
  JdwpVirtualMachineAllClassesWithGenericData *data = reply->data;
  for (int i = 0; i < data->classes; i++) {
    free(data->classes_data[i].signature);
    free(data->classes_data[i].generic_signature);
  }
  free(data->classes_data);
  free(data);
  free(reply);
}