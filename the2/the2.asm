; MERT ANIL YILMAZ 2172211
; GURKAN KISAOGLU 2171726    
    
LIST    P=18F8722

#INCLUDE <p18f8722.inc> 
    
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

v1 udata 0x20
valh_timer1
v2 udata 0x21
vall_timer1
v3 udata 0x22
num_asteroids 
v4 udata 0x23
btemp
v5 udata 0x24
bline1
v6 udata 0x25
bline2
v7 udata 0x26
bline3
v8 udata 0x27
bline4
v9 udata 0x28
bline5
v10 udata 0x29
bline6
v11 udata 0x2a
aline1
v12 udata 0x2b
aline2
v13 udata 0x2c
aline3
v14 udata 0x2d
aline4
v15 udata 0x2e
aline5
v16 udata 0x2f
aline6
v17 udata 0x30
counter
v18 udata 0x31
point
v19 udata 0x32
for5sn
v20 udata 0x33
shiftCount 
v21 udata 0x34
lineCount 
v22 udata 0x35
temp2
v23 udata 0x36
gameOver
v24 udata 0x37
shipLine 
v25 udata 0x38
atemp
v26 udata 0x39
temp3
v27 udata 0x3a
firstDigitCount
v28 udata 0x3b
secondDigitCount
v29 udata 0x3c
thirdDigitCount
v30 udata 0x3d
fourthDigitCount
v31 udata 0x40
digit9
v32 udata 0x41
digit8
v33 udata 0x42
digit7
v34 udata 0x43
digit6
v35 udata 0x44
digit5
v36 udata 0x45
digit4
v37 udata 0x46
digit3
v38 udata 0x47
digit2
v39 udata 0x48
digit1
v40 udata 0x49
digit0
v41 udata 0x4a
pointTemp
v42 udata 0x4b
stat
v43 udata 0x4c
segmentDigit 
 
 
org 0x00
goto main
 
org 0x08
goto isr ;go to int. serv. rout.
 
org 0x18
retfie

init
    movlw b'00000110'
    movff WREG, digit1
    movlw b'01011011'
    movff WREG, digit2
    movlw b'01001111'
    movff WREG, digit3
    movlw b'01100110'
    movff WREG, digit4
    movlw b'01101101'
    movff WREG, digit5
    movlw b'01111101'
    movff WREG, digit6
    movlw b'00000111'
    movff WREG, digit7
    movlw b'01111111'
    movff WREG, digit8
    movlw b'01101111'
    movff WREG, digit9
    movlw b'00111111'
    movff WREG, digit0
    ;Disable interrupts
    clrf INTCON
    clrf INTCON2
    
    movlw d'0'
    movwf gameOver
    movlw d'4'
    movwf shipLine
    
    ;Configure Output Ports
    clrf  LATA
    movlw 0Fh
    movwf ADCON1
    movlw b'11000000'
    movwf TRISA
    clrf  LATB
    movlw b'11000000'
    movwf TRISB
    clrf  LATC
    movlw b'11000000'
    movwf TRISC
    clrf  LATD
    movlw b'11000000'
    movwf TRISD
    clrf  LATE
    movlw b'11000000'
    movwf TRISE
    clrf  LATF
    movlw b'11000000'
    movwf TRISF
    ;for display
    clrf  LATH
    movlw b'11110000'
    movwf TRISH
    clrf  LATJ
    movlw b'00000000'
    movwf TRISJ
    ;for input
    clrf  LATG
    movlw b'00001111'
    movwf TRISG
    movlw b'00001000'
    movwf LATA
    movlw b'00000000'
    movwf LATB
    movwf LATC
    movwf LATD
    movwf LATE
    movwf LATF
    ;Initialize Timer0
    movlw b'01001000'
    
    movwf T0CON
    ;Enable Interrupts
    movlw b'11100000'
    ;Enable global, Peripheral, Timer0
    movwf INTCON
    movlw b'00000000'
    movwf segmentDigit
    movwf num_asteroids
    movwf for5sn
    movwf point
    movwf counter
    movwf shiftCount
    movwf bline1
    movwf bline2
    movwf bline3
    movwf bline4
    movwf bline5
    movwf bline6
    movwf aline1
    movwf aline2
    movwf aline3
    movwf aline4
    movwf aline5
    movwf aline6
    movlw d'5'
    movwf lineCount
    movlw b'00001000'
    movwf PORTH
    movlw b'00111111'
    movwf PORTJ
    ;Initialize Timer1
    movlw b'11000101' 
    movwf T1CON
    
    return

