#include "msg/virtual_machine/set_default_stratum.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.reference_type_id_size = 8};

static void test_set_default_stratum_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineSetDefaultStratumCommand cmd = {.stratum_id = "stratum"};
  JdwpLibError e = set_default_stratum_serialize(
      &buf, &bytes_written, &cmd, JDWP_VIRTUAL_MACHINE_SET_DEFAULT_STRATUM,
      NULL, 1);

  uint8_t expected[] =
      "\000\000\000\026\000\000\000\001\000\001\023\000\000\000\007stratum";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 22);
  assert_memory_equal(buf, expected, 22);

  free(buf);
}

static void test_set_default_stratum_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000\v\000\000\000\001\200\000\000";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_SET_DEFAULT_STRATUM,
      .id_sizes = &id_sizes,
  };
  JdwpLibError e = set_default_stratum_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_SET_DEFAULT_STRATUM);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  set_default_stratum_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_set_default_stratum_serialize),
      cmocka_unit_test(test_set_default_stratum_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}