#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>
#include <stdio.h>

#include "jdwp.h"

#include <string.h>
#include <unistd.h>

typedef struct {
  int should_exit;
  uint8_t has_string_id;
  uint64_t string_id;
} State;

static int setup(void **state) {
  *state = malloc(sizeof(State));
  ((State *)*state)->should_exit = 0;
  ((State *)*state)->has_string_id = 0;
  return 0;
}

static int teardown(void **state) {
  free(*state);
  return 0;
}

void reply_callback(JdwpReply *reply, void **state) {
  State *s = *state;

  if (reply->type == JDWP_VIRTUAL_MACHINE_CREATE_STRING) {
    assert_int_equal(reply->error, JDWP_ERR_NONE);
    s->string_id =
        ((JdwpVirtualMachineCreateStringData *)reply->data)->string_object;
    s->has_string_id = 1;
  }

  if (reply->type == JDWP_VIRTUAL_MACHINE_DISPOSE_OBJECTS) {
    assert_int_equal(reply->error, JDWP_ERR_NONE);
    s->should_exit = 1;
  }

  jdwp_reply_free(&reply);
}

static void test(void **state) {
  JdwpClient client;
  State *st = *state;
  JdwpLibError err = jdwp_client_new(&client);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  err = jdwp_client_set_callback(client, reply_callback, state);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  err = jdwp_client_connect(client, "127.0.0.1", 8000);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  // Create a string to dispose
  uint32_t id;
  JdwpVirtualMachineCreateStringCommand s_cmd = {.utf = "new string"};
  err =
      jdwp_client_send(client, &id, JDWP_VIRTUAL_MACHINE_CREATE_STRING, &s_cmd);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  while (!st->has_string_id) {
  }

  JdwpVirtualMachineDisposeObjectsObjectData obj = {
      .ref_cnt = 1,
      .object = st->string_id,
  };
  JdwpVirtualMachineDisposeObjectsCommand cmd = {
      .requests = 1,
      .requests_data = &obj,
  };

  err =
      jdwp_client_send(client, &id, JDWP_VIRTUAL_MACHINE_DISPOSE_OBJECTS, &cmd);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  while (!st->should_exit) {
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