init_rg0 
    i_rg0:
    btfss PORTG, 0
    goto i_rg0
    bsf T0CON, 7
    movlw b'00001111' ;to obtain exactly 1/40 ms between each interrupts
    movwf TMR0L
    movff TMR1L, WREG
    movwf vall_timer1
    movff TMR1H, WREG
    movwf valh_timer1
    return


    
isr:
    movlw b'00001111' ;to obtain exactly 1/40 ms between each interrupts
    movwf TMR0L
    Incf for5sn, 1, 0
    movlw b'11000111'
    cpfsgt for5sn, 0
    bcf INTCON, 2
    cpfsgt for5sn, 0
    retfie FAST; eger daha 200 olmadiysa islem yapma
    
    movff point, pointTemp
    movlw d'0'
    movff WREG, firstDigitCount
    movff WREG, secondDigitCount
    movff WREG, thirdDigitCount
    firstDigitControl:
	loopFirst:
	    movlw d'100'
	    subwf point, 1, 0
	    movff STATUS, stat
	    movff stat, WREG
	    andlw b'00010001'
	    xorlw d'17'
	    bz cont1
	    movff stat, WREG
	    andlw b'00010001'
	    xorlw d'16'
	    bz secondDigitControl
	    cont1:
	    incf firstDigitCount, 1
	    goto loopFirst
	    
    secondDigitControl:
	movlw d'100'
	addwf point, 1
	loopSecond:
	    movlw d'10'
	    subwf point, 1, 0
	    movff STATUS, stat
	    movff stat, WREG
	    andlw b'00010001'
	    xorlw d'17'
	    bz cont2
	    movff stat, WREG
	    andlw b'00010001'
	    xorlw d'16'
	    bz thirdDigitControl
	    cont2:
	    incf secondDigitCount, 1
	    goto loopSecond
	
    thirdDigitControl:
	movlw d'10'
	addwf point, 1
	loopThird:
	    movlw d'1'
	    subwf point, 1
	    movff STATUS, stat
	    movff stat, WREG
	    andlw b'00010001'
	    xorlw d'17'
	    bz cont3
	    movff stat, WREG
	    andlw b'00010001'
	    xorlw d'16'
	    bz write
	    cont3:
	    incf thirdDigitCount, 1
	    goto loopThird
   
    write:	    
    movff segmentDigit, WREG
    xorlw d'0'
    bz leftMostDigitUpdate
    movff segmentDigit, WREG
    xorlw d'1'
    bz middleDigitUpdate
    movff segmentDigit, WREG
    xorlw d'2'
    btfsc STATUS, 2
    goto rightMostDigitUpdate
    
    leftMostDigitUpdate:
	incf segmentDigit, 1, 0
	movlw b'11111111'
	andwf firstDigitCount, 0
	btfsc STATUS, 2
	goto contISR
	movlw b'00000010'
    	movwf PORTH
	movff firstDigitCount, WREG
	xorlw 1
	btfsc STATUS, 2
	goto Digit11
	movff firstDigitCount, WREG
	xorlw 2
	btfsc STATUS, 2
	goto Digit21
	movff firstDigitCount, WREG
	xorlw 3
	btfsc STATUS, 2
	goto Digit31
	movff firstDigitCount, WREG
	xorlw 4
	btfsc STATUS, 2
	goto Digit41
	movff firstDigitCount, WREG
	xorlw 5
	btfsc STATUS, 2
	goto Digit51
	movff firstDigitCount, WREG
	xorlw 6
	btfsc STATUS, 2
	goto Digit61
	movff firstDigitCount, WREG
	xorlw 7
	btfsc STATUS, 2
	goto Digit71
	movff firstDigitCount, WREG
	xorlw 8
	btfsc STATUS, 2
	goto Digit81
	movff firstDigitCount, WREG
	xorlw 9
	btfsc STATUS, 2
	goto Digit91
    middleDigitUpdate:
	incf segmentDigit, 1, 0
	movlw b'11111111'
	andwf firstDigitCount, 0
	btfsc STATUS, 2
	goto middleDigitCheck
	goto writeToMiddle
	
	
    middleDigitCheck:
	movlw b'11111111'
	andwf secondDigitCount, 0
	btfsc STATUS, 2
	goto contISR
	goto writeToMiddle
	
    writeToMiddle:
	movlw b'00000100'
    	movwf PORTH
	movff secondDigitCount, WREG
	xorlw 0
	btfsc STATUS, 2
	goto Digit01
	movff secondDigitCount, WREG
	xorlw 1
	btfsc STATUS, 2
	goto Digit11
	movff secondDigitCount, WREG
	xorlw 2
	btfsc STATUS, 2
	goto Digit21
	movff secondDigitCount, WREG
	xorlw 3
	btfsc STATUS, 2
	goto Digit31
	movff secondDigitCount, WREG
	xorlw 4
	btfsc STATUS, 2
	goto Digit41
	movff secondDigitCount, WREG
	xorlw 5
	btfsc STATUS, 2
	goto Digit51
	movff secondDigitCount, WREG
	xorlw 6
	btfsc STATUS, 2
	goto Digit61
	movff secondDigitCount, WREG
	xorlw 7
	btfsc STATUS, 2
	goto Digit71
	movff secondDigitCount, WREG
	xorlw 8
	btfsc STATUS, 2
	goto Digit81
	movff secondDigitCount, WREG
	xorlw 9
	btfsc STATUS, 2
	goto Digit91
    rightMostDigitUpdate:
	movlw d'0'
	movff WREG, segmentDigit
	movlw b'00001000'
    	movwf PORTH
	movff thirdDigitCount, WREG
	xorlw 0
	btfsc STATUS, 2
	goto Digit01
	movff thirdDigitCount, WREG
	xorlw 1
	btfsc STATUS, 2
	goto Digit11
	movff thirdDigitCount, WREG
	xorlw 2
	btfsc STATUS, 2
	goto Digit21
	movff thirdDigitCount, WREG
	xorlw 3
	btfsc STATUS, 2
	goto Digit31
	movff thirdDigitCount, WREG
	xorlw 4
	btfsc STATUS, 2
	goto Digit41
	movff thirdDigitCount, WREG
	xorlw 5
	btfsc STATUS, 2
	goto Digit51
	movff thirdDigitCount, WREG
	xorlw 6
	btfsc STATUS, 2
	goto Digit61
	movff thirdDigitCount, WREG
	xorlw 7
	btfsc STATUS, 2
	goto Digit71
	movff thirdDigitCount, WREG
	xorlw 8
	btfsc STATUS, 2
	goto Digit81
	movff thirdDigitCount, WREG
	xorlw 9
	btfsc STATUS, 2
	goto Digit91
	
    Digit01:
	movff digit0 , PORTJ
	goto contISR
    Digit11:
	movff digit1 , PORTJ
	goto contISR
    Digit21:
	movff digit2 , PORTJ
	goto contISR
    Digit31:
	movff digit3 , PORTJ
	goto contISR
    Digit41:
	movff digit4 , PORTJ
	goto contISR
    Digit51:
	movff digit5 , PORTJ
	goto contISR
    Digit61:
	movff digit6 , PORTJ
	goto contISR
    Digit71:
	movff digit7 , PORTJ
	goto contISR
    Digit81:
	movff digit8 , PORTJ
	goto contISR
    Digit91:
	movff digit9 , PORTJ

    
    contISR:
    movff pointTemp, point
    clrf for5sn ;200'e kadar yeniden saysin
    movlw b'00001010'
    cpfsgt num_asteroids, 0
    goto first10
    movlw b'00011110'
    cpfsgt num_asteroids, 0
    goto first30
    movlw b'00110010'
    cpfsgt num_asteroids, 0
    goto first50
    goto after50

    
