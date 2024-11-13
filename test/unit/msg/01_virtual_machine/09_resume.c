#include "msg/virtual_machine/resume.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static void test_resume_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineResumeCommand cmd = {};
  JdwpLibError e = resume_serialize(&buf, &bytes_written, &cmd,
                                    JDWP_VIRTUAL_MACHINE_RESUME, NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\011";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_resume_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000\013\000\000\000\001\200\000\000";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_RESUME,
  };
  JdwpLibError e = resume_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_RESUME);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  resume_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_resume_serialize),
      cmocka_unit_test(test_resume_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}