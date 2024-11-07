#include "msg/virtual_machine/top_level_thread_groups.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.object_id_size = 8};

static void test_top_level_thread_groups_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineAllThreadsCommand cmd = {};
  JdwpLibError e = top_level_thread_groups_serialize(
      &buf, &bytes_written, &cmd, JDWP_VIRTUAL_MACHINE_TOP_LEVEL_THREAD_GROUPS,
      NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\005";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_top_level_thread_groups_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000\027\000\000\000\001\200\000\000\000\000"
                       "\000\001\000\000\000\000\000\000\000\001";

  JdwpReply *reply;
  size_t len;
  JdwpLibError e = top_level_thread_groups_deserialize(
      &reply, &len, vm_reply, JDWP_VIRTUAL_MACHINE_TOP_LEVEL_THREAD_GROUPS,
      &id_sizes);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_TOP_LEVEL_THREAD_GROUPS);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineTopLevelThreadGroupsData *data = reply->data;

  assert_int_equal(data->groups, 1);
  for (uint32_t idx = 0; idx < data->groups; idx++)
    assert_true(data->groups_data[idx].group > 0);

  top_level_thread_groups_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_top_level_thread_groups_serialize),
      cmocka_unit_test(test_top_level_thread_groups_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}