first10:
    Incf counter, 1, 0
    movlw b'01100011'
    cpfsgt counter, 0 ;99'dan buyukse
    bcf INTCON, 2
    cpfsgt counter, 0
    retfie FAST
    goto shifting
    
first30:
    Incf counter, 1, 0
    movlw b'01001111' ;79'dan buyukse
    cpfsgt counter, 0
    bcf INTCON, 2
    cpfsgt counter, 0
    retfie FAST
    goto shifting    
    
first50:
    Incf counter, 1, 0
    movlw b'00111011' ;59'dan buyukse
    cpfsgt counter, 0
    bcf INTCON, 2
    cpfsgt counter, 0
    retfie FAST
    goto shifting 
    
after50:
    Incf counter, 1, 0
    movlw b'00100111' ;39'dan buyukse
    cpfsgt counter, 0
    bcf INTCON, 2
    cpfsgt counter, 0
    retfie  FAST
    goto shifting     
    
shifting:
    clrf counter
    
s_1:    
    movff aline1, WREG
    andwf bline1, 0, 0
    bnz c1    
s_2:    
    movff aline2, WREG
    andwf bline2, 0, 0
    bnz c2
s_3:    
    movff aline3, WREG
    andwf bline3, 0, 0
    bnz c3
s_4:    
    movff aline4, WREG
    andwf bline4, 0, 0
    bnz c4
