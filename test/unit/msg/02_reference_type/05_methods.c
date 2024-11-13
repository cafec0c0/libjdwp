#include "msg/reference_type/methods.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.reference_type_id_size = 8, .method_id_size = 8};

static void test_methods_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpReferenceTypeMethodsCommand cmd = {.ref_type = 1};
  JdwpLibError e = ref_type_methods_serialize(
      &buf, &bytes_written, &cmd, JDWP_REFERENCE_TYPE_METHODS, &id_sizes, 1);

  uint8_t expected[] = "\000\000\000\023\000\000\000\001\000\002\005\000\000"
                       "\000\000\000\000\000\001";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 19);
  assert_memory_equal(buf, expected, 19);

  free(buf);
}

static void test_methods_deserialize(void **state) {
  uint8_t vm_reply[] =
      "\000\000\000J\000\000\000\001\200\000\000\000\000\000\002\000\000u\356"
      "\220\030\230\350\000\000\000\006<init>\000\000\000\003()"
      "V\000\000\000\001\000\000u\356\220\030\230\360\000\000\000\006square\000"
      "\000\000\004(I)I\000\000\000\001";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_REFERENCE_TYPE_METHODS,
      .id_sizes = &id_sizes,
  };
  JdwpLibError e = ref_type_methods_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_REFERENCE_TYPE_METHODS);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpReferenceTypeMethodsData *data = reply->data;

  assert_int_equal(data->declared, 2);
  assert_int_equal(data->declared_data[0].method_id, 0x000075ee901898e8);
  assert_string_equal(data->declared_data[0].name, "<init>");
  assert_string_equal(data->declared_data[0].signature, "()V");
  assert_int_equal(data->declared_data[0].mod_bits, JDWP_METHOD_ACC_PUBLIC);

  assert_int_equal(data->declared_data[1].method_id, 0x000075ee901898f0);
  assert_string_equal(data->declared_data[1].name, "square");
  assert_string_equal(data->declared_data[1].signature, "(I)I");
  assert_int_equal(data->declared_data[1].mod_bits, JDWP_METHOD_ACC_PUBLIC);

  ref_type_methods_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_methods_serialize),
      cmocka_unit_test(test_methods_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}