// 2171726 GURKAN KISAOGLU 
// 2172211 MERT ANIL YILMAZ 

#include <p18cxxx.h>
#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#define _XTAL_FREQ   40000000

#include "Includes.h"
#include "LCD.h"

/*
In this code;
	- we used timer0 interrupt in order to set ADIE and GODONE bits so that we obtain ADC values in each 100 ms
	and to display blink in LCD screen.
	- we used timer1 interrupt to display the remaining time in LCD screen with updating in every 1 second
	- we used adc interrupt in order to start adc conversion, and
	- we used rb port change interrupt (rb6 and rb7)
		-rb6 is to arrange the number of voters
		-rb7 is to vote a candidate and also has a role in arranging the number of voters with rb6.
*/


int adcVal; // is for current adc value

// They are for initial screen of LCD
const char* initialScreen1stRow = " #Electro Vote# ";
const char* initialScreen2ndRow = " ############## ";

const char* voteInputScreen = " #Enter Voters# "; //After the initial screen, it will disappear
char voterCount[16] = " 00             "; // We manipulated chars in order to store and display the number of voters
int totalVoteCount = 0;
int currentVoteCount = 0;
int finished = 0;
char timeRemains[16] = "  Time left :90 "; // We manipulated chars in order to store and display time left


// These are interrupt flags which are set in interrupts. Outside of interrupts, by using them, different functions are called.
int tmr1Interrupt = 0;
int blinkOn = 0;
int blinkOff = 0;
int clearLCDFlag = 0;
int incrementVoterFlag = 0;
int voteFlag = 0;
int adcReadFlag = 0;

int currentLeaderIndex = -1; //This is hold current leader index in order to display in 7 segment display

// These are used in order to obtain accurate times for different functionalities.
int rb7Counter = 0;
int timer0count = 0;
int timer1count = 0;
int timer0ADCCount = 0;
int timer0BlinkCount = 0;

int displayNo = 0;
int stateNumber = 0;

// These are for 7 segment display. We calculated the binary values of
// each decimal digit for 7 segment display plus the binary values of dash figure
int digit1 = 0b00000110;
int digit2 = 0b01011011;
int digit3 = 0b01001111;
int digit4 = 0b01100110;
int digit5 = 0b01101101;
int digit6 = 0b01111101;
int digit7 = 0b00000111;
int digit8 = 0b01111111;
int digit9 = 0b01101111;
int digit0 = 0b00111111;
int dash = 0b01000000;

int index;

// These are for initial screen for 7 segment display
char leaderID[2] = "--";
char leaderVote[2] = "--";

const char *candidates[8][2] =
{
	{"18 ", "Poe    "},
	{"64 ", "Galib  "},
	{"34 ", "Selimi "},
	{"23 ", "Nesimi "},
	{"33 ", "Hatayi "},
	{"67 ", "Zweig  "},
	{"63 ", "Nabi   "},
	{"99 ", "Austen "}
};

// These are hold the votes for each candidate. We used char manipulation
char voteCounts_0[4] = ":00 ";
char voteCounts_1[4] = ":00 ";
char voteCounts_2[4] = ":00 ";
char voteCounts_3[4] = ":00 ";
char voteCounts_4[4] = ":00 ";
char voteCounts_5[4] = ":00 ";
char voteCounts_6[4] = ":00 ";
char voteCounts_7[4] = ":00 ";

// This is the fuction which is written the current values to 7 segment display
// in order to display this, we assign values to PORTJ.
void displayPort(char charToWrite) {
	switch (charToWrite) {
	case '-':
		PORTJ = dash;
		break;
	case '9':
		PORTJ = digit9;
		break;
	case '8':
		PORTJ = digit8;
		break;
	case '7':
		PORTJ = digit7;
		break;
	case '6':
		PORTJ = digit6;
		break;
	case '5':
		PORTJ = digit5;
		break;
	case '4':
		PORTJ = digit4;
		break;
	case '3':
		PORTJ = digit3;
		break;
	case '2':
		PORTJ = digit2;
		break;
	case '1':
		PORTJ = digit1;
		break;
	case '0':
		PORTJ = digit0;
		break;
	}
}