s_5:    
    movff aline5, WREG
    andwf bline5, 0, 0
    bnz c5
s_6:    
    movff aline6, WREG
    andwf bline6, 0, 0
    bnz c6
    goto shift_beg    
    
c1:
    xorwf aline1, 1, 0
    xorwf bline1, 1, 0
    Incf point, 1, 0
    goto s_2
c2:
    xorwf aline2, 1, 0
    xorwf bline2, 1, 0
    Incf point, 1, 0
    goto s_3
c3:
    xorwf aline3, 1, 0
    xorwf bline3, 1, 0    
    Incf point, 1, 0
    goto s_4
c4:
    xorwf aline4, 1, 0
    xorwf bline4, 1, 0    
    Incf point, 1, 0
    goto s_5
c5:
    xorwf aline5, 1, 0
    xorwf bline5, 1, 0    
    Incf point, 1, 0
    goto s_6
c6:
    xorwf aline6, 1, 0
    xorwf bline6, 1, 0    
    Incf point, 1, 0
    goto shift_beg
    
shift_beg:    
    rlcf aline1, 0, 0
    movff bline1, btemp
    xorwf bline1, 1, 0 ;bline1'e yaz
    movff btemp, WREG
    andwf bline1, 1
    movff aline1, atemp
    rrcf btemp, 0, 0 ;wreg'e yaz
    xorwf aline1, 1, 0
    movff atemp, WREG
    andwf aline1, 1
    movlw b'01111110'
    andwf aline1, 1
    andwf bline1, 1
    andwf btemp, 1 ;puan guncellemesinde kullanilacak
    movff bline1, WREG
    xorwf btemp, 0, 0
    bnz d1  
dd2:    
    rlcf aline2, 0, 0
    movff bline2, btemp
    xorwf bline2, 1, 0 ;bline1'e yaz
    movff btemp, WREG
    andwf bline2, 1
    movff aline2, atemp
    rrcf btemp, 0, 0 ;wreg'e yaz
    xorwf aline2, 1, 0
    movff atemp, WREG
    andwf aline2, 1
    movlw b'01111110'
    andwf aline2, 1
    andwf bline2, 1
    andwf btemp, 1    
    movff bline2, WREG
    xorwf btemp, 0, 0
    bnz d2
dd3:    
    rlcf aline3, 0, 0
    movff bline3, btemp
    xorwf bline3, 1, 0 ;bline1'e yaz
    movff btemp, WREG
    andwf bline3, 1
    movff aline3, atemp
    rrcf btemp, 0, 0 ;wreg'e yaz
    xorwf aline3, 1, 0
    movff atemp, WREG
    andwf aline3, 1
    movlw b'01111110'
    andwf aline3, 1
    andwf bline3, 1
    andwf btemp, 1    
    movff bline3, WREG
    xorwf btemp, 0, 0
    bnz d3
dd4:    
    rlcf aline4, 0, 0
    movff bline4, btemp
    xorwf bline4, 1, 0 ;bline1'e yaz
    movff btemp, WREG
    andwf bline4, 1
    movff aline4, atemp
    rrcf btemp, 0, 0 ;wreg'e yaz
    xorwf aline4, 1, 0
    movff atemp, WREG
    andwf aline4, 1
    movlw b'01111110'
    andwf aline4, 1
    andwf bline4, 1
    andwf btemp, 1    
    movff bline4, WREG
    xorwf btemp, 0, 0
    bnz d4
