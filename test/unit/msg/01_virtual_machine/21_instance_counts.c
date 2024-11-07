#include "msg/virtual_machine/instance_counts.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.reference_type_id_size = 8};

static void test_instance_counts_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;

  uint64_t ref = 1;
  JdwpVirtualMachineInstanceCountsCommand cmd = {
      .ref_types_count = 1,
      .ref_types_data = &ref,
  };
  JdwpLibError e = instance_counts_serialize(
      &buf, &bytes_written, &cmd, JDWP_VIRTUAL_MACHINE_INSTANCE_COUNTS,
      &id_sizes, 1);

  uint8_t expected[] = "\000\000\000\027\000\000\000\001\000\001\025\000\000"
                       "\000\001\000\000\000\000\000\000\000\001";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 23);
  assert_memory_equal(buf, expected, 23);

  free(buf);
}

static void test_instance_counts_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000\027\000\000\000\001\200\000\000\000\000"
                       "\000\001\000\000\000\000\000\000\000\001";

  JdwpReply *reply;
  size_t len;
  JdwpLibError e = instance_counts_deserialize(
      &reply, &len, vm_reply, JDWP_VIRTUAL_MACHINE_INSTANCE_COUNTS, &id_sizes);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_INSTANCE_COUNTS);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineInstanceCountsData *data = reply->data;

  assert_int_equal(data->counts, 1);
  assert_int_equal(data->instance_count_data[0], 1);

  instance_counts_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_instance_counts_serialize),
      cmocka_unit_test(test_instance_counts_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}