// This is the function which are determined the boundary values of each candidate in adc value
// It set 'index' which is essential in order to display the right candidate in LCD screen
void writeLCDToAdc(int adcVal) {
	if (adcVal <= 127) {
		index = 0;
	}
	else if (adcVal >= 128 && adcVal <= 255) {
		index = 1;
	}
	else if (adcVal >= 256 && adcVal <= 383) {
		index = 2;
	}
	else if (adcVal >= 384 && adcVal <= 511) {
		index = 3;
	}
	else if (adcVal >= 512 && adcVal <= 639) {
		index = 4;
	}
	else if (adcVal >= 640 && adcVal <= 767) {
		index = 5;
	}
	else if (adcVal >= 768 && adcVal <= 895) {
		index = 6;
	}
	else if (adcVal >= 896) {
		index = 7;
	}
}


// We have one interrupt service routine. We controlled all four types of interrupts in interrupt service routine
// We didn't use any "else if" not to miss any interrupt because all interrupts are mutually exclusive
void __interrupt() isr(void) {

	/*
	(1/40 MHz)*4 = 10^-7 second
	in order to obtain 1 second --> we must increment 10^7 times
	precaler:8 --> (10^7)/8 = 1250000
	since we used 16-bit read/write mode, 2^16 = 65536
	65536*19 + (65536-60720) = 1245184+4816 = 1250000
	Therefore we use timer1count==20 check whether we obtained 1 second, or not

	We used char manipulation in this part, too.
	*/
	if (TMR1IF) {
		timer1count++;
		if (timer1count == 20) { // 1 second
			timer1count = 0;
			TMR1 = 60720;
			if (timeRemains[14] == '0') {
				timeRemains[13] = (char)(((int)timeRemains[13]) - 1);
				timeRemains[14] = '9';
			}
			else {
				timeRemains[14] = (char)(((int)timeRemains[14]) - 1);
			}
			tmr1Interrupt = 1;
		}
		TMR1IF = 0; // We cleared interrupt flag
	}


	/*
	prescaler:32 and 8-bit timer0
	5*10^5 --> for 50 ms
	500000 / 32 = 15625
	(61*256)+(256-247) = 15625

	*/

	if (TMR0IF) { //enter every 1ms, every 250th, change blink state, every 100 start ADC conversion (ADIE=0,ADIF=1,GO=1)) and update seven segment display

		// This if condition is for flicker
		if (displayNo % 4 == 0) {
			PORTH = 0b00000001;
			displayPort(leaderID[0]);
		}
		else if (displayNo % 4 == 1) {
			PORTH = 0b00000010;
			displayPort(leaderID[1]);
		}
		else if (displayNo % 4 == 2) {
			PORTH = 0b00000100;
			displayPort(leaderVote[0]);
		}
		else {
			PORTH = 0b00001000;
			displayPort(leaderVote[1]);
		}

		// This part is for setting TMROL. (50 ms)
		displayNo++;
		if (displayNo == 4) displayNo = 0;
		timer0count++;
		if (timer0count == 62) {
			timer0count = 0;
			TMR0L = 247;
		}
		// This part is to set adc interrupt enable bir and start conversion (100 ms)
		timer0ADCCount++;
		if (timer0ADCCount == 124) {
			timer0ADCCount = 0;
			ADIF = 0;
			ADIE = 1;
			ADCON0bits.GODONE = 1;
		}

		// These parts are for blinking (250 and 500 ms)
		timer0BlinkCount++;
		if (timer0BlinkCount == 310) {
			blinkOff = 1;
		}
		if (timer0BlinkCount == 620) {
			blinkOn = 1;
			timer0BlinkCount = 0;
		}

		TMR0IF = 0; // clear the interrupt flag
	}
	if (RBIF) { // on port change interrupt, update LCD if voted, and update 7 segment display
		PORTBbits.RB7;
		PORTBbits.RB6;
		if (stateNumber == 1) { // to display and set the number of voter
			if (PORTBbits.RB7) {
				if (rb7Counter == 0) { //rb7Counter is for deciding when program will goes to state 2.
					rb7Counter++;
				}
				else if (rb7Counter == 1) {
					TRISBbits.TRISB6 = 0;
					stateNumber = 2;
					clearLCDFlag = 1; //vote will begin, clearLCD
				}
			}
			else if (PORTBbits.RB6) {
				incrementVoterFlag = 1; //it will increment the units/tens digit of voterCount after return if isr.
			}
		}
		else if (stateNumber == 2) { // to vote
			if (PORTBbits.RB7)
				voteFlag = 1;
		}

		RBIF = 0; // clear the interrupt flag
		INTCONbits.INT0IF = 0;

	}
	if (ADIF) { // update LCD

		adcVal = (ADRESH & 2) * 256 + (ADRESH & 1) * 256 + ADRESL;
		// ADRESH&2 = 0 OR ADRESH&2 = 2
		// ADRESH&1 = 0 OR ADRESH&1 = 1
		// adcVal cannot exceed 1023 and the min value of it is 0.
		ADIF = 0; // clear the interrupt flag
		adcReadFlag = 1;
	}
}
void init(){
    TRISJ = 0;  // PORTJ all output for 7segment display
    TRISH = 0xF0; // rightmost 4 bits are output for 7segment usage
    TRISE = 0; //all output
    TRISB = 0; //all output
    TRISB6 = 1; // rb6 input
    TRISB7 = 1; // rb7 input
    
}

