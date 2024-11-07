#ifndef ALL_THREADS_H
#define ALL_THREADS_H
#include "msg/command.h"
#include "msg/reply.h"

JdwpLibError all_threads_serialize(uint8_t **buf, size_t *len, void *command,
                                   JdwpCommandType type, IdSizes *id_sizes,
                                   uint32_t id);

JdwpLibError all_threads_deserialize(JdwpReply **reply, size_t *len,
                                     uint8_t *bytes, JdwpCommandType type,
                                     IdSizes *id_sizes);
void all_threads_free(JdwpReply *reply);

#endif // ALL_THREADS_H
