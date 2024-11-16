#include "client.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "jdwp.h"
#include "reply.h"
#include "serde.h"
#include "virtual_machine/classes_by_signature.h"
#include "virtual_machine/id_sizes.h"

#include <stdio.h>

#define DEFAULT_BUFFER_LEN 128

static void jdwp_socket_close(JdwpSocket sockfd) {
#ifdef _WIN32
  closesocket(sockfd);
#else
  close(sockfd);
#endif
}

static void jdwp_socket_shutdown(JdwpSocket sockfd) {
#ifdef _WIN32
  shutdown(sockfd, SD_BOTH);
#else
  shutdown(sockfd, SHUT_RDWR);
#endif
}

#ifdef _WIN32
static int has_initialized_winsock = 0;

static JdwpLibError try_init_winsock() {
  WSADATA wsa_data;
  int res;
  if (!has_initialized_winsock) {
    res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    has_initialized_winsock = 1;
    if (res != 0) {
      return JDWP_LIB_ERR_NATIVE;
    }
  }
  return JDWP_LIB_ERR_NONE;
}
#endif

JdwpLibError jdwp_client_new(JdwpClient *client) {
  if (!client)
    return JDWP_LIB_ERR_NULL_POINTER;

  Client *c = malloc(sizeof(Client));
  if (!c)
    return JDWP_LIB_ERR_MALLOC;

  c->sockfd = -1;
  c->thread = 0;

  pthread_mutex_init(&c->write_mutex, NULL);

  c->command_attr_buffer_len = DEFAULT_BUFFER_LEN;
  c->command_attr_buffer = NULL;

  *client = c;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError jdwp_client_set_buffer_size(JdwpClient *client, size_t size) {
  if (!client)
    return JDWP_LIB_ERR_NULL_POINTER;

  ((Client *)client)->command_attr_buffer_len = size;

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError jdwp_client_set_callback(JdwpClient *client,
                                      JdwpReplyCallback callback,
                                      void **state) {
  if (!client)
    return JDWP_LIB_ERR_NULL_POINTER;

  ((Client *)client)->callback = callback;
  ((Client *)client)->callback_state = state;

  return JDWP_LIB_ERR_NONE;
}

static CommandAttr *find_in_buffer(CommandAttr *command_attr_buffer, size_t len,
                                   uint32_t id) {
  if (!command_attr_buffer)
    return NULL;

  for (size_t idx = 0; idx < len; idx++) {
    CommandAttr *attr = command_attr_buffer + idx;
    if (attr->is_occupied && attr->id == id)
      return attr;
  }

  return NULL;
}

static ssize_t insert_into_buffer(CommandAttr *command_attr_buffer, size_t len,
                                  uint32_t id, JdwpCommandType type,
                                  void *command) {
  if (!command_attr_buffer)
    return -1;

  for (ssize_t idx = 0; idx < len; idx++) {
    CommandAttr *attr = command_attr_buffer + idx;
    if (!attr->is_occupied) {
      attr->is_occupied = 1;
      attr->id = id;
      attr->type = type;
      return idx;
    }
  }

  return -1;
}

static void remove_from_buffer(CommandAttr *command_attr_buffer, size_t len,
                               uint32_t id) {
  for (size_t idx = 0; idx < len; idx++) {
    CommandAttr *attr = command_attr_buffer + idx;
    if (attr->is_occupied && attr->id == id) {
      attr->is_occupied = 0;
      break;
    }
  }
}

static JdwpLibError client_read_full_reply(uint8_t **buf, int sockfd) {
  uint8_t length_buf[4] = {0};

  ssize_t bytes_read = recv(sockfd, length_buf, 4, MSG_WAITALL);
  if (bytes_read == 0)
    return JDWP_LIB_ERR_EOF;
  if (bytes_read == -1)
    return JDWP_LIB_ERR_NATIVE;

  uint32_t jdwp_reply_len = serde_read_uint32(length_buf);
  *buf = malloc(jdwp_reply_len);
  if (!*buf) {
    return JDWP_LIB_ERR_MALLOC;
  }

  memcpy(*buf, length_buf, 4);
  bytes_read = recv(sockfd, *buf + 4, jdwp_reply_len - 4, MSG_WAITALL);
  if (bytes_read == 0)
    return JDWP_LIB_ERR_EOF;
  if (bytes_read == -1)
    return JDWP_LIB_ERR_NATIVE;

  return JDWP_LIB_ERR_NONE;
}

void *client_listen(void *context) {
  ThreadContext *ctx = context;
  uint8_t *buf;

  while (!ctx->should_exit) {
    JdwpLibError err = client_read_full_reply(&buf, *ctx->sockfd);
    if (err != JDWP_LIB_ERR_NONE) {
      if (err == JDWP_LIB_ERR_EOF)
        return NULL;
      continue;
    }

    CommandAttr *attr =
        find_in_buffer(ctx->command_attr_buffer, *ctx->command_attr_buffer_len,
                       serde_read_uint32(buf + 4));

    if (!attr) {
      continue;
    }

    JdwpReply *reply;
    size_t len;
    DeserializationContext data = {
        .reply = &reply,
        .len = &len,
        .bytes = buf,
        .type = attr->type,
        .id_sizes = ctx->id_sizes,
    };

    if (reply_deserialize(&data) != JDWP_LIB_ERR_NONE) {
      free(buf);
      remove_from_buffer(ctx->command_attr_buffer,
                         *ctx->command_attr_buffer_len, attr->id);
      continue;
    }

    free(buf);
    if (ctx->callback)
      ctx->callback(reply, ctx->state);
    remove_from_buffer(ctx->command_attr_buffer, *ctx->command_attr_buffer_len,
                       attr->id);
  }
}

JdwpLibError spawn_listener_thread(Client *client, IdSizes *id_sizes,
                                   JdwpReplyCallback callback, void **state) {
  if (!client) {
    return JDWP_LIB_ERR_NULL_POINTER;
  }

  client->ctx = malloc(sizeof(ThreadContext));
  client->ctx->sockfd = &client->sockfd;
  client->ctx->command_attr_buffer_len = &client->command_attr_buffer_len;
  client->ctx->command_attr_buffer = client->command_attr_buffer;
  client->ctx->callback = callback;
  client->ctx->state = state;
  client->ctx->should_exit = 0;
  client->ctx->id_sizes = id_sizes;

  int ret = pthread_create(&client->thread, NULL, client_listen, client->ctx);
  if (ret)
    return JDWP_LIB_ERR_PTHREAD;

  return JDWP_LIB_ERR_NONE;
}

static JdwpLibError handshake(int sockfd) {
  char handshake_string[] = "JDWP-Handshake";
  ssize_t bytes_sent = send(sockfd, handshake_string, 14, 0);
  if (bytes_sent == -1) {
    return JDWP_LIB_ERR_NATIVE;
  }

  char handshake_buf[14];
  ssize_t bytes_read = recv(sockfd, handshake_buf, 14, MSG_WAITALL);
  if (bytes_read == 0)
    return JDWP_LIB_ERR_EOF;
  if (bytes_read == -1)
    return JDWP_LIB_ERR_NATIVE;

  if (memcmp(handshake_buf, handshake_string, 14) != 0) {
    char err_str[15];
    strcpy(err_str, handshake_buf);
    return JDWP_LIB_ERR_HANDSHAKE_FAILED;
  }

  return JDWP_LIB_ERR_NONE;
}

static JdwpLibError init_id_sizes(IdSizes **id_sizes, int sockfd, uint32_t id) {
  uint8_t *buf;
  size_t len;
  JdwpLibError err = id_sizes_serialize(
      &buf, &len, NULL, JDWP_VIRTUAL_MACHINE_ID_SIZES, NULL, id);

  if (err)
    return err;

  ssize_t bytes_sent = send(sockfd, buf, len, 0);
  free(buf);
  if (bytes_sent == -1) {
    return JDWP_LIB_ERR_NATIVE;
  }

  char in_buf[11 + (5 * 4)];
  ssize_t bytes_read = recv(sockfd, in_buf, 11 + (5 * 4), MSG_WAITALL);
  if (bytes_read == 0)
    return JDWP_LIB_ERR_EOF;
  if (bytes_read == -1)
    return JDWP_LIB_ERR_NATIVE;

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .len = NULL,
      .bytes = in_buf,
      .type = JDWP_VIRTUAL_MACHINE_ID_SIZES,
      .id_sizes = NULL,
  };
  err = id_sizes_deserialize(&ctx);
  if (err)
    return err;

  JdwpVirtualMachineIdSizesData *data = reply->data;
  *id_sizes = malloc(sizeof(IdSizes));
  if (!*id_sizes) {
    id_sizes_free(reply);
    return JDWP_LIB_ERR_MALLOC;
  }

  (*id_sizes)->object_id_size = data->object_id_size;
  (*id_sizes)->field_id_size = data->field_id_size;
  (*id_sizes)->method_id_size = data->method_id_size;
  (*id_sizes)->reference_type_id_size = data->reference_type_id_size;
  (*id_sizes)->frame_id_size = data->frame_id_size;

  id_sizes_free(reply);
  return JDWP_LIB_ERR_NONE;
}

JdwpLibError jdwp_client_connect(JdwpClient client, const char *hostname,
                                 uint16_t port) {
  if (!client)
    return JDWP_LIB_ERR_NULL_POINTER;

  Client *c = client;
  c->command_attr_buffer =
      calloc(c->command_attr_buffer_len, sizeof(CommandAttr));
  if (!c->command_attr_buffer)
    return JDWP_LIB_ERR_MALLOC;

#ifdef _WIN32
  JdwpLibError err = try_init_winsock();
  if (err != JDWP_LIB_ERR_NONE) {
    return err;
  }

  struct addrinfo *result = NULL, *ptr = NULL, hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  char port_string[6];
  sprintf(port_string, "%d", port);

  int res;
  res = getaddrinfo(hostname, port_string, &hints, &result);
  if (res != 0) {
    return JDWP_LIB_ERR_NATIVE;
  }

  JdwpSocket sockfd =
      socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (sockfd == INVALID_SOCKET) {
    return JDWP_LIB_ERR_NATIVE;
  }

  res = connect(sockfd, result->ai_addr, (int)result->ai_addrlen);
  if (res == SOCKET_ERROR) {
    jdwp_socket_close(sockfd);
    return JDWP_LIB_ERR_NATIVE;
  }
#else
  struct sockaddr_in serv_addr;
  JdwpSocket sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    return JDWP_LIB_ERR_NATIVE;
  }

  memset(&serv_addr, 0, sizeof(struct sockaddr_in));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  int ret = inet_pton(AF_INET, hostname, &serv_addr.sin_addr);
  if (ret <= 0) {
    return JDWP_LIB_ERR_INVALID_ADDRESS;
  }

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    return JDWP_LIB_ERR_NATIVE;
  }