void initRB(){
    INTCONbits.RBIE = 1; // On change port interrupt enabled
    INTCONbits.RBIF = 0; // on change port interrupt flag cleared
    INTCON2bits.RBPU = 0; // pull up disabled
    INTCONbits.INT0IE = 1; // interrupt enabled
    INTCONbits.INT0IF = 0; // interrupt flag cleared
}

void initTimer0(){
	T0CON = 0x44;
	//0100 0100
	//TMR0ON, T08BIT(8-bit), T0CS(internal cycle), T0SE
	//PSA(prescaler) T0PS2 T0PS1 T0PS0 -- prescaler:32.
	TMR0L = 247; //hesabi buraya ekle
	TMR0IF = 0; //clear flag
	INTCONbits.TMR0IE = 1;
}

void initTimer1(){
	T1CON = 0xF0;
	//1111 0000
	//RD16(16-bit), T1RUN(oscillator), T1CKPS1, T1CKPS0 -- precaler:8.
	//T1OSCEN(oscillator not enable), T1SYNC(ignored), TMR1CS(internal clock), TMR1ON
	TMR1 = 60720;
	PIE1bits.TMR1IE = 1;
}

void initADC(){ 
    ADCON1 = 0x00; //AVdd, AVss and all Analog inputs
    ADCON0 = 0b00110000; //just channel 12 selected, GODONE =0 , ADON = 0 for now
    ADCON2 = 0b10101110; //ACQT=12Tad Fosc/64=ADCS
    ADON = 1; // AD converter module enable
}

int findLeader(){
    int index0Votes = (voteCounts_0[1]-'0')*10 + (voteCounts_0[2]-'0'); // getting integer vote counts for every candidate
    int index1Votes = (voteCounts_1[1]-'0')*10 + (voteCounts_1[2]-'0');
    int index2Votes = (voteCounts_2[1]-'0')*10 + (voteCounts_2[2]-'0');
    int index3Votes = (voteCounts_3[1]-'0')*10 + (voteCounts_3[2]-'0');
    int index4Votes = (voteCounts_4[1]-'0')*10 + (voteCounts_4[2]-'0');
    int index5Votes = (voteCounts_5[1]-'0')*10 + (voteCounts_5[2]-'0');
    int index6Votes = (voteCounts_6[1]-'0')*10 + (voteCounts_6[2]-'0');
    int index7Votes = (voteCounts_7[1]-'0')*10 + (voteCounts_7[2]-'0');
    int arr[8]={index0Votes, index1Votes, index2Votes, index3Votes, index4Votes, index5Votes, index6Votes, index7Votes};
    int max = 0;
    int maxIndex = -1;
    for(int i=0 ; i<8 ; i++){ //find max vote count
        if(max<arr[i]){
            max = arr[i];
            maxIndex = i;
        }    
    }
	//the below lines ensures that current leader only changes when someone's votecount is bigger than current leader
    if(currentLeaderIndex!=-1 && arr[maxIndex]!=arr[currentLeaderIndex]){
        currentLeaderIndex = maxIndex;
    }
    else if(currentLeaderIndex==-1){
        currentLeaderIndex = maxIndex;
    }
    return currentLeaderIndex;
}

