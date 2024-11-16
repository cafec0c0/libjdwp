#include "msg/virtual_machine/exit.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static void test_exit_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineExitCommand cmd = {.exit_code = 3};
  JdwpLibError e = exit_serialize(&buf, &bytes_written, &cmd,
                                  JDWP_VIRTUAL_MACHINE_EXIT, NULL, 1);

  uint8_t expected[] =
      "\000\000\000\017\000\000\000\001\000\001\012\000\000\000\003";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 15);
  assert_memory_equal(buf, expected, 15);

  free(buf);
}

static void test_exit_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000\013\000\000\000\001\200\000\000";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_EXIT,
  };
  JdwpLibError e = exit_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_EXIT);
  assert_int_equal(reply->error, 0);
  assert_null(reply->data);

  exit_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_exit_serialize),
      cmocka_unit_test(test_exit_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}