#endif

  c->sockfd = sockfd;

  JdwpLibError err = handshake(c->sockfd);
  if (err) {
    jdwp_socket_close(c->sockfd);
    return err;
  }

  IdSizes *id_sizes;
  err = init_id_sizes(&id_sizes, c->sockfd, c->next_id++);
  if (err) {
    jdwp_socket_close(c->sockfd);
    return err;
  }

  err = spawn_listener_thread(c, id_sizes, c->callback, c->callback_state);
  if (err) {
    jdwp_socket_close(c->sockfd);
    return err;
  }

  return JDWP_LIB_ERR_NONE;
}

JdwpLibError jdwp_client_send(JdwpClient client, uint32_t id,
                              JdwpCommandType type, void *command) {
  if (!client || !command)
    return JDWP_LIB_ERR_NULL_POINTER;

  Client *c = client;

  JdwpLibError err;
  pthread_mutex_lock(&c->write_mutex); // Enter critical

  // try to insert into buffer
  if (insert_into_buffer(c->command_attr_buffer, c->command_attr_buffer_len, id,
                         type, command) == -1) {
    err = JDWP_LIB_ERR_COMMAND_BUFFER_FULL;
    goto cleanup;
  }

  uint8_t *buf = NULL;
  size_t len;
  err = command_serialize(&buf, &len, command, type, c->ctx->id_sizes, id);
  if (err)
    goto cleanup;

  ssize_t bytes_sent = send(c->sockfd, buf, len, 0);
  if (bytes_sent == -1) {
    err = JDWP_LIB_ERR_NATIVE;
  }

cleanup:
  free(buf);
  pthread_mutex_unlock(&c->write_mutex); // Exit critical

  return err;
}

JdwpLibError jdwp_client_disconnect(JdwpClient client) {
  if (!client)
    return JDWP_LIB_ERR_NULL_POINTER;

  Client *cl = client;

  if (cl->ctx)
    cl->ctx->should_exit = 1;

  jdwp_socket_shutdown(cl->sockfd);
  pthread_join(cl->thread, NULL);

  jdwp_socket_close(cl->sockfd);
  return JDWP_LIB_ERR_NONE;
}

void jdwp_client_free(JdwpClient *client) {
  if (!client)
    return;

  Client *c = *client;

  if (c) {
    pthread_mutex_destroy(&c->write_mutex);
    free(c->ctx->command_attr_buffer);
    free(c->ctx->id_sizes);
    free(c->ctx);
    free(*client);
  }
  *client = NULL;
}
