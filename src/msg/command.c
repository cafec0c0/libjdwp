#include "command.h"

#include "virtual_machine/virtual_machine.h"

JdwpLibError command_serialize(uint8_t **buf, size_t *len, void *command,
                               JdwpCommandType type, IdSizes *id_sizes,
                               uint32_t id) {
  if (!buf)
    return JDWP_LIB_ERR_NULL_POINTER;

  // serialize by command set
  switch (type & 0xFF00) {
  case 0x0100:
    return vm_command_serialize(buf, len, command, type, id_sizes, id);
  default:
    return JDWP_LIB_ERR_UNKNOWN_COMMAND_SET;
  }
}

void command_write_header(uint8_t *buf, size_t msg_len, JdwpCommandType type,
                          uint32_t id) {
  uint8_t *ptr = buf;
  serde_write_uint32_adv(&ptr, msg_len);
  serde_write_uint32_adv(&ptr, id);
  serde_write_uint8_adv(&ptr, 0);
  serde_write_uint8_adv(&ptr, (type & 0xFF00) >> 8);
  serde_write_uint8_adv(&ptr, type & 0xFF);
}