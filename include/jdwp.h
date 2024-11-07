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
  JDWP_VIRTUAL_MACHINE_HOLD_EVENTS = 0x010f,
  JDWP_VIRTUAL_MACHINE_RELEASE_EVENTS = 0x0110,
  JDWP_VIRTUAL_MACHINE_CAPABILITIES_NEW = 0x0111,
  JDWP_VIRTUAL_MACHINE_REDEFINE_CLASSES = 0x0112,
  JDWP_VIRTUAL_MACHINE_SET_DEFAULT_STRATUM = 0x0113,
  JDWP_VIRTUAL_MACHINE_ALL_CLASSES_WITH_GENERIC = 0x0114,
  JDWP_VIRTUAL_MACHINE_INSTANCE_COUNTS = 0x0115,
  JDWP_VIRTUAL_MACHINE_ALL_MODULES = 0x0116,

  JDWP_REFERENCE_TYPE_SIGNATURE = 0x0201,
  JDWP_REFERENCE_TYPE_CLASS_LOADER = 0x0202,
  JDWP_REFERENCE_TYPE_MODIFIERS = 0x0203,
  JDWP_REFERENCE_TYPE_FIELDS = 0x0204,
  JDWP_REFERENCE_TYPE_METHODS = 0x0205,
  JDWP_REFERENCE_TYPE_GET_VALUES = 0x0206,
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

// From https://docs.oracle.com/javase/specs/jvms/se23/jvms23.pdf pg. 75
typedef enum {
  JDWP_CLASS_ACC_PUBLIC = 0x0001,
  JDWP_CLASS_ACC_FINAL = 0x0010,
  JDWP_CLASS_ACC_SUPER = 0x0020,
  JDWP_CLASS_ACC_INTERFACE = 0x0200,
  JDWP_CLASS_ACC_ABSTRACT = 0x400,
  JDWP_CLASS_ACC_SYNTHETIC = 0x1000,
  JDWP_CLASS_ACC_ANNOTATION = 0x2000,
  JDWP_CLASS_ACC_ENUM = 0x4000,
  JDWP_CLASS_ACC_MODULE = 0x8000,

  JDWP_CLASS_ACC_SYNTHETIC_HIGH_BIT = 0xf000000,
} JdwpClassModifiers;

// From https://docs.oracle.com/javase/specs/jvms/se23/jvms23.pdf pg. 100
typedef enum {
  JDWP_FIELD_ACC_PUBLIC = 0x0001,
  JDWP_FIELD_ACC_PRIVATE = 0x0002,
  JDWP_FIELD_ACC_PROTECTED = 0x0004,
  JDWP_FIELD_ACC_STATIC = 0x0008,
  JDWP_FIELD_ACC_FINAL = 0x0010,
  JDWP_FIELD_ACC_VOLATILE = 0x0040,
  JDWP_FIELD_ACC_TRANSIENT = 0x0080,
  JDWP_FIELD_ACC_SYNTHETIC = 0x1000,
  JDWP_FIELD_ACC_ENUM = 0x4000,

  JDWP_FIELD_ACC_SYNTHETIC_HIGH_BIT = 0xf000000,
} JdwpFieldModifiers;

// From https://docs.oracle.com/javase/specs/jvms/se23/jvms23.pdf pg. 102
typedef enum {
  JDWP_METHOD_ACC_PUBLIC = 0x0001,
  JDWP_METHOD_ACC_PRIVATE = 0x0002,
  JDWP_METHOD_ACC_PROTECTED = 0x0004,
  JDWP_METHOD_ACC_STATIC = 0x0008,
  JDWP_METHOD_ACC_FINAL = 0x0010,
  JDWP_METHOD_ACC_SYNCHRONIZED = 0x0020,
  JDWP_METHOD_ACC_BRIDGE = 0x0040,
  JDWP_METHOD_ACC_VAARGS = 0x0080,
  JDWP_METHOD_ACC_NATIVE = 0x0100,
  JDWP_METHOD_ACC_ABSTRACT = 0x0400,
  JDWP_METHOD_ACC_STRICT = 0x0800,
  JDWP_METHOD_ACC_SYNTHETIC = 0x1000,

  JDWP_METHOD_ACC_SYNTHETIC_HIGH_BIT = 0xf000000,
} JdwpMethodModifiers;

#include "msg/reference_type.h"
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
