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
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_CAPABILITIES_NEW);

  JdwpVirtualMachineCapabilitiesNewData *data = reply->data;
  assert_in_range(data->can_watch_field_modification, 0, 1);
  assert_in_range(data->can_watch_field_access, 0, 1);
  assert_in_range(data->can_get_bytecodes, 0, 1);
  assert_in_range(data->can_get_synthetic_attribute, 0, 1);
  assert_in_range(data->can_get_owned_monitor_info, 0, 1);
  assert_in_range(data->can_get_current_contended_monitor, 0, 1);
  assert_in_range(data->can_get_monitor_info, 0, 1);
  assert_in_range(data->can_redefine_classes, 0, 1);
  assert_in_range(data->can_add_method, 0, 1);
  assert_in_range(data->can_unrestrictedly_redefine_classes, 0, 1);
  assert_in_range(data->can_pop_frames, 0, 1);
  assert_in_range(data->can_use_instance_filters, 0, 1);
  assert_in_range(data->can_get_source_debug_extensions, 0, 1);
  assert_in_range(data->can_request_vm_death_event, 0, 1);
  assert_in_range(data->can_set_default_stratum, 0, 1);
  assert_in_range(data->can_get_instance_info, 0, 1);
  assert_in_range(data->can_request_monitor_events, 0, 1);
  assert_in_range(data->can_get_monitor_frame_info, 0, 1);
  assert_in_range(data->can_use_source_name_filters, 0, 1);
  assert_in_range(data->can_get_constant_pool, 0, 1);
  assert_in_range(data->can_force_early_return, 0, 1);
  assert_in_range(data->reserved_22, 0, 1);
  assert_in_range(data->reserved_23, 0, 1);
  assert_in_range(data->reserved_24, 0, 1);
  assert_in_range(data->reserved_25, 0, 1);
  assert_in_range(data->reserved_26, 0, 1);
  assert_in_range(data->reserved_27, 0, 1);
  assert_in_range(data->reserved_28, 0, 1);
  assert_in_range(data->reserved_29, 0, 1);
  assert_in_range(data->reserved_30, 0, 1);
  assert_in_range(data->reserved_31, 0, 1);
  assert_in_range(data->reserved_32, 0, 1);

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

  JdwpVirtualMachineCapabilitiesNewCommand cmd;
  uint32_t id;
  err = jdwp_client_send(client, &id, JDWP_VIRTUAL_MACHINE_CAPABILITIES_NEW,
                         &cmd);
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
