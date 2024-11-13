
#include "class_loader.h"
#include "fields.h"
#include "get_values.h"
#include "methods.h"
#include "modifiers.h"
#include "signature.h"

#include <jdwp.h>

JdwpLibError ref_type_command_serialize(uint8_t **buf, size_t *len,
                                        void *command, JdwpCommandType type,
                                        IdSizes *id_sizes, uint32_t id) {
  switch (type) {
  case JDWP_REFERENCE_TYPE_SIGNATURE:
    return ref_type_signature_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_REFERENCE_TYPE_CLASS_LOADER:
    return ref_type_class_loader_serialize(buf, len, command, type, id_sizes,
                                           id);
  case JDWP_REFERENCE_TYPE_MODIFIERS:
    return ref_type_modifiers_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_REFERENCE_TYPE_FIELDS:
    return ref_type_fields_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_REFERENCE_TYPE_METHODS:
    return ref_type_methods_serialize(buf, len, command, type, id_sizes, id);
  case JDWP_REFERENCE_TYPE_GET_VALUES:
    return ref_type_get_values_serialize(buf, len, command, type, id_sizes, id);
  default:
    return JDWP_LIB_ERR_UNKNOWN_COMMAND;
  }
}

JdwpLibError ref_type_command_deserialize(DeserializationContext *ctx) {
  switch (ctx->type) {
  case JDWP_REFERENCE_TYPE_SIGNATURE:
    return ref_type_signature_deserialize(ctx);
  case JDWP_REFERENCE_TYPE_CLASS_LOADER:
    return ref_type_class_loader_deserialize(ctx);
  case JDWP_REFERENCE_TYPE_MODIFIERS:
    return ref_type_modifiers_deserialize(ctx);
  case JDWP_REFERENCE_TYPE_FIELDS:
    return ref_type_fields_deserialize(ctx);
  case JDWP_REFERENCE_TYPE_METHODS:
    return ref_type_methods_deserialize(ctx);
  case JDWP_REFERENCE_TYPE_GET_VALUES:
    return ref_type_get_values_deserialize(ctx);
  default:
    return JDWP_LIB_ERR_UNKNOWN_COMMAND;
  }
}

void ref_type_reply_free(JdwpReply *reply) {
  switch (reply->type) {
  case JDWP_REFERENCE_TYPE_SIGNATURE:
    ref_type_signature_free(reply);
    break;
  case JDWP_REFERENCE_TYPE_CLASS_LOADER:
    ref_type_class_loader_free(reply);
    break;
  case JDWP_REFERENCE_TYPE_MODIFIERS:
    ref_type_modifiers_free(reply);
    break;
  case JDWP_REFERENCE_TYPE_FIELDS:
    ref_type_fields_free(reply);
    break;
  case JDWP_REFERENCE_TYPE_METHODS:
    ref_type_methods_free(reply);
    break;
  case JDWP_REFERENCE_TYPE_GET_VALUES:
    ref_type_get_values_free(reply);
    break;
  default:;
  }
}