dd5:    
    rlcf aline5, 0, 0
    movff bline5, btemp
    xorwf bline5, 1, 0 ;bline1'e yaz
    movff btemp, WREG
    andwf bline5, 1
    movff aline5, atemp
    rrcf btemp, 0, 0 ;wreg'e yaz
    xorwf aline5, 1, 0
    movff atemp, WREG
    andwf aline5, 1
    movlw b'01111110'
    andwf aline5, 1
    andwf bline5, 1
    andwf btemp, 1    
    movff bline5, WREG
    xorwf btemp, 0, 0
    bnz d5
dd6:    
    rlcf aline6, 0, 0
    movff bline6, btemp
    xorwf bline6, 1, 0 ;bline1'e yaz
    movff btemp, WREG
    andwf bline6, 1
    movff aline6, atemp
    rrcf btemp, 0, 0 ;wreg'e yaz
    xorwf aline6, 1, 0
    movff atemp, WREG
    andwf aline6, 1
    movlw b'01111110'
    andwf aline6, 1
    andwf bline6, 1
    andwf btemp, 1  
    movff bline6, WREG
    xorwf btemp, 0, 0
    bnz d6
    goto notScore

d1:
    Incf point, 1, 0
    goto dd2
d2:
    Incf point, 1, 0
    goto dd3
d3:
    Incf point, 1, 0
    goto dd4
d4:
    Incf point, 1, 0
    goto dd5
d5:
    Incf point, 1, 0
    goto dd6
d6:
    Incf point, 1, 0
    goto notScore
    
notScore:
    rlcf aline1
    rlcf aline2
    rlcf aline3
    rlcf aline4
    rlcf aline5
    rlcf aline6
    rrcf bline1
    rrcf bline2
    rrcf bline3
    rrcf bline4
    rrcf bline5
    rrcf bline6
    
    movlw b'01111110'
    andwf aline1, 1, 0
    andwf bline1, 1, 0
    movff aline1, WREG
    andwf bline1, 0 ,0
    movwf temp3
    movff aline1, WREG
    xorwf bline1, 0, 0 ;wreg'e yaz
    andwf aline1, 1, 0
    andwf bline1, 1, 0
    movff temp3, WREG
    Iorwf bline1, 1, 0
    Iorwf aline1, 1, 0

    movlw b'01111110'
    andwf aline2, 1, 0
    andwf bline2, 1, 0
    movff aline2, WREG
    andwf bline2, 0 ,0
    movwf temp3
    movff aline2, WREG
    xorwf bline2, 0, 0 ;wreg'e yaz
    andwf aline2, 1, 0
    andwf bline2, 1, 0
    movff temp3, WREG
    Iorwf bline2, 1, 0
    Iorwf aline2, 1, 0
    
    movlw b'01111110'
    andwf aline3, 1, 0
    andwf bline3, 1, 0
    movff aline3, WREG
    andwf bline3, 0 ,0
    movwf temp3
    movff aline3, WREG
    xorwf bline3, 0, 0 ;wreg'e yaz
    andwf aline3, 1, 0
    andwf bline3, 1, 0
    movff temp3, WREG
    Iorwf bline3, 1, 0
    Iorwf aline3, 1, 0
    
    movlw b'01111110'
    andwf aline4, 1, 0
    andwf bline4, 1, 0
    movff aline4, WREG
    andwf bline4, 0 ,0
    movwf temp3
    movff aline4, WREG
    xorwf bline4, 0, 0 ;wreg'e yaz
    andwf aline4, 1, 0
    andwf bline4, 1, 0
    movff temp3, WREG
    Iorwf bline4, 1, 0
    Iorwf aline4, 1, 0
    
    movlw b'01111110'
    andwf aline5, 1, 0
    andwf bline5, 1, 0
    movff aline5, WREG
    andwf bline5, 0 ,0
    movwf temp3
    movff aline5, WREG
    xorwf bline5, 0, 0 ;wreg'e yaz
    andwf aline5, 1, 0
    andwf bline5, 1, 0
    movff temp3, WREG
    Iorwf bline5, 1, 0
    Iorwf aline5, 1, 0

    movlw b'01111110'
    andwf aline6, 1, 0
    andwf bline6, 1, 0
    movff aline6, WREG
    andwf bline6, 0 ,0
    movwf temp3
    movff aline6, WREG
    xorwf bline6, 0, 0 ;wreg'e yaz
    andwf aline6, 1, 0
    andwf bline6, 1, 0
    movff temp3, WREG
    Iorwf bline6, 1, 0
    Iorwf aline6, 1, 0

