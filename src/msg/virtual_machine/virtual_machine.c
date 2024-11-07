#include "all_classes.h"
#include "all_threads.h"
#include "capabilities.h"
#include "class_paths.h"
#include "classes_by_signature.h"
#include "create_string.h"
#include "dispose.h"
#include "dispose_objects.h"
#include "exit.h"
#include "id_sizes.h"
#include "msg/command.h"
#include "resume.h"
#include "suspend.h"
#include "top_level_thread_groups.h"

#include "version.h"

#include <jdwp.h>

JdwpLibError vm_command_serialize(uint8_t **buf, size_t *len, void *command,
                                  JdwpCommandType type, IdSizes *id_sizes,
                                  uint32_t id) {
  switch (type) {
  case JDWP_VIRTUAL_MACHINE_VERSION:
    return version_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE:
    return classes_by_signature_serialize(buf, len, command, type, id_sizes,
                                          id);
  case JDWP_VIRTUAL_MACHINE_ALL_CLASSES:
    return all_classes_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_ALL_THREADS:
    return all_threads_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_TOP_LEVEL_THREAD_GROUPS:
    return top_level_thread_groups_serialize(buf, len, command, type, id_sizes,
                                             id);
  case JDWP_VIRTUAL_MACHINE_DISPOSE:
    return dispose_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_ID_SIZES:
    return id_sizes_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_SUSPEND:
    return suspend_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_RESUME:
    return resume_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_EXIT:
    return exit_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_CREATE_STRING:
    return create_string_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_CAPABILITIES:
    return capabilities_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_CLASS_PATHS:
    return class_paths_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_DISPOSE_OBJECTS:
    return dispose_objects_serialize(buf, len, command, type, id_sizes, id);
  default:
    return JDWP_LIB_ERR_UNKNOWN_COMMAND;
  }
}

JdwpLibError vm_command_deserialize(JdwpReply **reply, size_t *len,
                                    uint8_t *bytes, JdwpCommandType type,
                                    IdSizes *id_sizes) {
  switch (type) {
  case JDWP_VIRTUAL_MACHINE_VERSION:
    return version_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE:
    return classes_by_signature_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_ALL_CLASSES:
    return all_classes_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_ALL_THREADS:
    return all_threads_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_TOP_LEVEL_THREAD_GROUPS:
    return top_level_thread_groups_deserialize(reply, len, bytes, type,
                                               id_sizes);
  case JDWP_VIRTUAL_MACHINE_DISPOSE:
    return dispose_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_ID_SIZES:
    return id_sizes_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_SUSPEND:
    return suspend_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_RESUME:
    return resume_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_EXIT:
    return exit_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_CREATE_STRING:
    return create_string_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_CAPABILITIES:
    return capabilities_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_CLASS_PATHS:
    return class_paths_deserialize(reply, len, bytes, type, id_sizes);
  case JDWP_VIRTUAL_MACHINE_DISPOSE_OBJECTS:
    return dispose_objects_deserialize(reply, len, bytes, type, id_sizes);
  default:
    return JDWP_LIB_ERR_UNKNOWN_COMMAND;
  }
}

void vm_reply_free(JdwpReply *reply) {
  switch (reply->type) {
  case JDWP_VIRTUAL_MACHINE_VERSION:
    version_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE:
    classes_by_signature_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_ALL_CLASSES:
    all_classes_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_ALL_THREADS:
    all_threads_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_TOP_LEVEL_THREAD_GROUPS:
    top_level_thread_groups_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_DISPOSE:
    dispose_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_ID_SIZES:
    id_sizes_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_SUSPEND:
    suspend_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_RESUME:
    resume_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_EXIT:
    exit_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_CREATE_STRING:
    create_string_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_CAPABILITIES:
    capabilities_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_CLASS_PATHS:
    class_paths_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_DISPOSE_OBJECTS:
    dispose_objects_free(reply);
  default:;
  }
}