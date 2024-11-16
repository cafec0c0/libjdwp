#include "msg/virtual_machine/all_modules.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.object_id_size = 8};

static void test_all_modules_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpLibError e =
      all_modules_serialize(&buf, &bytes_written, NULL,
                            JDWP_VIRTUAL_MACHINE_ALL_MODULES, &id_sizes, 1);

  uint8_t expected[] = "\000\000\000\v\000\000\000\001\000\001\026";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_all_modules_deserialize(void **state) {
  uint8_t vm_reply[] =
      "\000\000\002\017\000\000\000\001\200\000\000\000\000\000\004"
      "\000\000\000\000\000\000\000\001\000\000\000\000\000\000\000\002\000\000"
      "\000\000\000\000\000\003\000\000\000\000\000\000\000\004";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_ALL_MODULES,
      .id_sizes = &id_sizes,
  };
  JdwpLibError e = all_modules_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_ALL_MODULES);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineAllModulesData *data = reply->data;

  assert_int_equal(data->modules, 4);
  for (uint32_t idx = 0; idx < data->modules; idx++)
    assert_int_equal(data->modules_data[idx], idx + 1);

  all_modules_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_all_modules_serialize),
      cmocka_unit_test(test_all_modules_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}