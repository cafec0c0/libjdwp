#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>
#include <stdio.h>

#include "jdwp.h"
#include "util/delay.h"

#include <string.h>

typedef struct {
  int should_exit;
} State;

static int setup(void **state) {
  *state = malloc(sizeof(State));
  ((State *)*state)->should_exit = 0;
  return 0;
}

static int teardown(void **state) {
  free(*state);
  return 0;
}

void reply_callback(JdwpReply *reply, void **state) {
  State *s = *state;
  s->should_exit = 1;

  assert_int_equal(reply->error, JDWP_ERR_NONE);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_CLASS_PATHS);

  JdwpVirtualMachineClassPathsData *data = reply->data;
  assert_non_null(data->base_dir);
  assert_true(data->class_paths > 0);
  for (int i = 0; i < data->class_paths; i++)
    assert_non_null(data->class_paths_data[i]);

  for (int i = 0; i < data->boot_class_paths; i++)
    assert_non_null(data->boot_class_paths_data[i]);

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

  JdwpVirtualMachineClassPathsCommand cmd = {};
  uint32_t id;
  err = jdwp_client_send(client, &id, JDWP_VIRTUAL_MACHINE_CLASS_PATHS, &cmd);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  while (!((State *)*state)->should_exit) {
  }

  err = jdwp_client_disconnect(client);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);
  jdwp_client_free(&client);
}

int main(void) {
  delay();
  const struct CMUnitTest tests[] = {
      cmocka_unit_test_setup_teardown(test, setup, teardown)};

  return cmocka_run_group_tests(tests, NULL, NULL);
}
