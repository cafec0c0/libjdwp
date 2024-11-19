#include "msg/virtual_machine/capabilities.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static void test_capabilities_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpLibError e = capabilities_serialize(
      &buf, &bytes_written, NULL, JDWP_VIRTUAL_MACHINE_CAPABILITIES, NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\014";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_capabilities_deserialize(void **state) {
  uint8_t vm_reply[] = {0x0, 0x0, 0x0, 0x12, 0x0, 0x0, 0x0, 0x1, 0x80,
                        0x0, 0x0, 0x1, 0x1,  0x1, 0x1, 0x1, 0x1, 0x1};

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_CAPABILITIES,
  };
  JdwpLibError e = capabilities_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_CAPABILITIES);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineCapabilitiesData *data = reply->data;
  assert_int_equal(data->can_watch_field_modification, 1);
  assert_int_equal(data->can_watch_field_access, 1);
  assert_int_equal(data->can_get_bytecodes, 1);
  assert_int_equal(data->can_get_synthetic_attribute, 1);
  assert_int_equal(data->can_get_owned_monitor_info, 1);
  assert_int_equal(data->can_get_current_contended_monitor, 1);
  assert_int_equal(data->can_get_monitor_info, 1);

  capabilities_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_capabilities_serialize),
      cmocka_unit_test(test_capabilities_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}