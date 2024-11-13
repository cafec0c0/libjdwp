#include "all_classes.h"
#include "all_classes_with_generic.h"
#include "all_modules.h"
#include "all_threads.h"
#include "capabilities.h"
#include "capabilities_new.h"
#include "class_paths.h"
#include "classes_by_signature.h"
#include "create_string.h"
#include "dispose.h"
#include "dispose_objects.h"
#include "exit.h"
#include "hold_events.h"
#include "id_sizes.h"
#include "instance_counts.h"
#include "redefine_classes.h"
#include "release_events.h"
#include "resume.h"
#include "set_default_stratum.h"
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
  case JDWP_VIRTUAL_MACHINE_HOLD_EVENTS:
    return hold_events_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_RELEASE_EVENTS:
    return release_events_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_CAPABILITIES_NEW:
    return capabilities_new_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_REDEFINE_CLASSES:
    return redefine_classes_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_SET_DEFAULT_STRATUM:
    return set_default_stratum_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_ALL_CLASSES_WITH_GENERIC:
    return all_classes_with_generic_serialize(buf, len, command, type, id_sizes,
                                              id);
  case JDWP_VIRTUAL_MACHINE_INSTANCE_COUNTS:
    return instance_counts_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_VIRTUAL_MACHINE_ALL_MODULES:
    return all_modules_serialize(buf, len, command, type, id_sizes, id);
  default:
    return JDWP_LIB_ERR_UNKNOWN_COMMAND;
  }
}

JdwpLibError vm_command_deserialize(DeserializationContext *ctx) {
  switch (ctx->type) {
  case JDWP_VIRTUAL_MACHINE_VERSION:
    return version_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE:
    return classes_by_signature_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_ALL_CLASSES:
    return all_classes_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_ALL_THREADS:
    return all_threads_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_TOP_LEVEL_THREAD_GROUPS:
    return top_level_thread_groups_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_DISPOSE:
    return dispose_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_ID_SIZES:
    return id_sizes_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_SUSPEND:
    return suspend_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_RESUME:
    return resume_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_EXIT:
    return exit_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_CREATE_STRING:
    return create_string_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_CAPABILITIES:
    return capabilities_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_CLASS_PATHS:
    return class_paths_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_DISPOSE_OBJECTS:
    return dispose_objects_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_HOLD_EVENTS:
    return hold_events_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_RELEASE_EVENTS:
    return release_events_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_CAPABILITIES_NEW:
    return capabilities_new_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_REDEFINE_CLASSES:
    return redefine_classes_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_SET_DEFAULT_STRATUM:
    return set_default_stratum_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_ALL_CLASSES_WITH_GENERIC:
    return all_classes_with_generic_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_INSTANCE_COUNTS:
    return instance_counts_deserialize(ctx);
  case JDWP_VIRTUAL_MACHINE_ALL_MODULES:
    return all_modules_deserialize(ctx);
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
    break;
  case JDWP_VIRTUAL_MACHINE_HOLD_EVENTS:
    hold_events_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_RELEASE_EVENTS:
    release_events_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_CAPABILITIES_NEW:
    capabilities_new_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_REDEFINE_CLASSES:
    redefine_classes_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_SET_DEFAULT_STRATUM:
    set_default_stratum_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_ALL_CLASSES_WITH_GENERIC:
    all_classes_with_generic_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_INSTANCE_COUNTS:
    instance_counts_free(reply);
    break;
  case JDWP_VIRTUAL_MACHINE_ALL_MODULES:
    all_modules_free(reply);
    break;
  default:;
  }
}