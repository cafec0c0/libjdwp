#include "msg/virtual_machine/all_threads.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.object_id_size = 8};

static void test_all_threads_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpLibError e = all_threads_serialize(
      &buf, &bytes_written, NULL, JDWP_VIRTUAL_MACHINE_ALL_THREADS, NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\004";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_all_threads_deserialize(void **state) {
  uint8_t vm_reply[] =
      "\000\000\000?"
      "\000\000\000\001\200\000\000\000\000\000\006\000\000\000\000\000\000\000"
      "\001\000\000\000\000\000\000\000\002\000\000\000\000\000\000\000\003\000"
      "\000\000\000\000\000\000\004\000\000\000\000\000\000\000\005\000\000\000"
      "\000\000\000\000\006";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_ALL_THREADS,
      .id_sizes = &id_sizes,
  };
  JdwpLibError e = all_threads_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_ALL_THREADS);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineAllThreadsData *data = reply->data;

  assert_int_equal(data->threads, 6);
  for (uint32_t idx = 0; idx < data->threads; idx++)
    assert_true(data->threads_data[idx].thread > 0);

  all_threads_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_all_threads_serialize),
      cmocka_unit_test(test_all_threads_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}