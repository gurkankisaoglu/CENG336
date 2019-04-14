
LIST    P=18F8722

#INCLUDE <p18f8722.inc> 

CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

mode udata 0x20
mode
 
current udata 0x24
current
 

ra4 udata 0x40
ra4  
 
REG5 udata 0x28
REG5

REG6 udata 0x32
REG6

REG7 udata 0x36
REG7
 
 
ORG     0x00
goto    main


init:
    movlw b'11111111'
    movwf ADCON1    
    clrf PORTA 
    clrf LATA
    movlw b'00010000'
    movwf TRISA

    movlw b'00000000'
    clrf PORTB 
    clrf LATB
    movwf TRISB

    movlw b'00000000'
    clrf PORTC 
    clrf LATC
    movwf TRISC

    movlw b'00000000'
    clrf PORTD
    clrf LATD
    movwf TRISD

    movlw b'00011000'
    clrf PORTE
    clrf LATE
    movwf TRISE

    movlw b'00001111' 
    movwf LATA
    movwf LATB
    movwf LATC 
    movwf LATD
    call wait
    call wait
    call wait
    call wait

    movlw b'00000000' 
    movwf LATA
    movwf LATB
    movwf LATC 
    movwf LATD
    movlw b'00000001'
    movwf mode
    call wait
    call wait

call start  
    
setRA4
    movlw h'01'
    movwf ra4
    return
    
checkPressRelease    
    btfsc ra4, 0
    goto waitState
    return
wait 
    MOVLW	h'a0'
    MOVWF	REG7
    Loop3:
    MOVLW	h'c4'
    MOVWF	REG6
    Loop2:
    MOVLW	h'0b'
    MOVWF	REG5
    Loop1:
    btfsc PORTA, 4
    call setRA4    
    btfss PORTA, 4
    call checkPressRelease
    DECFSZ	REG5,F
    GOTO	Loop1
    DECFSZ	REG6,F
    GOTO	Loop2
    DECFSZ	REG7,F
    GOTO	Loop3
    RETURN

