#include "msg/virtual_machine/all_classes_with_generic.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.reference_type_id_size = 8};

static void test_all_classes_with_generic_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpVirtualMachineAllClassesCommand cmd = {};
  JdwpLibError e = all_classes_with_generic_serialize(
      &buf, &bytes_written, &cmd, JDWP_VIRTUAL_MACHINE_ALL_CLASSES_WITH_GENERIC,
      NULL, 1);

  uint8_t expected[] = "\000\000\000\013\000\000\000\001\000\001\024";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 11);
  assert_memory_equal(buf, expected, 11);

  free(buf);
}

static void test_all_classes_with_generic_deserialize(void **state) {
  uint8_t vm_reply[] =
      "\000\000\004\000\000\000\000\001\200\000\000\000\000\000\003\001\000\000"
      "\000"
      "\000\000\000\000\001\000\000\000\016LAnotherClass;"
      "\000\000\000\000\000\000\000\a\001\000\000\000\000\000\000\000\002\000"
      "\000\000\016LSimpleServer;"
      "\000\000\000\000\000\000\000\a\001\000\000\000\000\000\000\000\003\000"
      "\000"
      "\000\025Ljava/nio/CharBuffer;\000\000\000\200Ljava/nio/Buffer;Ljava/"
      "lang/Comparable<Ljava/nio/CharBuffer;>;Ljava/lang/Appendable;Ljava/lang/"
      "CharSequence;Ljava/lang/"
      "Readable;"
      "\000\000\000\a";

  JdwpReply *reply;
  size_t len;
  JdwpLibError e = all_classes_with_generic_deserialize(
      &reply, &len, vm_reply, JDWP_VIRTUAL_MACHINE_ALL_CLASSES_WITH_GENERIC,
      &id_sizes);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_VIRTUAL_MACHINE_ALL_CLASSES_WITH_GENERIC);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpVirtualMachineAllClassesWithGenericData *data = reply->data;

  assert_int_equal(data->classes, 3);
  assert_int_equal(data->classes_data[2].ref_type_tag, 1);
  assert_int_equal(data->classes_data[2].type_id, 3);
  assert_int_equal(data->classes_data[2].status,
                   JDWP_CLASS_STATUS_VERIFIED | JDWP_CLASS_STATUS_PREPARED |
                       JDWP_CLASS_STATUS_INITIALIZED);
  assert_string_equal(data->classes_data[2].signature, "Ljava/nio/CharBuffer;");
  assert_string_equal(data->classes_data[2].generic_signature,
                      "Ljava/nio/Buffer;Ljava/lang/Comparable<Ljava/nio/"
                      "CharBuffer;>;Ljava/lang/Appendable;Ljava/lang/"
                      "CharSequence;Ljava/lang/Readable;");

  all_classes_with_generic_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_all_classes_with_generic_serialize),
      cmocka_unit_test(test_all_classes_with_generic_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}