void updateLeader(){
    int leaderIndex = findLeader(); // finds leader of the voting period
    leaderID[0] = candidates[leaderIndex][0][0]; //updates leaderID with use of above variable
    leaderID[1] = candidates[leaderIndex][0][1];
    switch(leaderIndex){ // finds vote count of leader
        case 0:
            leaderVote[0]=voteCounts_0[1]; //updates leaderVote and leaderID and leaderVote will be used in seven segment display to show leader info
            leaderVote[1]=voteCounts_0[2];
            break;
        case 1:
            leaderVote[0]=voteCounts_1[1];
            leaderVote[1]=voteCounts_1[2];
            break;
        case 2:
            leaderVote[0]=voteCounts_2[1];
            leaderVote[1]=voteCounts_2[2];
            break;
        case 3:
            leaderVote[0]=voteCounts_3[1];
            leaderVote[1]=voteCounts_3[2];
            break;
        case 4:
            leaderVote[0]=voteCounts_4[1];
            leaderVote[1]=voteCounts_4[2];
            break;
        case 5:
            leaderVote[0]=voteCounts_5[1];
            leaderVote[1]=voteCounts_5[2];
            break;
        case 6:
            leaderVote[0]=voteCounts_6[1];
            leaderVote[1]=voteCounts_6[2];
            break;
        case 7:
            leaderVote[0]=voteCounts_7[1];
            leaderVote[1]=voteCounts_7[2];
            break;
        default:
            break;
    }
}

void delayLoop(int milisecs){ // three second loop
    for(int i=0 ; i<milisecs ; i++){
        for(int j=0 ; j<910 ; j++){   
        }
    }
}


