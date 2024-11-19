#include "msg/virtual_machine/release_events.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static void test_release_events_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpLibError e = release_events_serialize(
      &buf, &bytes_written, NULL, JDWP_VIRTUAL_MACHINE_RELEASE_EVENTS, NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\020";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_release_events_deserialize(void **state) {
  uint8_t vm_reply[] = "\000\000\000\013\000\000\000\001\200\000\000";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_RELEASE_EVENTS,
  };
  JdwpLibError e = release_events_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_RELEASE_EVENTS);
  assert_int_equal(reply->error, 0);
  assert_null(reply->data);

  release_events_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_release_events_serialize),
      cmocka_unit_test(test_release_events_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}