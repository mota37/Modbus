#include "inc/lm4f120h5qr.h"
#include <inc/hw_ints.h>
#include <stdint.h>
//#include "common.h"
//#include "enc28j60.h"
//#include "spi.h"
//#include <uip/uip.h>
//#include <uip/uip_arp.h>
//#include <driverlib/systick.h>
//#include <driverlib/interrupt.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
//#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"

#define SET_RUN_ON GPIOPinWrite(GPIO_PORTB_BASE, 0x10, 0x10);
#define SET_RUN_OFF GPIOPinWrite(GPIO_PORTB_BASE, 0x10, 0);
#define SET_INPUT {GPIO_PORTF_DIR_R = 0x00;}
#define SET_OUTPUT {GPIO_PORTF_DIR_R = 0x01;}

//Stucture to be used with the timer fcn.
struct Timer{
  unsigned char en;
  unsigned char dn;
  unsigned long setTime;
  unsigned long long endTime;
};//timers[8];

//Completes a time delay
//SetTime is only loaded on the transition of enable from 0 to 1
//Updates after enable goes high will be inored until the next transition
void timer(struct Timer * cTimer,char enable, unsigned long setTime);

//Sets up timer hardware
void timerSetup();

void setupIO();

//Sets the bit number on the selected card
//Should not be used directly
void setBitN(unsigned char bitN);

//Read a single input bit
//Input the bit in hex format
//0xXY where X is the slot and Y is the bit
unsigned char readBit(unsigned char bit);

void read8Bits(unsigned char slot, unsigned char bits, unsigned char *data);

//Write a single output bit
//Input the bit in hex format
//0xXY where X is the slot and Y is the bit
void writeBit(unsigned char bit,unsigned char data);

void write8Bits(unsigned char slot, unsigned char data);
