#include "msg/reference_type/fields.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.reference_type_id_size = 8, .field_id_size = 8};

static void test_fields_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpReferenceTypeFieldsCommand cmd = {.ref_type = 1};
  JdwpLibError e = ref_type_fields_serialize(
      &buf, &bytes_written, &cmd, JDWP_REFERENCE_TYPE_FIELDS, &id_sizes, 1);

  uint8_t expected[] = "\000\000\000\023\000\000\000\001\000\002\004\000\000"
                       "\000\000\000\000\000\001";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 19);
  assert_memory_equal(buf, expected, 19);

  free(buf);
}

static void test_fields_deserialize(void **state) {
  uint8_t vm_reply[] =
      "\000\000\000L\000\000\000\001\200\000\000\000\000\000\002\000\000\000"
      "\000\000\000\0002\000\000\000\001a\000\000\000\001I\000\000\000\002\000"
      "\000u\355\314\000'P\000\000\000\001b\000\000\000\022Ljava/lang/"
      "String;\000\000\000\t";

  JdwpReply *reply;
  DeserializationContext ctx = {
      .reply = &reply,
      .bytes = vm_reply,
      .type = JDWP_REFERENCE_TYPE_FIELDS,
      .id_sizes = &id_sizes,
  };
  JdwpLibError e = ref_type_fields_deserialize(&ctx);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_REFERENCE_TYPE_FIELDS);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpReferenceTypeFieldsData *data = reply->data;

  assert_int_equal(data->declared, 2);
  assert_int_equal(data->declared_data[0].field_id, 50);
  assert_string_equal(data->declared_data[0].name, "a");
  assert_string_equal(data->declared_data[0].signature, "I");
  assert_int_equal(data->declared_data[0].mod_bits, JDWP_FIELD_ACC_PRIVATE);

  assert_int_equal(data->declared_data[1].field_id, 129664190261072);
  assert_string_equal(data->declared_data[1].name, "b");
  assert_string_equal(data->declared_data[1].signature, "Ljava/lang/String;");
  assert_int_equal(data->declared_data[1].mod_bits,
                   JDWP_FIELD_ACC_PUBLIC | JDWP_FIELD_ACC_STATIC);

  ref_type_fields_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_fields_serialize),
      cmocka_unit_test(test_fields_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}