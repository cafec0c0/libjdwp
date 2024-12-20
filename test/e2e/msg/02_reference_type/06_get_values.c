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
  int has_field_id;
  uint64_t field_id;
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

  if (reply->type == JDWP_REFERENCE_TYPE_FIELDS) {
    JdwpReferenceTypeFieldsData *data = reply->data;
    s->has_field_id = 1;
    s->field_id = data->declared_data[0].field_id; // Field 0 is not static
    return;
  }

  s->should_exit = 1;
  assert_int_equal(reply->type, JDWP_REFERENCE_TYPE_GET_VALUES);
  JdwpReferenceTypeGetValuesData *data = reply->data;
  // assert_true(data->values_data[0].field_id > 0);
  // assert_string_equal(data->declared_data[0].name, "<init>");
  // assert_string_equal(data->declared_data[0].signature, "()V");
  // assert_int_equal(data->declared_data[0].mod_bits, JDWP_METHOD_ACC_PUBLIC);
  //
  // assert_true(data->declared_data[1].method_id > 0);
  // assert_string_equal(data->declared_data[1].name, "square");
  // assert_string_equal(data->declared_data[1].signature, "(I)I");
  // assert_int_equal(data->declared_data[1].mod_bits, JDWP_METHOD_ACC_PUBLIC);

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

  uint32_t id;

  // Get reference for testing
  JdwpVirtualMachineClassesBySignatureCommand c_cmd = {.signature =
                                                           "LAnotherClass;"};
  err = jdwp_client_send(client, &id, JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE,
                         &c_cmd);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  while (!((State *)*state)->has_ref) {
  }

  // Get a field
  JdwpReferenceTypeFieldsCommand f_cmd = {.ref_type = ((State *)*state)->ref};
  err = jdwp_client_send(client, &id, JDWP_REFERENCE_TYPE_FIELDS, &f_cmd);
  assert_int_equal(err, JDWP_LIB_ERR_NONE);

  while (!((State *)*state)->has_field_id) {
  }

  uint64_t f = ((State *)*state)->field_id;
  JdwpReferenceTypeGetValuesCommand cmd = {
      .ref_type = ((State *)*state)->ref,
      .fields = 1,
      .fields_data = &f,
  };
  err = jdwp_client_send(client, &id, JDWP_REFERENCE_TYPE_GET_VALUES, &cmd);
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
