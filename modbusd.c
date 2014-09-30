#include "modbusd.h"
#include "uip.h"
//#include "common.h"

#include <stdbool.h>
#include <string.h>

//#define REG_COUNT 128
unsigned char modRegisters[REG_COUNT];//These are the modbus registers where control data is stored

struct MODBUS_HEADER_FIELDS{
	unsigned short transID;
	unsigned short protoID;
	unsigned short length;
	unsigned char unitID;
	unsigned char fcnID;
	unsigned short offset;
	unsigned short size;
};

union MODBUS_HEADER{
	unsigned char bytes[12];
	struct MODBUS_HEADER_FIELDS fields;
}request;

#define CONFIG_NOT_USED		0
#define CONFIG_INPUT		1
#define CONFIG_OUTPUT		2

#define FILE_ROOT		1

#define TRANSFER_SIZE		200

#define HEADER_SIZE	10

#define DATA_BUF ((uint8_t*)(uip_appdata))

void mobdusd_init(void) {
  uip_listen(HTONS(502));
}

void modbusd_appcall(void) {
  //1. Check a packet was received on the correct port
  if(uip_conn->lport != HTONS(502)) {
    uip_abort();
    return;
  }

  //2.
  struct modd_state *hs = (struct modd_state *)&(uip_conn->appstate);
  uint8_t buf[150];

  //3a If newly connected
  //keep
  if(uip_connected()) {
    printf("Connected\n");
    hs->data_count = 0;
    hs->idle_count = 0;
    hs->state = 0;
    hs->done = false;
	//3b Packet received
  } else if(uip_newdata()) {
    printf("New data\n");
	//Copy request header
	memcpy(request.bytes,DATA_BUF,12);//Get all data just in case
    printf("Got get request\n");

	//Parse modbus header here
	unsigned short offset = 0,size = 0;
	memcpy(buf,request.bytes,12);
	offset = (request.fields.offset >> 8)*2;
	size = (request.fields.size >> 8)*2;

  switch(request.fields.fcnID){
	case 3://Read multiple registers TESTED works
		printf("3\n");
		if(size + offset > REG_COUNT){//Send error here
			buf[9] = 2;//Error data out of range
			buf[8] = 0x83;//Give error
			buf[5] = 4;
			hs->xmit_buf_size = 10;
		}else{//send data
			memcpy(buf + 9,&modRegisters[offset],size);
			buf[8] = size;
			buf[5] = 3 + size;
			hs->xmit_buf_size = 9 + size;
		}
		break;
	case 4:///Read Single register UNTESTED Probably works
		printf("4\n");
		memcpy(buf,request.bytes,8);
		//size = 2;
		buf[8] = 2;//Set response size
		if(offset > REG_COUNT){
			buf[8] = 0x84;//Give error
			buf[9] = 2;//Error data out of range
			buf[5] = 4;
			hs->xmit_buf_size = 10;
		}else{
			memcpy(&buf[9],&modRegisters[offset],2);
			buf[5] = 5;
			hs->xmit_buf_size = 11;
		}
		break;
	case 6://Write single register UNTESTED Probably works
		printf("6\n");
		//memcpy(buf,request.bytes,8);
		if(offset > REG_COUNT){
			buf[8] = 0x86;
			buf[9] = 2; //Error Data out of range
			buf[5] = 4;
			hs->xmit_buf_size = 10;
		}else{
			memcpy(&modRegisters[offset],(const void *)&DATA_BUF[10],2);
			hs->xmit_buf_size = 12;
		}
		break;

	case 16://Write multiple registers TESTED WORKS
		printf("16\n");
		if(size + offset > REG_COUNT){
			buf[8] = 0x90;
			buf[9] = 2; //Error Data out of range.
			buf[5] = 4;
			hs->xmit_buf_size = 10;
		}else{
			memcpy(modRegisters+offset,&DATA_BUF[13], size);
			buf[5] = 6;
			hs->xmit_buf_size = 12;
		}
		break;
	case 23://Read and write UNTESTED
		printf("23\n");
		if(size + offset > REG_COUNT || (buf[13]*2 + buf[15]*2) > REG_COUNT){
			buf[8] = 0x97;
			buf[9] = 2;
			buf[5] = 4;
			hs->xmit_buf_size = 10;
		}else{
			memcpy(buf + 9,&modRegisters[offset],size);
			buf[8] = size;
			buf[5] = 3 + size;
			hs->xmit_buf_size = 10 + size;
			offset = buf[13]*2;
			size = buf[15]*2;
			memcpy(modRegisters+offset,&buf[17],size);
		}
		break;
	default:
		memcpy(buf,request.bytes,8);
		buf[8] = 0x00;
		buf[9] = 1;
		buf[5] = 4;
		hs->xmit_buf_size = 10;
	}//Switch

	hs->xmit_buf = buf;
	uip_send(hs->xmit_buf, hs->xmit_buf_size);
	hs->done = true;
	
  }else if( uip_acked()){//3c Got response acknowledge
    hs->data_count++;

  }else if( uip_poll()){//3d Poll to keep connection live
    printf("Poll\n");
    hs->idle_count++;
    if( hs->idle_count > 1000 ) {//Maybe not keep this.  Connection shouldn't close
      uip_close();
    }
  }//end of part 3

  //4 Send the response here resend if the last packet failed
  if( uip_rexmit()) {
    //printf("%p: Sending data (%d)\n", hs, hs->data_count);
	uip_send(buf, hs->xmit_buf_size);	
  }//if( uip_rexmit() || send_new_data )
}
