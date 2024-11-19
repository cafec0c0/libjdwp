#include "msg/virtual_machine/class_paths.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static void test_class_paths_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpLibError e = class_paths_serialize(
      &buf, &bytes_written, NULL, JDWP_VIRTUAL_MACHINE_CLASS_PATHS, NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\015";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_class_paths_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000C\000\000\000\001\200\000\000\000\000\000\v"
                       "/home/rogue\000\000\000\001\000\000\000\035/home/rogue/"
                       "simple-server.jar\000\000\000\000";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_CLASS_PATHS,
  };
  JdwpLibError e = class_paths_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_CLASS_PATHS);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineClassPathsData *data = reply->data;
  assert_string_equal(data->base_dir, "/home/rogue");
  assert_int_equal(data->class_paths, 1);
  assert_string_equal(data->class_paths_data[0],
                      "/home/rogue/simple-server.jar");
  assert_int_equal(data->boot_class_paths, 0);

  class_paths_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_class_paths_serialize),
      cmocka_unit_test(test_class_paths_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}