#include "common.h"

/**********************************************************************
 * Function you want to call when an IT occurs.
 **********************************************************************/
  extern void AddOneTick(void);
/*extern void MyOwnISR(void); */
  void InterruptVectorL(void);
  void InterruptVectorH(void);

  extern unsigned int state;
  extern unsigned int pressed;
/**********************************************************************
 * General interrupt vector. Do not modify.
 **********************************************************************/
#pragma code IT_vector_low=0x18
void Interrupt_low_vec(void)
{
   _asm goto InterruptVectorL  _endasm
}
#pragma code

#pragma code IT_vector_high=0x08
void Interrupt_high_vec(void)
{
   _asm goto InterruptVectorH  _endasm
}
#pragma code


#pragma	code _INTERRUPT_VECTORL = 0x003000
#pragma interruptlow InterruptVectorL 
void InterruptVectorL(void)
{
	EnterISR();
	
	if (INTCONbits.TMR0IF == 1)
		AddOneTick();
	/* Here are the other interrupts you would desire to manage */
	if (PIR1bits.TX1IF == 1) {
		transmitData();
	}
	if (PIR1bits.RC1IF == 1) {
		dataReceived();
		PIR1bits.RC1IF = 0;	// clear RC1IF flag
	}
    if (RCSTA1bits.OERR)
    {
      RCSTA1bits.CREN = 0;
      RCSTA1bits.CREN = 1;
    }

    
    /* Interrupt on change routine
     * when RB4 button pressed and released, this routine executes, and switches 
     * LCD states
    */
    if (INTCONbits.RBIF) { 
        PORTBbits.RB4;            
        if (PORTBbits.RB4 && pressed==0) {
            pressed = 1;
        }
        else if(pressed==1 && PORTBbits.RB4==0){
            state = (state+1)%2;
            pressed = 0;
        }
        INTCONbits.RBIF = 0; // clear the interrupt flag
    }

	
	LeaveISR();
}
#pragma	code


#pragma	code _INTERRUPT_VECTORH = 0x003300
#pragma interrupt InterruptVectorH nosave=FSR0, TBLPTRL, TBLPTRH, TBLPTRU, TABLAT, PCLATH, PCLATU, PROD, section(".tmpdata"), section("MATH_DATA")
void InterruptVectorH(void)
{
  if (INTCONbits.INT0IF == 1)
    INTCONbits.INT0IF = 0;
}
#pragma	code



extern void _startup (void);
#pragma code _RESET_INTERRUPT_VECTOR = 0x003400
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code


/* End of file : int.c */