asteroid_creation:
    movlw b'00110011'
    cpfsgt num_asteroids
    Incf num_asteroids, 1, 0
    movff vall_timer1, WREG
    andlw b'00000111'    
   
createAsteroid:
    goto modulo
modRet:
    movff WREG, temp2
    xorlw d'0'
    bz createAsteroidOn0
    movff temp2,WREG
    xorlw d'1'
    bz createAsteroidOn1
    movff temp2,WREG
    xorlw d'2'
    bz createAsteroidOn2
    movff temp2,WREG
    xorlw d'3'
    bz createAsteroidOn3
    movff temp2,WREG
    xorlw d'4'
    bz createAsteroidOn4
    movff temp2,WREG
    xorlw d'5'
    bz createAsteroidOn5
   
createAsteroidOn0:
    bsf aline1, 1
    goto update
createAsteroidOn1:
    bsf aline2, 1
    goto update    
createAsteroidOn2: 
    bsf aline3, 1
    goto update
createAsteroidOn3:
    bsf aline4, 1
    goto update
createAsteroidOn4:
    bsf aline5, 1
    goto update
createAsteroidOn5:
    bsf aline6, 1
    goto update
    
update:
    movff aline1, WREG
    Iorwf bline1, 0
    bcf LATF, 0
    btfsc WREG,1
    bsf LATF, 0
    bcf LATE, 0
    btfsc WREG,2
    bsf LATE, 0
    bcf LATD, 0
    btfsc WREG,3
    bsf LATD, 0
    bcf LATC, 0    
    btfsc WREG,4
    bsf LATC, 0
    bcf LATB, 0
    btfsc WREG,5
    bsf LATB, 0
    bcf LATA, 0
    btfsc WREG,6
    bsf LATA, 0
    
    movff aline2, WREG
    Iorwf bline2, 0
    bcf LATF, 1
    btfsc WREG,1
    bsf LATF, 1
    bcf LATE, 1
    btfsc WREG,2
    bsf LATE, 1
    bcf LATD, 1
    btfsc WREG,3
    bsf LATD, 1
    bcf LATC, 1
    btfsc WREG,4
    bsf LATC, 1
    bcf LATB, 1
    btfsc WREG,5
    bsf LATB, 1
    bcf LATA, 1
    btfsc WREG,6
    bsf LATA, 1
    
    movff aline3, WREG
    Iorwf bline3, 0
    bcf LATF, 2
    btfsc WREG,1
    bsf LATF, 2
    bcf LATE, 2
    btfsc WREG,2
    bsf LATE, 2
    bcf LATD, 2
    btfsc WREG,3
    bsf LATD, 2
    bcf LATC, 2
    btfsc WREG,4
    bsf LATC, 2
    bcf LATB, 2
    btfsc WREG,5
    bsf LATB, 2
    bcf LATA, 2
    btfsc WREG,6
    bsf LATA, 2
    
    movff aline4, WREG
    Iorwf bline4, 0
    bcf LATF, 3
    btfsc WREG,1
    bsf LATF, 3
    bcf LATE, 3
    btfsc WREG,2
    bsf LATE, 3
    bcf LATD, 3
    btfsc WREG,3
    bsf LATD, 3
    bcf LATC, 3
    btfsc WREG,4
    bsf LATC, 3
    bcf LATB, 3
    btfsc WREG,5
    bsf LATB, 3
    bcf LATA, 3
    btfsc WREG,6
    bsf LATA, 3
    
    movff aline5, WREG
    Iorwf bline5, 0
    bcf LATF, 4
    btfsc WREG,1
    bsf LATF, 4
    bcf LATE, 4
    btfsc WREG,2
    bsf LATE, 4
    bcf LATD, 4
    btfsc WREG,3
    bsf LATD, 4
    bcf LATC, 4
    btfsc WREG,4
    bsf LATC, 4
    bcf LATB, 4
    btfsc WREG,5
    bsf LATB, 4
    bcf LATA, 4
    btfsc WREG,6
    bsf LATA, 4
    
    movff aline6, WREG
    Iorwf bline6, 0
    bcf LATF, 5
    btfsc WREG,1
    bsf LATF, 5
    bcf LATE, 5
    btfsc WREG,2
    bsf LATE, 5
    bcf LATD, 5
    btfsc WREG,3
    bsf LATD, 5
    bcf LATC, 5
    btfsc WREG,4
    bsf LATC, 5
    bcf LATB, 5
    btfsc WREG,5
    bsf LATB, 5
    bcf LATA, 5
    btfsc WREG,6
    bsf LATA, 5
    
    movff shipLine, WREG
    xorlw 1
    bz checkLATA0
    movff shipLine, WREG
    xorlw 2
    bz checkLATA1
    movff shipLine, WREG
    xorlw 3
    bz checkLATA2
    movff shipLine, WREG
    xorlw 4
    bz checkLATA3
    movff shipLine, WREG
    xorlw 5
    bz checkLATA4
    movff shipLine, WREG
    xorlw 6
    bz checkLATA5
    
    checkLATA0:
    btfsc LATA, 0
    bsf gameOver, 0
    bsf LATA, 0
    goto timer1Shift
    checkLATA1:
    btfsc LATA, 1
    bsf gameOver, 0
    bsf LATA, 1
    goto timer1Shift
    checkLATA2:
    btfsc LATA, 2
    bsf gameOver, 0
    bsf LATA, 2
    goto timer1Shift
    checkLATA3:
    btfsc LATA, 3
    bsf gameOver, 0
    bsf LATA, 3
    goto timer1Shift
    checkLATA4:
    btfsc LATA, 4
    bsf gameOver, 0
    bsf LATA, 4
    goto timer1Shift
    checkLATA5:
    btfsc LATA, 5
    bsf gameOver, 0
    bsf LATA, 5
    goto timer1Shift
    timer1Shift:
    movff vall_timer1, temp2
    rrcf vall_timer1
    bcf vall_timer1, 7
    btfsc valh_timer1,0
    bsf vall_timer1, 7
    
    rrcf valh_timer1
    bcf valh_timer1, 7
    btfsc temp2, 0
    bsf valh_timer1, 7
    
    Incf shiftCount, 1, 0
    movff shiftCount, WREG
    xorlw d'10'
    bz complement    
    bcf INTCON, 2
    retfie FAST
