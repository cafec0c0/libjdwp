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
    JdwpVirtualMachineClassesBySignatureData *data = reply->data;
    s->has_ref = 1;
    s->ref = data->classes_data[0].type_id;
    return;
  }

  s->should_exit = 1;
  assert_int_equal(reply->type, JDWP_REFERENCE_TYPE_METHODS);
  JdwpReferenceTypeMethodsData *data = reply->data;
  assert_true(data->declared_data[0].method_id > 0);
  assert_string_equal(data->declared_data[0].name, "<init>");
  assert_string_equal(data->declared_data[0].signature, "()V");
  assert_int_equal(data->declared_data[0].mod_bits, JDWP_METHOD_ACC_PUBLIC);

  assert_true(data->declared_data[1].method_id > 0);
  assert_string_equal(data->declared_data[1].name, "square");
  assert_string_equal(data->declared_data[1].signature, "(I)I");
  assert_int_equal(data->declared_data[1].mod_bits, JDWP_METHOD_ACC_PUBLIC);

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

  // Get reference for testing
  JdwpVirtualMachineClassesBySignatureCommand c_cmd = {.signature =
                                                           "LAnotherClass;"};
  err = jdwp_client_send(client, 205, JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE,
                         &c_cmd);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  while (!((State *)*state)->has_ref) {
  }

  JdwpReferenceTypeMethodsCommand cmd = {.ref_type = ((State *)*state)->ref};
  err = jdwp_client_send(client, 205, JDWP_REFERENCE_TYPE_METHODS, &cmd);
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
