#include "msg/reference_type/signature.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

static IdSizes id_sizes = {.reference_type_id_size = 8};

static void test_signature_serialize(void **state) {
  uint8_t *buf = NULL;
  size_t bytes_written;
  JdwpReferenceTypeSignatureCommand cmd = {.ref_type = 1};
  JdwpLibError e = ref_type_signature_serialize(
      &buf, &bytes_written, &cmd, JDWP_REFERENCE_TYPE_SIGNATURE, &id_sizes, 1);

  uint8_t expected[] = "\000\000\000\023\000\000\000\001\000\002\001\000\000"
                       "\000\000\000\000\000\001";

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(buf);
  assert_int_equal(bytes_written, 19);
  assert_memory_equal(buf, expected, 19);

  free(buf);
}

static void test_signature_deserialize(void **state) {
  uint8_t vm_reply[] =
      "\000\000\000!\000\000\000\001\200\000\000\000\000\000\022Ljava/lang/"
      "String;";

  JdwpReply *reply;
  size_t len;
  JdwpLibError e = ref_type_signature_deserialize(
      &reply, &len, vm_reply, JDWP_REFERENCE_TYPE_SIGNATURE, &id_sizes);

  assert_int_equal(e, JDWP_LIB_ERR_NONE);
  assert_non_null(reply);
  assert_int_equal(reply->id, 1);
  assert_int_equal(reply->type, JDWP_REFERENCE_TYPE_SIGNATURE);
  assert_int_equal(reply->error, 0);
  assert_non_null(reply->data);

  JdwpReferenceTypeSignatureData *data = reply->data;
  assert_string_equal(data->signature, "Ljava/lang/String;");

  ref_type_signature_free(reply);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_signature_serialize),
      cmocka_unit_test(test_signature_deserialize),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}