complement:
    movlw d'0'
    movwf shiftCount
    comf valh_timer1
    comf vall_timer1
    bcf INTCON, 2
    retfie FAST
        
modulo:
    movff WREG, temp2
    moduloLoop:
	cpfslt lineCount
	goto moduloEnd
	
	movlw d'6'
	subwf temp2,0
	movff WREG,temp2
	goto moduloLoop
    moduloEnd:
	goto modRet

checkUpMove
    btfss PORTG, 3
    return
    releaseUp:
	btfsc PORTG, 3
	goto releaseUp
	moveUp:
	    movff shipLine, WREG
	    xorlw 1
	    bz returnMove
	    movff shipLine, WREG
	    xorlw 2
	    bz checkLAT02
	    movff shipLine, WREG
	    xorlw 3
	    bz checkLAT12
	    movff shipLine, WREG
	    xorlw 4
	    bz checkLAT22
	    movff shipLine, WREG
	    xorlw 5
	    bz checkLAT32
	    movff shipLine, WREG
	    xorlw 6
	    bz checkLAT42
	    
	    checkLAT02:
		btfsc LATA, 0
		bsf gameOver, 0
		btfsc LATA, 0
		goto returnMove
		bcf LATA,1
		bsf LATA,0
		goto preReturn
    	    checkLAT12:
		btfsc LATA, 1
		bsf gameOver, 0
		btfsc LATA, 1
		goto returnMove
		bcf LATA,2
		bsf LATA,1
		goto preReturn
    	    checkLAT22:
		btfsc LATA, 2
		bsf gameOver, 0
		btfsc LATA, 2
		goto returnMove
		bcf LATA,3
		bsf LATA,2
		goto preReturn
    	    checkLAT32:
		btfsc LATA, 3
		bsf gameOver, 0
		btfsc LATA, 3
		goto returnMove
		bcf LATA,4
		bsf LATA,3
		goto preReturn
    	    checkLAT42:
		btfsc LATA, 4
		bsf gameOver, 0
		btfsc LATA, 4
		goto returnMove
		bcf LATA,5
		bsf LATA,4
		goto preReturn
	    preReturn:
	    decf shipLine, 1
	    returnMove:
	    return
	    
