#include "modbusd.h"
#include "uip.h"
#include "common.h"

#include <stdbool.h>
#include <string.h>

//#define REG_COUNT 128
unsigned char modRegisters[REG_COUNT];//These are the modbus registers where control data is stored

struct MODBUS_HEADER_MSG{
	unsigned short transID;
	unsigned short protoID;
	unsigned short length;
	unsigned char unitID;
	unsigned char fcnID;
	unsigned short offset;
	unsigned short size;
	unsigned char data[128];
}*request,*wResponse;

struct MODBUS_HEADER_RD{
	unsigned short transID;
	unsigned short protoID;
	unsigned short length;
	unsigned char unitID;
	unsigned char fcnID;
	unsigned char size;
	unsigned char data[128];
}*rResponse;

struct MODBUS_HEADER_ERR{
	unsigned short transID;
	unsigned short protoID;
	unsigned short length;
	unsigned char unitID;
	unsigned char fcnID;
	unsigned char error;
	unsigned char errorID;
}*error;

struct MODBUS_HEADER_MSG23{
	unsigned short transID;
	unsigned short protoID;
	unsigned short length;
	unsigned char unitID;
	unsigned char fcnID;
	unsigned short readOffset;
	unsigned short readSize;
	unsigned short writeOffset;
	unsigned short writeSize;
	unsigned char byteCount;
	unsigned char data[121];
}*request23;

#define DATA_BUF ((uint8_t*)(uip_appdata))//Stores the request

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
  uint8_t buf[150];//Stores the outgoing response

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
    request = (struct MODBUS_HEADER_MSG*)DATA_BUF;
    wResponse = (struct MODBUS_HEADER_MSG*)buf;
    rResponse = (struct MODBUS_HEADER_RD*)buf;
    error = (struct MODBUS_HEADER_ERR*)buf;
	//memcpy(buf,DATA_BUF,12);//Get all data just in case
    printf("Got new request\n");

	//Parse modbus header here
	unsigned short offset,size,wOffset,wSize;
	memcpy(buf,DATA_BUF,12);//Copy the header to the out buffer.
	offset = HTONS(request->offset)*2;//NTOHS(request.fields.offset)*2;
	size = HTONS(request->size)*2;//NTOHS(request.fields.size)*2;

  switch(request->fcnID){
	case 3://Read multiple registers TESTED works
		printf("3\n");
		if(size + offset > REG_COUNT){//Send error here
			error->error = 0x83;
			error->errorID = 2;
			error->length = HTONS(4);
			hs->xmit_buf_size = 10;
		}else{//send data
			memcpy(rResponse->data,&modRegisters[offset],size);
			rResponse->length = HTONS(3 + size);
			rResponse->size = size;
			hs->xmit_buf_size = 9 + size;
		}
		break;
	case 6://Write single register UNTESTED Probably works
		printf("6\n");
		if(offset > REG_COUNT){
			error->error = 0x86;
			error->errorID = 2;
			error->length = HTONS(4);
			hs->xmit_buf_size = 10;
		}else{
			memcpy(&modRegisters[offset],&request->size,2);//data is stored in size heres
			hs->xmit_buf_size = 12;
		}
		break;
	case 16://Write multiple registers TESTED WORKS
		printf("16\n");
		if(size + offset > REG_COUNT){
			error->error = 0x90;
			error->errorID = 2;
			error->length = HTONS(4);
			hs->xmit_buf_size = 10;
		}else{
			memcpy(modRegisters+offset,request->data+1, size);//Byte count is data[0]
			wResponse->length = HTONS(6);
			hs->xmit_buf_size = 12;
		}
		break;
	case 23://Read and write UNTESTED
		printf("23\n");
		request23 = (struct MODBUS_HEADER_MSG23*)DATA_BUF;
		wSize = HTONS(request23->writeSize)*2;
		wOffset = HTONS(request23->writeOffset)*2;
		if(size + offset > REG_COUNT ||	(wOffset + wSize) > REG_COUNT){
			error->error = 0x97;
			error->errorID = 2;
			error->length = HTONS(4);
			hs->xmit_buf_size = 10;
		}else{
			memcpy(rResponse->data,&modRegisters[offset],size);//Copy out bound data
			rResponse->size = size;
			rResponse->length = HTONS(3 + size);
			hs->xmit_buf_size = 9 + size;
			memcpy(&modRegisters[wOffset],request23->data,wSize);//Write local data
		}
		break;
	default:
		//This is for all unsupported codes
		error->error = 0x00;
		error->errorID = 1;
		error->length = HTONS(4);
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
	uip_send(buf, hs->xmit_buf_size);	
  }//if( uip_rexmit())
}
