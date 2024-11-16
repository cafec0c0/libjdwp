#include "msg/virtual_machine/all_classes.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.object_id_size = 8};

static void test_all_classes_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpLibError e = all_classes_serialize(
      &buf, &bytes_written, NULL, JDWP_VIRTUAL_MACHINE_ALL_CLASSES, NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\003";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_all_classes_deserialize(void **state) {
  uint8_t vm_reply[] =
      "\000\000\000\331\000\000\000\001\200\000\000\000\000\000\003\001\000\000"
      "\000\000\000\000\000\001\000\000\0003Ljava/lang/invoke/"
      "LambdaForm$MH.0x0000726e17140800;"
      "\000\000\000\a\001\000\000\000\000\000\000\000\002\000\000\0004Ljava/"
      "lang/invoke/LambdaForm$DMH.0x0000726e17140400;"
      "\000\000\000\a\001\000\000\000\000\000\000\000\003\000\000\0004Ljava/"
      "lang/invoke/LambdaForm$DMH.0x0000726e17140000;"
      "\000\000\000\a";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_ALL_CLASSES,
      .id_sizes = &id_sizes,
  };
  JdwpLibError e = all_classes_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_ALL_CLASSES);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineAllClassesData *data = reply->data;

  assert_int_equal(data->classes, 3);
  assert_int_equal(data->classes_data[0].ref_type_tag, 1);
  assert_int_equal(data->classes_data[0].type_id, 1);
  assert_int_equal(data->classes_data[0].status,
                   JDWP_CLASS_STATUS_VERIFIED | JDWP_CLASS_STATUS_PREPARED |
                       JDWP_CLASS_STATUS_INITIALIZED);
  assert_string_equal(data->classes_data[0].signature,
                      "Ljava/lang/invoke/LambdaForm$MH.0x0000726e17140800;");

  all_classes_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_all_classes_serialize),
      cmocka_unit_test(test_all_classes_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}