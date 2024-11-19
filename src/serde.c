#include "serde.h"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

void serde_write_uint8(uint8_t *buffer, uint8_t value) {
  memcpy(buffer, &value, sizeof(uint8_t));
}

void serde_write_uint16(uint8_t *buffer, uint16_t value) {
  uint16_t converted = htons(value);
  memcpy(buffer, &converted, sizeof(uint16_t));
}

void serde_write_uint32(uint8_t *buffer, uint32_t value) {
  uint32_t converted = htonl(value);
  memcpy(buffer, &converted, sizeof(uint32_t));
}
void serde_write_uint64(uint8_t *buffer, uint64_t value) {
  int needs_swap = htonl(1) != 1;
  uint32_t low = value >> 32;
  uint32_t high = value & 0xFFFFFFFFULL;

  if (needs_swap) {
    serde_write_uint32(buffer, low);
    serde_write_uint32(buffer + 4, high);
  }
}

void serde_write_uint8_adv(uint8_t **buffer, uint8_t value) {
  serde_write_uint8(*buffer, value);
  (*buffer)++;
}

void serde_write_uint16_adv(uint8_t **buffer, uint16_t value) {
  serde_write_uint16(*buffer, value);
  *buffer += 2;
}

void serde_write_uint32_adv(uint8_t **buffer, uint32_t value) {
  serde_write_uint32(*buffer, value);
  *buffer += 4;
}

void serde_write_uint64_adv(uint8_t **buffer, uint64_t value) {
  serde_write_uint64(*buffer, value);
  *buffer += 8;
}

void serde_write_string(uint8_t *buffer, char *value) {
  uint32_t len = strlen(value);
  serde_write_uint32(buffer, len);
  memcpy(buffer + 4, value, len);
}
void serde_write_string_adv(uint8_t **buffer, char *value) {
  serde_write_string(*buffer, value);
  *buffer += 4 + strlen(value);
}

uint32_t serde_read_uint32(uint8_t *ptr) { return ntohl(*(uint32_t *)ptr); }
uint64_t serde_read_uint64(uint8_t *ptr) {
  int needs_swap = htonl(1) != 1;
  uint32_t left = serde_read_uint32(ptr);
  uint32_t right = serde_read_uint32(ptr + 4);
  if (needs_swap) {
    return (uint64_t)right + ((uint64_t)left << 32);
  }
  return ((uint64_t)right << 32) + (uint64_t)left;
}

uint8_t serde_read_uint8_adv(uint8_t **ptr) {
  uint8_t value = serde_read_uint8(*ptr);
  *ptr += 1;
  return value;
}

uint16_t serde_read_uint16_adv(uint8_t **ptr) {
  uint16_t value = serde_read_uint16(*ptr);
  *ptr += 2;
  return value;
}

uint32_t serde_read_uint32_adv(uint8_t **ptr) {
  uint32_t value = serde_read_uint32(*ptr);
  *ptr += 4;
  return value;
}

uint64_t serde_read_uint64_adv(uint8_t **ptr) {
  uint64_t value = serde_read_uint64(*ptr);
  *ptr += 8;
  return value;
}

uint16_t serde_read_uint16(uint8_t *ptr) { return ntohs(*(uint16_t *)ptr); }

uint8_t serde_read_uint8(uint8_t *ptr) { return *ptr; }

char *serde_read_string(uint8_t *ptr) {
  uint32_t str_len = serde_read_uint32(ptr);
  char *str = malloc(str_len + 1);
  memcpy(str, ptr + 4, str_len);
  str[str_len] = '\0';

  return str;
}

char *serde_read_string_adv(uint8_t **ptr) {
  uint32_t str_len = serde_read_uint32(*ptr);
  char *str = malloc(str_len + 1);
  memcpy(str, *ptr + 4, str_len);
  str[str_len] = '\0';

  *ptr = *ptr + (4 + str_len);

  return str;
}

void serde_write_variable(uint8_t *buffer, uint64_t value, uint32_t size) {
  switch (size) {
  case 1:
    serde_write_uint8(buffer, value & 0xFF);
    break;
  case 2:
    serde_write_uint16(buffer, value & 0xFFFF);
    break;
  case 4:
    serde_write_uint32(buffer, value & 0xFFFFFFFF);
    break;
  case 8:
    serde_write_uint64(buffer, value);
    break;
  default:;
    // TODO: ERROR
  }
}

void serde_write_variable_adv(uint8_t **buffer, uint64_t value, uint32_t size) {
  switch (size) {
  case 1:
    serde_write_uint8_adv(buffer, value & 0xFF);
    break;
  case 2:
    serde_write_uint16_adv(buffer, value & 0xFFFF);
    break;
  case 4:
    serde_write_uint32_adv(buffer, value & 0xFFFFFFFF);
    break;
  case 8:
    serde_write_uint64_adv(buffer, value);
    break;
  default:;
    // TODO: ERROR
  }
}

uint64_t serde_read_variable(uint8_t *ptr, uint32_t size) {
  switch (size) {
  case 1:
    return serde_read_uint8(ptr);
  case 2:
    return serde_read_uint16(ptr);
  case 4:
    return serde_read_uint32(ptr);
  case 8:
    return serde_read_uint64(ptr);
  default:
    return 0;
    // TODO: ERROR
  }
}

uint64_t serde_read_variable_adv(uint8_t **ptr, uint32_t size) {
  switch (size) {
  case 1:
    return serde_read_uint8_adv(ptr);
  case 2:
    return serde_read_uint16_adv(ptr);
  case 4:
    return serde_read_uint32_adv(ptr);
  case 8:
    return serde_read_uint64_adv(ptr);
  default:
    return 0;
    // TODO: ERROR
  }
}
