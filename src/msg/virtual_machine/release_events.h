#ifndef RELEASE_EVENTS_H
#define RELEASE_EVENTS_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError release_events_serialize(uint8_t **buf, size_t *len, void *command,
                                      JdwpCommandType type, IdSizes *id_sizes,
                                      uint32_t id);

JdwpLibError release_events_deserialize(JdwpReply **reply, size_t *len,
                                        uint8_t *bytes, JdwpCommandType type,
                                        IdSizes *id_sizes);
void release_events_free(JdwpReply *reply);
#endif // RELEASE_EVENTS_H
