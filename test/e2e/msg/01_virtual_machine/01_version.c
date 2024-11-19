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
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_VERSION);

  JdwpVirtualMachineVersionData *data = reply->data;

  regex_t desc_regex;
  if (regcomp(&desc_regex, "OpenJDK", 0) != 0)
    fail_msg("Unable to compile regex");
  assert_int_equal(regexec(&desc_regex, data->description, 0, NULL, 0), 0);

  assert_true(data->jdwp_major > 0);
  assert_true(strlen(data->vm_version) > 0);
  assert_int_equal(regexec(&desc_regex, data->vm_name, 0, NULL, 0), 0);
  regfree(&desc_regex);

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

  err = jdwp_client_send(client, 101, JDWP_VIRTUAL_MACHINE_VERSION, NULL);
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
