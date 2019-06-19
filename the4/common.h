#ifndef _COMMON_H
#define _COMMON_H

#include "device.h"

#define _XTAL_FREQ   40000000

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _10MHZ	63320
#define _16MHZ	61768
#define _20MHZ	60768
#define _32MHZ	57768
#define _40MHZ 	55768

/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define ALARM_EVENT       0x80
#define LCD_EVENT         0x01

/***********************************************************************
 * ----------------------------- Alarms --------------------------------
 **********************************************************************/
#define ALARM_TSK0          0       /* Alarm ID (index) in tascdesc.c */
#define LCD_ALARM_ID        1       /* Alarm ID (index) in tascdesc.c */
#define LCD_ALARM_MAIN      2
/***********************************************************************
 * ---------------------- Task ID & Priorities -------------------------
 **********************************************************************/
#define TASK0_ID            1
#define LCD_ID              5

#define TASK0_PRIO          10
#define LCD_PRIO            5


/* System States */
#define _WAITING	0		// waiting state
#define _OPERATING      3

#define SIZE_TM_BUF 100
#define SIZE_RC_BUF 100


/**********************************************************************
 * ----------------------- FUNCTION PROTOTYPES ------------------------
 **********************************************************************/
 /* transmits data using serial communication */
void transmitData();
/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived();

void transmitBuffer_push(char *str,unsigned char size);

unsigned char receiveBuffer_pop(char *str);


#endif /* _DEFINE_H */


/* End of File : define.h */