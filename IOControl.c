/*
 * IOControl.c
 *
 *  Created on: Aug 15, 2014
 *      Author: Ben
 */

#include "IOControl.h"

void timerSetup(){
	 //Set speed to 66MHZ use SYSCTL_SYSDIV_2_5 for 80mhz
	 //SysCtlClockSet(SYSCTL_SYSDIV_3|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	 SysCtlDelay(10);
	 TimerConfigure(WTIMER0_BASE,TIMER_CFG_ONE_SHOT_UP);
	 TimerDisable(WTIMER0_BASE,TIMER_BOTH);
	 //Set the timer to the max value
	 TimerLoadSet64(WTIMER0_BASE, ~0);//Timer isn't 64 bit but this sets both timers at once.
	 TimerEnable(WTIMER0_BASE, TIMER_A);
}


void setupIO()
{
	volatile unsigned long ulLoop;
	//Enable io ports
	SYSCTL_RCGC2_R |= (SYSCTL_RCGC2_GPIOF + SYSCTL_RCGC2_GPIOD + SYSCTL_RCGC2_GPIOB
	  		+ SYSCTL_RCGC2_GPIOA);

	//This just wastes a bit of time
	ulLoop = SYSCTL_RCGC2_R;

    //Setup io ports
	//Data IO
    GPIO_PORTF_LOCK_R = 0x4C4F434B;//Unlock the port F conf
    GPIO_PORTF_DIR_R = 0x01;//Set as output
    GPIO_PORTF_CR_R = 0x00000001;
    GPIO_PORTF_DEN_R = 0x0d;
    GPIO_PORTF_PUR_R = 0x01;
    GPIO_PORTF_DR8R_R = 0x01;
    GPIO_PORTF_LOCK_R = 0;
    GPIO_PORTF_DATA_R = 0x0c;

    //Card select bits
    GPIO_PORTD_DATA_R = 0;
    GPIO_PORTD_LOCK_R = 0x4C4F434B;//Unlock the port conf
    GPIO_PORTD_CR_R =  0x0f;
    GPIO_PORTD_DIR_R = 0x0f;
    GPIO_PORTD_DEN_R = 0x0f;
    GPIO_PORTD_PUR_R = 0x0f;
    GPIO_PORTD_LOCK_R = 0;


    //Bit select and Run bit
    GPIO_PORTB_DATA_R = 0;
    GPIO_PORTB_DIR_R |= 0x17;
    GPIO_PORTB_DEN_R |= 0x17;
    GPIO_PORTB_PUR_R |= 0x17;

	//Chip select b
    GPIO_PORTA_DIR_R = 0xc0 + GPIO_PORTA_DIR_R;
    GPIO_PORTA_DEN_R = 0xc0 + GPIO_PORTA_DEN_R;
    GPIO_PORTA_PUR_R = 0xc0 + GPIO_PORTA_PUR_R;
    GPIO_PORTA_DATA_R = 0;

    GPIOPinWrite(GPIO_PORTA_BASE, 0xC0, 0xC0);
}

void setBitN(unsigned char bitN)
{
	GPIOPinWrite(GPIO_PORTB_BASE, 0x07, ~bitN);
}

unsigned char readBit(unsigned char bit)
{
	unsigned char data, wr;
	unsigned char slot = bit >> 4;
	GPIOPinWrite(GPIO_PORTF_BASE, 0x1, 0);

	SET_INPUT;

	//Decide which slots to read
	//This may need to be expanded to include WRc and WRd
    if(slot < 8){
    	wr = 0x80;
    }else{
    	wr = 0x40;
    	slot -= 10;
    }

	setBitN(bit);
	GPIO_PORTD_DATA_R = slot;
	SysCtlDelay(5);
    GPIOPinWrite(GPIO_PORTA_BASE, 0xC0, wr);//Active LOW

	SysCtlDelay(10);//Delay 30 cycles to allow input to settle in 125nS
	data = GPIO_PORTF_DATA_R & 1;
	GPIOPinWrite(GPIO_PORTA_BASE, 0xC0, 0xC0);
	SET_OUTPUT;

	return !data;
}

void read8Bits(unsigned char slot, unsigned char bits, unsigned char *data)
{
    unsigned char i,wr;

	SET_INPUT;
	GPIO_PORTD_DATA_R = slot;
	*data = 0;

	//Decide which slots to read
    if(slot < 8){
    	wr = 0x80;
    }else{
    	wr = 0x40;
    	slot -= 10;
    }

    for(i = 0; i < 8 ;i++){
      setBitN(i);
      SysCtlDelay(5);
      GPIOPinWrite(GPIO_PORTA_BASE, 0xC0, wr);//Enable write
	  SysCtlDelay(10);//Delay 30 cycles to allow input to settle in 125nS
	  *data |= (GPIO_PORTF_DATA_R & 1) << i;
      GPIOPinWrite(GPIO_PORTA_BASE, 0xC0, 0xC0);//Disable write
    }

    SET_OUTPUT;
	*data = ~*data;
}


void writeBit(unsigned char bit, unsigned char data)
{
	unsigned char wr, slot = bit >> 4;
	SET_OUTPUT;

	//Set output before bit is selected.
	GPIOPinWrite(GPIO_PORTF_BASE, 0x1, data);

	//Decide which slot to write
    if(slot < 8){
    	wr = 0x80;
    }else{
    	wr = 0x40;
    	slot -= 10;
    }

	GPIO_PORTD_DATA_R = slot;
	setBitN(bit);
	SysCtlDelay(5);//Delay 15 cycles
    GPIOPinWrite(GPIO_PORTA_BASE, 0xC0, wr);//enable write

	SysCtlDelay(5);//Delay 15 cycles
	GPIOPinWrite(GPIO_PORTA_BASE, 0xC0, 0xC0);//Disable write
	//SET_INPUT;
}

void write8Bits(unsigned char slot, unsigned char data)
{
	unsigned char i, wr;
	SET_OUTPUT;

    if(slot < 8){
    	wr = 0x80;
    }else{
    	wr = 0x40;
    	slot -= 10;
    }
	GPIO_PORTD_DATA_R = slot;
	SysCtlDelay(5);//Delay 15 cycles

    for(i = 0;i < 8;i++){
      setBitN(i);
	  GPIOPinWrite(GPIO_PORTF_BASE, 0x1, data >> i);//set data bit
	  SysCtlDelay(5);//Delay 15 cycles
	  GPIOPinWrite(GPIO_PORTA_BASE, 0xC0, wr);//Enable write
	  SysCtlDelay(5);//Delay 15 cycles
      GPIOPinWrite(GPIO_PORTA_BASE, 0xC0, 0xC0);//Disable write
      SysCtlDelay(3);//Delay 9 cycles
    }
}

void timer(struct Timer *ctimer, char enable,unsigned long setTime)
{
  unsigned long long ctime = TimerValueGet64(WTIMER0_BASE);

  if(enable && !ctimer->en){//First time after enable goes high
	ctimer->setTime = setTime;
	ctimer->endTime = SysCtlClockGet();
	ctimer->endTime *= ctimer->setTime;
	ctimer->endTime /= 1000;
	ctimer->endTime += ctime;

	ctimer->en = 1;//Mark timer as enabled
	ctimer->dn = 0;
  }else if(enable && ctimer->en){//Timer is already running.
	if((ctimer->endTime <= ctime) && !ctimer->dn){
	   ctimer->dn = 1;//set done
	}
  }else{
	  ctimer->en = 0;
	  ctimer->dn = 0;
  }
}
