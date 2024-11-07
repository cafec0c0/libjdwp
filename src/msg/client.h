#ifndef CLIENT_H
#define CLIENT_H

#include <pthread.h>
#include <stddef.h>

#include "command.h"
#include "serde.h"

typedef struct {
  int *sockfd;
  size_t *command_attr_buffer_len;
  CommandAttr *command_attr_buffer;
  JdwpReplyCallback callback;
  void **state;
  IdSizes *id_sizes;
  uint8_t should_exit;
} ThreadContext;

typedef struct {
  int sockfd;
  pthread_t thread;
  pthread_mutex_t write_mutex;
  size_t command_attr_buffer_len;
  CommandAttr *command_attr_buffer;
  JdwpReplyCallback callback;
  void **callback_state;
  uint32_t next_id;
  ThreadContext *ctx;
} Client;

#endif // CLIENT_H
