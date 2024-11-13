#ifndef REFERENCE_TYPE_H
#define REFERENCE_TYPE_H
#include <jdwp.h>

// Signature
typedef struct {
  uint64_t ref_type;
} JdwpReferenceTypeSignatureCommand;

typedef struct {
  char *signature;
} JdwpReferenceTypeSignatureData;

// Class loader
typedef struct {
  uint64_t ref_type;
} JdwpReferenceTypeClassLoaderCommand;

typedef struct {
  uint64_t class_loader_id;
} JdwpReferenceTypeClassLoaderData;

// Modifiers
typedef struct {
  uint64_t ref_type;
} JdwpReferenceTypeModifiersCommand;

typedef struct {
  uint32_t mod_bits;
} JdwpReferenceTypeModifiersData;

// Fields
typedef struct {
  uint64_t ref_type;
} JdwpReferenceTypeFieldsCommand;

typedef struct {
  uint64_t field_id;
  char *name;
  char *signature;
  uint32_t mod_bits;
} JdwpReferenceTypeFieldsFieldData;

typedef struct {
  uint32_t declared;
  JdwpReferenceTypeFieldsFieldData *declared_data;
} JdwpReferenceTypeFieldsData;

// Methods
typedef struct {
  uint64_t ref_type;
} JdwpReferenceTypeMethodsCommand;

typedef struct {
  uint64_t method_id;
  char *name;
  char *signature;
  uint32_t mod_bits;
} JdwpReferenceTypeMethodsMethodData;

typedef struct {
  uint32_t declared;
  JdwpReferenceTypeMethodsMethodData *declared_data;
} JdwpReferenceTypeMethodsData;

// Get values
typedef struct {
  uint64_t ref_type;
  uint32_t fields;
  uint64_t *fields_data;
} JdwpReferenceTypeGetValuesCommand;

typedef struct {
  void *value;
} JdwpReferenceTypeGetValuesValueData;

typedef struct {
  uint32_t values;
  JdwpReferenceTypeGetValuesValueData *values_data;
} JdwpReferenceTypeGetValuesData;

#endif // REFERENCE_TYPE_H
