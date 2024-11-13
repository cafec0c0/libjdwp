#include "msg/virtual_machine/classes_by_signature.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.reference_type_id_size = 8};

static void test_classes_by_signature_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineClassesBySignatureCommand cmd = {.signature =
                                                         "Ljava/lang/String;"};
  JdwpLibError e = classes_by_signature_serialize(
      &buf, &bytes_written, &cmd, JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE,
      NULL, 1);

  uint8_t expected[] =
      "\000\000\000!\000\000\000\001\000\001\002\000\000\000\022Ljava/lang/"
      "String;";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 33);
  assert_memory_equal(buf, expected, 33);

  free(buf);
}

static void test_classes_by_signature_deserialize(void **state) {
  uint8_t vm_reply[] =
      "\000\000\000\034\000\000\000\001\200\000\000\000\000\000\001\001\000\000"
      "\000\000\000\000\000\001\000\000\000\a";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE,
      .id_sizes = &id_sizes,
  };
  JdwpLibError e = classes_by_signature_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_CLASSES_BY_SIGNATURE);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineClassesBySignatureData *data = reply->data;

  assert_int_equal(data->classes, 1);
  assert_int_equal(data->classes_data[0].ref_type_tag, 1);
  assert_int_equal(data->classes_data[0].type_id, 1);
  assert_int_equal(data->classes_data[0].status, 7);

  classes_by_signature_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_classes_by_signature_serialize),
      cmocka_unit_test(test_classes_by_signature_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}