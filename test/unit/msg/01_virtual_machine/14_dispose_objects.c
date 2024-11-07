#include "msg/virtual_machine/dispose_objects.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.object_id_size = 8};

static void test_dispose_objects_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;

  JdwpVirtualMachineDisposeObjectsObjectData obj = {
      .ref_cnt = 2,
      .object = 0x34,
  };
  JdwpVirtualMachineDisposeObjectsCommand cmd = {
      .requests = 1,
      .requests_data = &obj,
  };
  JdwpLibError e = dispose_objects_serialize(
      &buf, &bytes_written, &cmd, JDWP_VIRTUAL_MACHINE_DISPOSE_OBJECTS,
      &id_sizes, 1);

  uint8_t expected[] =
      "\000\000\000\033\000\000\000\001\000\001\016\000\000\000\001\000\000\000"
      "\000\000\000\000\064\000\000\000\002";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 27);
  assert_memory_equal(buf, expected, 27);

  free(buf);
}

static void test_dispose_objects_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000\v\000\000\000\001\200\000\000";

  JdwpReply *reply;
  size_t len;
  JdwpLibError e = dispose_objects_deserialize(
      &reply, &len, vm_reply, JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE,
      &id_sizes);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  dispose_objects_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_dispose_objects_serialize),
      cmocka_unit_test(test_dispose_objects_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}