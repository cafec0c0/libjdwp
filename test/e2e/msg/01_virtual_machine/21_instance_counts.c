#include <regex.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>
#include <stdio.h>

#include "jdwp.h"

#include <string.h>

typedef struct {
  int should_exit;
  int has_ref;
  uint64_t ref;
} State;

static int setup(void **state) {
  *state = malloc(sizeof(State));
  ((State *)*state)->should_exit = 0;
  ((State *)*state)->has_ref = 0;
  return 0;
}

static int teardown(void **state) {
  free(*state);
  return 0;
}

void reply_callback(JdwpReply *reply, void **state) {
  State *s = *state;

  assert_int_equal(reply->error, JDWP_ERR_NONE);

  if (reply->type == JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE) {
    s->has_ref = 1;
    s->ref = ((JdwpVirtualMachineClassesBySignatureData *)reply->data)
                 ->classes_data[0]
                 .type_id;
    return;
  }

  s->should_exit = 1;
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_INSTANCE_COUNTS);

  JdwpVirtualMachineInstanceCountsData *data = reply->data;

  assert_true(data->counts > 0);
  for (int i = 0; i < data->counts; i++)
    assert_true(data->instance_count_data[i] > 0);

  jdwp_reply_free(&reply);
}

static void test(void **state) {
  JdwpClient client;
  JdwpLibError err = jdwp_client_new(&client);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  err = jdwp_client_set_callback(client, reply_callback, state);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  err = jdwp_client_connect(client, "127.0.0.1", 8000);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  // Find our class
  JdwpVirtualMachineClassesBySignatureCommand c_cmd = {.signature =
                                                           "LAnotherClass;"};

  jdwp_client_send(client, 121, JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE,
                   &c_cmd);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  while (!((State *)*state)->has_ref) {
  }

  uint64_t ref_id[] = {((State *)*state)->ref};
  JdwpVirtualMachineInstanceCountsCommand cmd = {
      .ref_types_count = 1,
      .ref_types_data = ref_id,
  };

  err =
      jdwp_client_send(client, 122, JDWP_VIRTUAL_MACHINE_INSTANCE_COUNTS, &cmd);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  while (!((State *)*state)->should_exit) {
  }

  err = jdwp_client_disconnect(client);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);
  jdwp_client_free(&client);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup_teardown(test, setup, teardown)};

  return cmocka_run_group_tests(tests, NULL, NULL);
}
