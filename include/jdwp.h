#ifndef JDWP_H
#define JDWP_H
#include <stddef.h>
#include <stdint.h>

// Command types are represented using their command set and command.
// (command set) (command)
//            v   v
// Format: 0x 00 00
typedef enum {
  JDWP_VIRTUAL_MACHINE_VERSION = 0x0101,
  JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE = 0x0102,
  JDWP_VIRTUAL_MACHINE_ALL_CLASSES = 0x0103,
  JDWP_VIRTUAL_MACHINE_ALL_THREADS = 0x0104,
  JDWP_VIRTUAL_MACHINE_TOP_LEVEL_THREAD_GROUPS = 0x0105,
  JDWP_VIRTUAL_MACHINE_DISPOSE = 0x0106,
  JDWP_VIRTUAL_MACHINE_ID_SIZES = 0x0107,
  JDWP_VIRTUAL_MACHINE_SUSPEND = 0x0108,
  JDWP_VIRTUAL_MACHINE_RESUME = 0x0109,
  JDWP_VIRTUAL_MACHINE_EXIT = 0x010a,
  JDWP_VIRTUAL_MACHINE_CREATE_STRING = 0x010b,
  JDWP_VIRTUAL_MACHINE_CAPABILITIES = 0x010c,
  JDWP_VIRTUAL_MACHINE_CLASS_PATHS = 0x010d,
  JDWP_VIRTUAL_MACHINE_DISPOSE_OBJECTS = 0x010e,
} JdwpCommandType;

typedef enum {
  JDWP_LIB_ERR_NONE,
  JDWP_LIB_ERR_UNKNOWN_COMMAND_SET,
  JDWP_LIB_ERR_UNKNOWN_COMMAND,
  JDWP_LIB_ERR_NULL_POINTER,
  JDWP_LIB_ERR_MALLOC,
  JDWP_LIB_ERR_EOF,
  JDWP_LIB_ERR_NATIVE,
  JDWP_LIB_ERR_PTHREAD,
  JDWP_LIB_ERR_HANDSHAKE_FAILED,
  JDWP_LIB_ERR_INVALID_ADDRESS,
  JDWP_LIB_ERR_COMMAND_BUFFER_FULL,
} JdwpLibError;

typedef enum {
  JDWP_TYPE_TAG_CLASS = 1,
  JDWP_TYPE_TAG_INTERFACE = 2,
  JDWP_TYPE_TAG_ARRAY = 3,
} JdwpTypeTag;

typedef enum {
  JDWP_CLASS_STATUS_VERIFIED = 1,
  JDWP_CLASS_STATUS_PREPARED = 2,
  JDWP_CLASS_STATUS_INITIALIZED = 4,
  JDWP_CLASS_STATUS_ERROR = 8,
} JdwpClassStatus;

#include "msg/virtual_machine.h"

typedef enum { JDWP_ERR_NONE, JDWP_ERR_SOMETHING } JdwpProtocolError;

typedef struct {
  uint32_t id;
  JdwpCommandType type;
  void *data;
  JdwpProtocolError error;
} JdwpReply;

typedef void *JdwpClient;

typedef void (*JdwpReplyCallback)(JdwpReply *, void **);

JdwpLibError jdwp_client_new(JdwpClient *client);
JdwpLibError jdwp_client_set_buffer_size(JdwpClient *client, size_t size);
JdwpLibError jdwp_client_set_callback(JdwpClient *client,
                                      JdwpReplyCallback callback, void **state);
JdwpLibError jdwp_client_connect(JdwpClient client, const char *hostname,
                                 uint16_t port);
JdwpLibError jdwp_client_send(JdwpClient client, uint32_t *id,
                              JdwpCommandType type, void *command);
JdwpLibError jdwp_client_disconnect(JdwpClient client);

void jdwp_reply_free(JdwpReply **reply);

void jdwp_client_free(JdwpClient *client);

#endif // JDWP_H
