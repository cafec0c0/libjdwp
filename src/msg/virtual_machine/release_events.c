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

JdwpLibError release_events_deserialize(JdwpReply **reply, size_t *len,
                                        uint8_t *bytes, JdwpCommandType type,
                                        IdSizes *id_sizes) {
  REPLY_NEW(rep, JdwpVirtualMachineReleaseEventsData)

  ReplyHeader header;
  reply_read_header(&header, bytes);

  REPLY_POPULATE(rep, header.error, header.id, type)

  if (header.error) {
    free(data);
    rep->data = NULL;
  }

cleanup:
  *reply = rep;

  return JDWP_LIB_ERR_NONE;
}

void release_events_free(JdwpReply *reply) {
  JdwpVirtualMachineReleaseEventsData *data = reply->data;
  free(data);
  free(reply);
}