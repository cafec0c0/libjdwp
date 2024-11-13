#include "msg/virtual_machine/version.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static void test_version_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineVersionCommand cmd = {};
  JdwpLibError e = version_serialize(&buf, &bytes_written, &cmd,
                                     JDWP_VIRTUAL_MACHINE_VERSION, NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\001";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_version_deserialize(void **state) {
  uint8_t vm_reply[] =
      "\000\000\000\331\000\000\000\001\200\000\000\000\000\000\240Java Debug "
      "Wire Protocol (Reference Implementation) version 23.0\nJVM Debug "
      "Interface version 23.0\nJVM version 23 (OpenJDK 64-Bit Server VM, mixed "
      "mode, sharing)"
      "\000\000\000\027\000\000\000\000\000\000\000\00223\000\000\000\030OpenJD"
      "K 64-Bit Server VM";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_VERSION,
  };
  JdwpLibError e = version_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_VERSION);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineVersionData *data = reply->data;

  assert_string_equal(
      data->description,
      "Java Debug Wire Protocol (Reference Implementation) version 23.0\nJVM "
      "Debug Interface version 23.0\nJVM version 23 (OpenJDK 64-Bit Server VM, "
      "mixed mode, sharing)");
  assert_int_equal(data->jdwp_major, 23);
  assert_int_equal(data->jdwp_minor, 0);
  assert_string_equal(data->vm_version, "23");
  assert_string_equal(data->vm_name, "OpenJDK 64-Bit Server VM");

  version_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_version_serialize),
      cmocka_unit_test(test_version_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}