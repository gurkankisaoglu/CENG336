#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include "common.h"

/**********************************************************************
 * Definition dedicated to the local functions.
 **********************************************************************/
#define DEFAULT_MODE       0


/**********************************************************************
 * Function prototypes.
 **********************************************************************/
void main (void);
void Init(void);
void StartupHook(void);
void ShutdownHook(StatusType error);
void ErrorHook(StatusType error);
void PreTaskHook(void);
void PostTaskHook(void);

extern union Timers Tmr0;

AppModeType SelectedMode;

/**********************************************************************
 * -------------------------- main function ---------------------------
 *
 * 
 * ID: 2171726 Name: Gurkan KISAOGLU
 * ID: 2172211 Name: Mert Anil YILMAZ
 *
 * 
 * We have 3 alarms and 2 events and 2 tasks in this Operating System.
 * 
 * 
 * * THE TASKS ARE:
 * *** TASK0:
 * ***** Main robot task, it can move forward or turn around with 50ms intervals,
 * ***** in every 50ms the robot sleeps and give its CPU to other Task. It has 10 priority.
 * 
 * *** TASK1:
 * ***** Main LCD task, it can refresh LCD screen in every 350ms, and when an portB change
 * ***** interrupt occurs, low priority interrupt begins to execute to update LCD screen. It has
 * ***** 5 priority.
 * 
 * 
 * * THE ALARMS ARE:
 * *** ALARM_TASK0:
 * ***** The alarm used for synchronize the robot and give it commands
 * ***** in every 50ms and doing operations like move turn and getting responses like
 * ***** sensor and encoder responses.
 * 
 * *** LCD_ALARM_ID:
 * ***** The alarm used for LCD delays.
 * 
 * *** LCD_ALARM_MAIN:
 * ***** The alarm used for synchronizing LCD refreshes. LCD
 * ***** refreshes itself in every 350ms with help of this alarm.
 * 
 * 
 * * THE EVENTS ARE:
 * *** LCD_EVENT:
 * ***** LCD_ALARM_MAIN fires this event, when this fires, LCD operations can be done.
 * 
 * *** ALARM_EVENT:
 * ***** ALARM_TASK0 fires this event to notify waiting robot and encourage it to do
 * ***** operations.
 * 
 **********************************************************************/







void main(void)
{
  STKPTR = 0;
  SelectedMode = DEFAULT_MODE;
  Init();
  
  while(1) {
    StartOS(SelectedMode);
  }
}

/**********************************************************************
 * Clear all RAM memory and set PORTB to output mode.
 *
 * @return void
 **********************************************************************/
void Init(void)
{
	FSR0H = 0;
	FSR0L = 0;
	
	/* User setting : actual PIC frequency */
	Tmr0.lt = _40MHZ;
	
	/* Timer OFF - Enabled by Kernel */
	T0CON = 0x08;
	TMR0H = Tmr0.bt[1];
	TMR0L = Tmr0.bt[0];
	
	/* configure I/O ports */
    TRISCbits.RC7 = 1; // TX1 and RX1 pin configuration
    TRISCbits.RC6 = 0;

    
	
	/* configure USART transmitter/receiver */
	SPBRG1 = 21;		// for 40 MHz, to have 115200 baud rate, it should be 21
	TXSTA1 = 0x04;      // (= 00000100) 8-bit transmit, transmitter NOT enabled,
                        // asynchronous, high speed mode
	RCSTA1 = 0x90;      // (= 10010000) 8-bit receiver, receiver enabled,
                        // continuous receive, serial port enabled
	
	/* configure the interrupts */
	INTCON = 0;			// clear interrupt register completely
	PIE1bits.TX1IE = 1;	// enable USART transmit interrupt
	PIE1bits.RC1IE = 1;	// enable USART receive interrupt
	PIR1 = 0;			// clear all peripheral flags
	
	INTCONbits.PEIE = 1;// enable peripheral interrupts
	INTCONbits.GIE = 1;	// globally enable interrupts
}

/**********************************************************************
 * Hook routine called just before entering in kernel.
 *
 * @param error      IN The new error stored in buffer
 * @return error     Error level
 **********************************************************************/
void StartupHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving the kernel.
 *
 * @param error      IN The last error detected by OS
 * @return void
 **********************************************************************/
void ShutdownHook(StatusType error)
{
}

/**********************************************************************
 * Store a new error in a global buffer keeping a track of the 
 * application history.
 *
 * @param error      IN The new error stored in buffer
 * @return void
 **********************************************************************/
void ErrorHook(StatusType error)
{
}

/**********************************************************************
 * Hook routine called just before entering in a task.
 *
 * @return void
 **********************************************************************/
void PreTaskHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving a task.
 *
 * @return void
 **********************************************************************/
void PostTaskHook(void)
{
}

/* End of File : main.c */
