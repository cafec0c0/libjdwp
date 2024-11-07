#include "capabilities_new.h"

#include <stdlib.h>

JdwpLibError capabilities_new_serialize(uint8_t **buf, size_t *len,
                                        void *command, JdwpCommandType type,
                                        IdSizes *id_sizes, uint32_t id) {
  uint8_t *buffer = malloc(11);
  if (!buffer)
    return JDWP_LIB_ERR_MALLOC;

  command_write_header(buffer, 11, type, id);

  *buf = buffer;
  *len = 11;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError capabilities_new_deserialize(JdwpReply **reply, size_t *len,
                                          uint8_t *bytes, JdwpCommandType type,
                                          IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpVirtualMachineCapabilitiesNewData)

  ReplyHeader header;
  reply_read_header(&header, bytes);

  REPLY_POPULATE(rep, header.error, header.id, type)

  if (header.error) {
    free(data);
    rep->data = NULL;
    goto cleanup;
  }

  uint8_t *ptr = bytes + 11;
  data->can_watch_field_modification = serde_read_uint8_adv(&ptr);
  data->can_watch_field_access = serde_read_uint8_adv(&ptr);
  data->can_get_bytecodes = serde_read_uint8_adv(&ptr);
  data->can_get_synthetic_attribute = serde_read_uint8_adv(&ptr);
  data->can_get_owned_monitor_info = serde_read_uint8_adv(&ptr);
  data->can_get_current_contended_monitor = serde_read_uint8_adv(&ptr);
  data->can_get_monitor_info = serde_read_uint8_adv(&ptr);
  data->can_redefine_classes = serde_read_uint8_adv(&ptr);
  data->can_add_method = serde_read_uint8_adv(&ptr);
  data->can_unrestrictedly_redefine_classes = serde_read_uint8_adv(&ptr);
  data->can_pop_frames = serde_read_uint8_adv(&ptr);
  data->can_use_instance_filters = serde_read_uint8_adv(&ptr);
  data->can_get_source_debug_extensions = serde_read_uint8_adv(&ptr);
  data->can_request_vm_death_event = serde_read_uint8_adv(&ptr);
  data->can_set_default_stratum = serde_read_uint8_adv(&ptr);
  data->can_get_instance_info = serde_read_uint8_adv(&ptr);
  data->can_request_monitor_events = serde_read_uint8_adv(&ptr);
  data->can_get_monitor_frame_info = serde_read_uint8_adv(&ptr);
  data->can_use_source_name_filters = serde_read_uint8_adv(&ptr);
  data->can_get_constant_pool = serde_read_uint8_adv(&ptr);
  data->can_force_early_return = serde_read_uint8_adv(&ptr);
  data->reserved_22 = serde_read_uint8_adv(&ptr);
  data->reserved_23 = serde_read_uint8_adv(&ptr);
  data->reserved_24 = serde_read_uint8_adv(&ptr);
  data->reserved_25 = serde_read_uint8_adv(&ptr);
  data->reserved_26 = serde_read_uint8_adv(&ptr);
  data->reserved_27 = serde_read_uint8_adv(&ptr);
  data->reserved_28 = serde_read_uint8_adv(&ptr);
  data->reserved_29 = serde_read_uint8_adv(&ptr);
  data->reserved_30 = serde_read_uint8_adv(&ptr);
  data->reserved_31 = serde_read_uint8_adv(&ptr);
  data->reserved_32 = serde_read_uint8_adv(&ptr);

cleanup:
  *reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void capabilities_new_free(JdwpReply *reply) {
  JdwpVirtualMachineCapabilitiesNewData *data = reply->data;
  free(data);
  free(reply);
}