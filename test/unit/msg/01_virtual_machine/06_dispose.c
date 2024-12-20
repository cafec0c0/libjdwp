#include "msg/virtual_machine/dispose.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static void test_dispose_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineDisposeCommand cmd = {};
  JdwpLibError e = dispose_serialize(&buf, &bytes_written, &cmd,
                                     JDWP_VIRTUAL_MACHINE_DISPOSE, NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\006";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_dispose_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000\013\000\000\000\001\200\000\000";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_DISPOSE,
  };
  JdwpLibError e = dispose_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_DISPOSE);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  dispose_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_dispose_serialize),
      cmocka_unit_test(test_dispose_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}