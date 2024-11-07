#include "msg/virtual_machine/create_string.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.object_id_size = 8};

static void test_create_string_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineCreateStringCommand cmd = {.utf = "hello"};
  JdwpLibError e = create_string_serialize(
      &buf, &bytes_written, &cmd, JDWP_VIRTUAL_MACHINE_CREATE_STRING, NULL, 1);

  uint8_t expected[] = {0x0, 0x0, 0x0, 0x14, 0x0, 0x0,  0x0,  0x1,  0x0,  0x1,
                        0xb, 0x0, 0x0, 0x0,  0x5, 0x68, 0x65, 0x6c, 0x6c, 0x6f};

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 20);
  assert_memory_equal(buf, expected, 20);

  free(buf);
}

static void test_create_string_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000\023\000\000\000\001\200\000\000\000\000"
                       "\000\000\000\000\000\001";

  JdwpReply *reply;
  size_t len;
  JdwpLibError e = create_string_deserialize(
      &reply, &len, vm_reply, JDWP_VIRTUAL_MACHINE_CREATE_STRING, &id_sizes);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_CREATE_STRING);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineCreateStringData *data = reply->data;

  assert_int_equal(data->string_object, 1);

  create_string_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_create_string_serialize),
      cmocka_unit_test(test_create_string_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}