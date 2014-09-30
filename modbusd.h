#ifndef MODBUSD_H
#define MODBUSD_H

#include <stdint.h>
#include <stdbool.h>

#define REG_COUNT 128
extern unsigned char modRegisters[REG_COUNT];//These are the modbus registers where control data is stored

struct modd_state {
  uint8_t	idle_count;
  uint8_t	data_count;
  uint8_t	state;
  uint8_t	request_type;
  const uint8_t	*xmit_buf;
  uint16_t	xmit_buf_size;
  uint16_t	offset;
  bool		done;
};

void modbusd_appcall(void);
void modbusd_init(void);

#endif
