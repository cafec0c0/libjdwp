#include "msg/virtual_machine/capabilities_new.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static void test_capabilities_new_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineCapabilitiesNewCommand cmd = {};
  JdwpLibError e = capabilities_new_serialize(
      &buf, &bytes_written, &cmd, JDWP_VIRTUAL_MACHINE_CAPABILITIES_NEW, NULL,
      1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\021";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_capabilities_new_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000+"
                       "\000\000\000\001\200\000\000\001\001\001\001\001\001"
                       "\001\001\000\000\001\001\001\001\001\001\001\001\000"
                       "\001\001\000\000\000\000\000\000\000\000\000\000\000";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_CAPABILITIES_NEW,
  };
  JdwpLibError e = capabilities_new_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_CAPABILITIES_NEW);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineCapabilitiesNewData *data = reply->data;
  assert_int_equal(data->can_watch_field_modification, 1);
  assert_int_equal(data->can_watch_field_access, 1);
  assert_int_equal(data->can_get_bytecodes, 1);
  assert_int_equal(data->can_get_synthetic_attribute, 1);
  assert_int_equal(data->can_get_owned_monitor_info, 1);
  assert_int_equal(data->can_get_current_contended_monitor, 1);
  assert_int_equal(data->can_get_monitor_info, 1);
  assert_int_equal(data->can_redefine_classes, 1);
  assert_int_equal(data->can_add_method, 0);
  assert_int_equal(data->can_unrestrictedly_redefine_classes, 0);
  assert_int_equal(data->can_pop_frames, 1);
  assert_int_equal(data->can_use_instance_filters, 1);
  assert_int_equal(data->can_get_source_debug_extensions, 1);
  assert_int_equal(data->can_request_vm_death_event, 1);
  assert_int_equal(data->can_set_default_stratum, 1);
  assert_int_equal(data->can_get_instance_info, 1);
  assert_int_equal(data->can_request_monitor_events, 1);
  assert_int_equal(data->can_get_monitor_frame_info, 1);
  assert_int_equal(data->can_use_source_name_filters, 0);
  assert_int_equal(data->can_get_constant_pool, 1);
  assert_int_equal(data->can_force_early_return, 1);
  assert_int_equal(data->reserved_22, 0);
  assert_int_equal(data->reserved_23, 0);
  assert_int_equal(data->reserved_24, 0);
  assert_int_equal(data->reserved_25, 0);
  assert_int_equal(data->reserved_26, 0);
  assert_int_equal(data->reserved_27, 0);
  assert_int_equal(data->reserved_28, 0);
  assert_int_equal(data->reserved_29, 0);
  assert_int_equal(data->reserved_30, 0);
  assert_int_equal(data->reserved_31, 0);
  assert_int_equal(data->reserved_32, 0);

  capabilities_new_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_capabilities_new_serialize),
      cmocka_unit_test(test_capabilities_new_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}