checkDownMove
    btfss PORTG, 2
    return
    releaseDown:
	btfsc PORTG, 2
	goto releaseDown
	moveDown:
	    movff shipLine, WREG
	    xorlw 6
	    bz returnMove2
	    movff shipLine, WREG
	    xorlw 1
	    bz checkLAT13
	    movff shipLine, WREG
	    xorlw 2
	    bz checkLAT23
	    movff shipLine, WREG
	    xorlw 3
	    bz checkLAT33
	    movff shipLine, WREG
	    xorlw 4
	    bz checkLAT43
	    movff shipLine, WREG
	    xorlw 5
	    bz checkLAT53
	    
	    checkLAT13:
		btfsc LATA, 1
		bsf gameOver, 0
		btfsc LATA, 1
		goto returnMove2
		bcf LATA,0
		bsf LATA,1
		goto preReturn2
    	    checkLAT23:
		btfsc LATA, 2
		bsf gameOver, 0
		btfsc LATA, 2
		goto returnMove2
		bcf LATA,1
		bsf LATA,2
		goto preReturn2
    	    checkLAT33:
		btfsc LATA, 3
		bsf gameOver, 0
		btfsc LATA, 3
		goto returnMove2
		bcf LATA,2
		bsf LATA,3
		goto preReturn2
    	    checkLAT43:
		btfsc LATA, 4
		bsf gameOver, 0
		btfsc LATA, 4
		goto returnMove2
		bcf LATA,3
		bsf LATA,4
		goto preReturn2
	    checkLAT53:
		btfsc LATA, 5
		bsf gameOver, 0
		btfsc LATA, 5
		goto returnMove2
		bcf LATA,4
		bsf LATA,5
		goto preReturn2	
	    preReturn2:
	    incf shipLine, 1
	    returnMove2:
	    return    

checkBeamFire
    btfss PORTG, 1
    return
    releaseBeam:
	btfsc PORTG, 1
	goto releaseBeam
	beamFire: ;SHIPLINEA GORE KONTROL ET
	    movff shipLine, WREG
	    xorlw d'1'
	    bz l0
	    movff shipLine, WREG
	    xorlw d'2'
	    bz l1
	    movff shipLine, WREG
	    xorlw d'3'
	    bz l2
	    movff shipLine, WREG
	    xorlw d'4'
	    bz l3
	    movff shipLine, WREG
	    xorlw d'5'
	    bz l4
	    movff shipLine, WREG
	    xorlw d'6'
	    bz l5
	beamEnd:
    return
	    
l0:
    btfsc aline1, 5
    goto collisionAtStart0
    btfsc bline1, 5
    goto beamEnd
    bsf bline1, 5
    bsf LATB, 0
    goto beamEnd

collisionAtStart0:
    incf point, 1, 0
    bcf aline1,5
    goto beamEnd

l1:
    btfsc aline2, 5
    goto collisionAtStart1
    btfsc bline2, 5
    goto beamEnd
    bsf bline2, 5
    bsf LATB, 1
    goto beamEnd

collisionAtStart1:
    incf point, 1, 0
    bcf aline2,5
    goto beamEnd    
    
l2:
    btfsc aline3, 5
    goto collisionAtStart2
    btfsc bline3, 5
    goto beamEnd
    bsf bline3, 5
    bsf LATB, 2 
    goto beamEnd

collisionAtStart2:
    incf point, 1, 0
    bcf aline3,5
    goto beamEnd
    
l3:
    btfsc aline4, 5
    goto collisionAtStart3
    btfsc bline4, 5
    goto beamEnd
    bsf bline4, 5
    bsf LATB, 3
    goto beamEnd

collisionAtStart3:
    incf point, 1, 0
    bcf aline4,5
    goto beamEnd
    
l4:
    btfsc aline5, 5
    goto collisionAtStart4
    btfsc bline5, 5
    goto beamEnd
    bsf bline5, 5
    bsf LATB, 4
    goto beamEnd
    
collisionAtStart4:
    incf point, 1, 0
    bcf aline5,5
    goto beamEnd    
l5:
    btfsc aline6, 5
    goto collisionAtStart5
    btfsc bline6, 5
    goto beamEnd
    bsf bline6, 5
    bsf LATB, 5
    goto beamEnd

collisionAtStart5:
    incf point, 1, 0
    bcf aline6,5
    goto beamEnd    




game
    call checkUpMove
    call checkDownMove
    call checkBeamFire
    btfss gameOver, 0
    goto game
    return	
	
main:
    call init
    call init_rg0    
    call game
    goto main
end    