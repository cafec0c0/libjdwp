#include "msg/virtual_machine/id_sizes.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static void test_id_sizes_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpLibError e = id_sizes_serialize(&buf, &bytes_written, NULL,
                                      JDWP_VIRTUAL_MACHINE_ID_SIZES, NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\007";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_id_sizes_deserialize(void **state) {
  uint8_t vm_reply[] = "\x000\000\000\037\000\000\000\001\200\000\000\000\000"
                       "\000\b\000\000\000\b"
                       "\000\000\000\b\000\000\000\b\000\000\000\b";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_ID_SIZES,
  };
  JdwpLibError e = id_sizes_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_ID_SIZES);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineIdSizesData *data = reply->data;
  assert_int_equal(data->object_id_size, 8);
  assert_int_equal(data->field_id_size, 8);
  assert_int_equal(data->method_id_size, 8);
  assert_int_equal(data->reference_type_id_size, 8);
  assert_int_equal(data->frame_id_size, 8);

  id_sizes_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_id_sizes_serialize),
      cmocka_unit_test(test_id_sizes_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}