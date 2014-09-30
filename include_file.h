//This file just gets the includes out of main for cleanliness.

#include "inc/lm4f120h5qr.h"
#include <inc/hw_ints.h>
#include <stdint.h>
#include "common.h"
#include "enc28j60.h"
#include "spi.h"
#include <driverlib/systick.h>
#include <driverlib/interrupt.h>
#include <uip/uip.h>
#include <uip/uip_arp.h>
#include "modbusd.h"