void votingState(){
    int tens = voterCount[1]-'0';
    int ones = voterCount[2]-'0';
    totalVoteCount = tens*10+ones;
    TMR1ON = 1;//when voting state begins, timer will be on
    TMR0ON = 1;
    ClearLCDScreen();
    WriteCommandToLCD(0x80);
    WriteDataToLCD(' ');
    WriteDataToLCD(' ');
    WriteDataToLCD('T');
    WriteDataToLCD('i');
    WriteDataToLCD('m');
    WriteDataToLCD('e');
    WriteDataToLCD(' ');
    WriteDataToLCD('l');
    WriteDataToLCD('e');
    WriteDataToLCD('f');
    WriteDataToLCD('t');
    WriteDataToLCD(' ');
    WriteDataToLCD(':');
    WriteDataToLCD('9');
    WriteDataToLCD('0');
    WriteDataToLCD(' ');
    while(1){
		// This loop is for deciding which operation will program do based on the interrupt enter flags we defined until time remaining set to zero or all voters has vote.
        if(currentVoteCount == totalVoteCount){ // all voters voted.
            TMR1ON = 0; //stop timer so time remains wont be decreased
            finished = 1;
            
        }
        if(tmr1Interrupt == 1){ // 1second flag, must update LCD with time remaining.
            WriteCommandToLCD(0x8D);
            WriteDataToLCD(timeRemains[13]);
            WriteDataToLCD(timeRemains[14]);
            WriteCommandToLCD(0xC0);
            WriteStringToLCD("  ");
            WriteStringToLCD(candidates[index][0]);
            WriteStringToLCD(candidates[index][1]);
            tmr1Interrupt = 0;
        }

        if(blinkOn == 1){ // 250ms blink
            WriteCommandToLCD(0xC1);
            WriteDataToLCD('>');
            blinkOn = 0;
        }
        if(blinkOff == 1 && !finished){ // 250ms blink
            WriteCommandToLCD(0xC1);
            WriteDataToLCD(' ');
            blinkOff = 0;
        }
        
        if(blinkOff == 1 && finished){ // when program finishes, the character '>' must be shown.
            WriteCommandToLCD(0xC1);
            WriteDataToLCD('>');
            blinkOff = 0;
        }
        
        if(voteFlag == 1 && currentVoteCount!=totalVoteCount){ //vote given
            currentVoteCount++;
			// variable index comes from adc calculations, this is candidate index
			// below operations are for update candidate vote counts.
			if (index == 0) { 
                if(voteCounts_0[2]=='9'){
                    voteCounts_0[2]='0';
                    voteCounts_0[1]=(char)(((int)voteCounts_0[1]) + 1);
                }else{
                    voteCounts_0[2]=(char)(((int)voteCounts_0[2]) + 1);
                }
                WriteCommandToLCD(0xCC);
                WriteStringToLCD(voteCounts_0);       // must update voteCount on LCD when vote given          
            }else if(index == 1){
                if(voteCounts_1[2]=='9'){
                    voteCounts_1[2]='0';
                    voteCounts_1[1]=(char)(((int)voteCounts_1[1]) + 1);
                }else{
                    voteCounts_1[2]=(char)(((int)voteCounts_1[2]) + 1);
                }
                WriteCommandToLCD(0xCC);
                WriteStringToLCD(voteCounts_1);
            }else if(index == 2){
                if(voteCounts_2[2]=='9'){
                    voteCounts_2[2]='0';
                    voteCounts_2[1]=(char)(((int)voteCounts_2[1]) + 1);
                }else{
                    voteCounts_2[2]=(char)(((int)voteCounts_2[2]) + 1);
                }
                WriteCommandToLCD(0xCC);
                WriteStringToLCD(voteCounts_2);
            }else if(index == 3){
                if(voteCounts_3[2]=='9'){
                    voteCounts_3[2]='0';
                    voteCounts_3[1]=(char)(((int)voteCounts_3[1]) + 1);
                }else{
                    voteCounts_3[2]=(char)(((int)voteCounts_3[2]) + 1);
                }
                WriteCommandToLCD(0xCC);
                WriteStringToLCD(voteCounts_3);
            }else if(index == 4){
                if(voteCounts_4[2]=='9'){
                    voteCounts_4[2]='0';
                    voteCounts_4[1]=(char)(((int)voteCounts_4[1]) + 1);
                }else{
                    voteCounts_4[2]=(char)(((int)voteCounts_4[2]) + 1);
                }
                WriteCommandToLCD(0xCC);
                WriteStringToLCD(voteCounts_4);
            }else if(index == 5){
                if(voteCounts_5[2]=='9'){
                    voteCounts_5[2]='0';
                    voteCounts_5[1]=(char)(((int)voteCounts_5[1]) + 1);
                }else{
                    voteCounts_5[2]=(char)(((int)voteCounts_5[2]) + 1);
                }
                WriteCommandToLCD(0xCC);
                WriteStringToLCD(voteCounts_5);
            }else if(index == 6){
                if(voteCounts_6[2]=='9'){
                    voteCounts_6[2]='0';
                    voteCounts_6[1]=(char)(((int)voteCounts_6[1]) + 1);
                }else{
                    voteCounts_6[2]=(char)(((int)voteCounts_6[2]) + 1);
                }
                WriteCommandToLCD(0xCC);
                WriteStringToLCD(voteCounts_6);
            }else if(index == 7){
                if(voteCounts_7[2]=='9'){
                    voteCounts_7[2]='0';
                    voteCounts_7[1]=(char)(((int)voteCounts_7[1]) + 1);
                }else{
                    voteCounts_7[2]=(char)(((int)voteCounts_7[2]) + 1);
                }
                WriteCommandToLCD(0xCC);
                WriteStringToLCD(voteCounts_7);
            }
            updateLeader(); // update leader if changed, so that seven segment display will shows winner of the voting period.
            voteFlag = 0;
        }
        
        if(adcReadFlag == 1){ // every 100ms, adc interrupt occurs.
            writeLCDToAdc(adcVal); // find candidate index of given adcValue
            WriteCommandToLCD(0xC2);
            WriteStringToLCD(candidates[index][0]); //write candidate info to LCD
            WriteStringToLCD(candidates[index][1]);
            if(index == 0){
                WriteStringToLCD(voteCounts_0);                
            }else if(index == 1){
                WriteStringToLCD(voteCounts_1);
            }else if(index == 2){
                WriteStringToLCD(voteCounts_2);
            }else if(index == 3){
                WriteStringToLCD(voteCounts_3);
            }else if(index == 4){
                WriteStringToLCD(voteCounts_4);
            }else if(index == 5){
                WriteStringToLCD(voteCounts_5);
            }else if(index == 6){
                WriteStringToLCD(voteCounts_6);
            }else if(index == 7){
                WriteStringToLCD(voteCounts_7);
            }
            adcReadFlag = 0;
        }
        if(timeRemains[13]=='0' && timeRemains[14]=='0')
            break;
    }
    WriteCommandToLCD(0x8D); //timer finishes
    WriteDataToLCD('0');
    WriteDataToLCD('0');
    finished = 1;
    TMR1ON=0; // close timer
    WriteCommandToLCD(0xC1); // char '>' always shown
    WriteDataToLCD('>');
    while(1){
        
    }
}