start:
    goto ledA1
    ledTurnOffA:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000000'
	movwf current
	movwf LATA
	call wait
	goto waitState
    ledA1:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000001'
	movwf LATA
	call wait
	movwf current
	btfss mode, 0
	goto ledTurnOffA
    ledA2:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000011'
	movwf LATA
	movlw b'00000010'
	movwf current
	call wait
	btfss mode, 0
	goto ledA1
    ledA3:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000111'
	movwf LATA
	movlw b'00000011'
	movwf current
	call wait
	btfss mode, 0
	goto ledA2
    ledA4:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000000'
	movwf LATB
	movlw b'00001111'
	movwf LATA
	movlw b'00000100'
	movwf current
	call wait
	btfss mode, 0
	goto ledA3
    ledB1:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000001'
	movwf LATB
	movlw b'00000101'
	movwf current
	call wait
	btfss mode, 0
	goto ledA4
    ledB2:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000011'
	movwf LATB
	movlw b'00000110'
	movwf current
	call wait
	btfss mode, 0
	goto ledB1
    ledB3:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000111'
	movwf LATB
	movlw b'00000111'
	movwf current
	call wait
	btfss mode, 0
	goto ledB2
    ledB4:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000000'
	movwf LATC
	movlw b'00001111'
	movwf LATB
	movlw b'00001000'
	movwf current
	call wait
	btfss mode, 0
	goto ledB3
    ledC1: 
	btfsc PORTA, 4
	goto waitState
	movlw b'00000001'
	movwf LATC
	movlw b'00001001'
	movwf current
	call wait
	btfss mode, 0
	goto ledB4
    ledC2:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000011'
	movwf LATC
	movlw b'00001010'
	movwf current
	call wait	
	btfss mode, 0
	goto ledC1
    ledC3:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000111'
	movwf LATC
	movlw b'00001011'
	movwf current
	call wait
	btfss mode, 0
	goto ledC2
    ledC4:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000000'
	movwf LATD
	movlw b'00001111'
	movwf LATC	
	movlw b'00001100'
	movwf current
	call wait
	btfss mode, 0
	goto ledC3
    ledD1:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000001'
	movwf LATD
	movlw b'00001101'
	movwf current
	call wait
	btfss mode, 0
	goto ledC4
    ledD2:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000011'
	movwf LATD
	movlw b'00001110'
	movwf current
	call wait
	btfss mode, 0
	goto ledD1
	
    ledD3:
	btfsc PORTA, 4
	goto waitState
	movlw b'00000111'
	movwf LATD
	movlw b'00001111'
	movwf current
	call wait
	btfss mode, 0
	goto ledD2
	
    ledD4:
	btfsc PORTA, 4
	goto waitState
	movlw b'00001111'
	movwf LATD
	movlw b'00010000'
	movwf current
	call wait
	btfss mode, 0
	goto ledD3
	goto waitState
	
	
    waitState:	
    movlw h'00'
    movwf ra4
    re3press:
	btfss PORTE, 3
	goto re4press
    re3release:
	btfsc PORTE, 3
	goto re3release
	movlw b'00000001'
	movwf mode
	movff current, WREG
	xorlw 0
	cpfslt 0
	goto  ledA1
	movff current, WREG
	xorlw 1
	cpfslt 0
	goto  ledA2
	movff current, WREG
	xorlw 2
	cpfslt 0
	goto  ledA3
	movff current, WREG
	xorlw 3
	cpfslt 0
	goto  ledA4
	movff current, WREG
	xorlw 4
	cpfslt 0 
	goto  ledB1
	movff current, WREG
	xorlw 5
	cpfslt 0 
	goto  ledB2
	movff current, WREG
	xorlw 6
	cpfslt 0 
	goto  ledB3
	movff current, WREG
	xorlw 7
	cpfslt 0 
	goto  ledB4
	movff current, WREG
	xorlw 8
	cpfslt 0 
	goto  ledC1
	movff current, WREG
	xorlw 9
	cpfslt 0 
	goto  ledC2
	movff current, WREG
	xorlw h'A'
	cpfslt 0 
	goto  ledC3
	movff current, WREG
	xorlw h'B'
	cpfslt 0 
	goto  ledC4
	movff current, WREG
	xorlw h'C'
	cpfslt 0 
	goto  ledD1
	movff current, WREG
	xorlw h'D'
	cpfslt 0 
	goto  ledD2
	movff current, WREG
	xorlw h'E'
	cpfslt 0 
	goto  ledD3
	movff current, WREG
	xorlw h'F'
	cpfslt 0 
	goto  ledD4
	goto waitState
	
    re4press:
	btfss PORTE, 4
	goto re3press
    re4release:
	btfsc PORTE, 4
	goto re4release
	movlw b'00000000'
	movwf mode
	movff current, WREG
	xorlw 1
	cpfslt 0 
	goto ledTurnOffA
	movff current, WREG
	xorlw 2
	cpfslt 0 
	goto ledA1
	movff current, WREG
	xorlw 3
	cpfslt 0 
	goto  ledA2
	movff current, WREG
	xorlw 4
	cpfslt 0 
	goto  ledA3
	movff current, WREG
	xorlw 5
	cpfslt 0 
	goto  ledA4
	movff current, WREG
	xorlw 6
	cpfslt 0 
	goto  ledB1
	movff current, WREG
	xorlw 7
	cpfslt 0 
	goto  ledB2
	movff current, WREG
	xorlw 8
	cpfslt 0 
	goto  ledB3
	movff current, WREG
	xorlw 9
	cpfslt 0 
	goto  ledB4
	movff current, WREG
	xorlw h'A'
	cpfslt 0 
	goto  ledC1
	movff current, WREG
	xorlw h'B'
	cpfslt 0 
	goto  ledC2
	movff current, WREG
	xorlw h'C'
	cpfslt 0 
	goto  ledC3
	movff current, WREG
	xorlw h'D'
	cpfslt 0 
	goto  ledC4
	movff current, WREG
	xorlw h'E'
	cpfslt 0 
	goto  ledD1
	movff current, WREG
	xorlw h'F'
	cpfslt 0 
	goto  ledD2
	movff current, WREG
	xorlw 10
	cpfslt 0 
	goto  ledD3
	goto waitState

main:
    call init
end


