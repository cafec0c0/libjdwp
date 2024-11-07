#ifndef SERDE_H
#define SERDE_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  size_t field_id_size;
  size_t method_id_size;
  size_t object_id_size;
  size_t reference_type_id_size;
  size_t frame_id_size;
} IdSizes;

void serde_write_uint8(uint8_t *buffer, uint8_t value);
void serde_write_uint16(uint8_t *buffer, uint16_t value);
void serde_write_uint32(uint8_t *buffer, uint32_t value);
void serde_write_uint64(uint8_t *buffer, uint64_t value);

void serde_write_uint8_adv(uint8_t **buffer, uint8_t value);
void serde_write_uint16_adv(uint8_t **buffer, uint16_t value);
void serde_write_uint32_adv(uint8_t **buffer, uint32_t value);
void serde_write_uint64_adv(uint8_t **buffer, uint64_t value);

uint8_t serde_read_uint8_adv(uint8_t **ptr);
uint16_t serde_read_uint16_adv(uint8_t **ptr);
uint32_t serde_read_uint32_adv(uint8_t **ptr);
uint64_t serde_read_uint64_adv(uint8_t **ptr);

uint8_t serde_read_uint8(uint8_t *ptr);
uint16_t serde_read_uint16(uint8_t *ptr);
uint32_t serde_read_uint32(uint8_t *ptr);
uint64_t serde_read_uint64(uint8_t *ptr);

void serde_write_string(uint8_t *buffer, char *value);
void serde_write_string_adv(uint8_t **buffer, char *value);
char *serde_read_string(uint8_t *ptr);
char *serde_read_string_adv(uint8_t **ptr);

void serde_write_variable(uint8_t *buffer, uint64_t value, uint32_t size);
void serde_write_variable_adv(uint8_t **buffer, uint64_t value, uint32_t size);

uint64_t serde_read_variable(uint8_t *ptr, uint32_t size);
uint64_t serde_read_variable_adv(uint8_t **ptr, uint32_t size);

#endif // SERDE_H