void main(void) // main routine, program starts from here
{
	//initializations
    TRISA = 0;
    InitLCD();			
    initRB(); 
    initTimer0();
    initTimer1();

    initADC();
    init();

    GIE = 1;



    ClearLCDScreen();           // Clear LCD screen
    WriteCommandToLCD(0x80);   // Goto to the beginning of the first line
    WriteStringToLCD(initialScreen1stRow);	// Write LCD
    WriteCommandToLCD(0xC0);
    WriteStringToLCD(initialScreen2ndRow);	// Write LCD
    TRISEbits.RE1 = 1;
    while(!PORTEbits.RE1){
    }
    while(PORTEbits.RE1){
    }
    
    delayLoop(3000);
    
    ClearLCDScreen();
    WriteCommandToLCD(0x80);   // Goto to the beginning of the first line
	//Write Data To First Line
    WriteDataToLCD(' ');
    WriteDataToLCD('#');
    WriteDataToLCD('E');
    WriteDataToLCD('n');
    WriteDataToLCD('t');
    WriteDataToLCD('e');
    WriteDataToLCD('r');
    WriteDataToLCD(' ');
    WriteDataToLCD('V');
    WriteDataToLCD('o');
    WriteDataToLCD('t');
    WriteDataToLCD('e');
    WriteDataToLCD('r');
    WriteDataToLCD('s');
    WriteDataToLCD('#');
    WriteDataToLCD(' ');
    WriteCommandToLCD(0xC1);
    WriteDataToLCD('0');
    WriteDataToLCD('0');
    PORTBbits.RB7=0;
    PORTBbits.RB6=0;
    stateNumber = 1; //state number == 1 indicates program is on stage of deciding voter count.
    while(stateNumber == 1){
		// This loop is for deciding which operation will program do based on the interrupt enter flags we defined until state number set as 2.
        if(clearLCDFlag == 1){ // when clearLCDFlag == 1, the state will actually be 2 in next loop so LCD must be cleaned.
            ClearLCDScreen();
            clearLCDFlag = 0;
        }
        if(incrementVoterFlag == 1){ //voter count increment
            if(rb7Counter==0){ //if rb7Counter==0 this means we will increment tens digit.
                if(voterCount[1]=='9'){
                    voterCount[1]='0';
                }else{
                    voterCount[1]=(char)(((int)voterCount[1]) + 1);
                }
                WriteCommandToLCD(0xC1); // Write new voter count/digit value to screen
                WriteDataToLCD(voterCount[1]);
                    
            }else if(rb7Counter==1){ //if rb7Counter==1 this means we will increment units digit.
                if(voterCount[2]=='9'){
                    voterCount[2]='0';
                }else{
                    voterCount[2]=(char)(((int)voterCount[2]) + 1);
                }
                WriteCommandToLCD(0xC2); // Write new voter count/digit value to screen
                WriteDataToLCD(voterCount[2]);
            }
            incrementVoterFlag = 0;
        }
        
    }
    votingState(); //when state number == 2, voting state begins
}