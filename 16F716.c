
#include <pic.h>

#include "16F716.h"


__CONFIG(0x1FF6);


void InitPort(void)
{
    TRISA = 0xff;
    TRISB = 0xff;
    PORTA = 0xff;
    PORTB = 0xff;
	

	// 핀 방향 설정
	TRIS_PIN_TEMP_IN 		= 1; 
	TRIS_PIN_TEMP_SET_LED	= 0; 
	
	TRIS_PIN_SW_BCD_1		= 1;		 
	TRIS_PIN_SW_BCD_2	 	= 1;		
	TRIS_PIN_SW_BCD_3		= 1;		
	TRIS_PIN_SW_BCD_4		= 1;	
	
	TRIS_PIN_HEATER_OUT 	= 0;
	TRIS_PIN_HEATER_LED 	= 0;	
	TRIS_PIN_ERR_ALARM_LED	= 0;	


	// 핀 값 설정 
	PIN_TEMP_IN				= 0;			
	PIN_TEMP_SET_LED 		= OFF_LED;
	
	PIN_SW_BCD_1			= 1;	 // if 1 이면, 마이컴 입장에선 0 값이 입력 되는 것이다.  	
	PIN_SW_BCD_2	 		= 1;			
	PIN_SW_BCD_3			= 1;			 		
	PIN_SW_BCD_4			= 1;	
	
	PIN_HEATER_OUT			= OFF_RELAY;	
	PIN_HEATER_LED			= OFF_LED;	
	PIN_ERR_ALARM_LED		= OFF_LED;	
}

void  InitTMR0_REGISTER(void)
{
  /* 1 PwOnDelay timer */
	T0IE=1;
	T0IF=0;
    
	T0CS=0;    	// TMR0 Source Select ..(0 = internal clock)  	
	PSA=0;	    // Prescaler Assignment ..(0 = Prescaler is assigned to the Timer0 module)

	PS2=0;	    // Prescaler
	PS1=1;
	PS0=0;     

	TMR0 = TMR0_MSEC;
} 



// RA0 ~ 3 포트 핀 D/A 설정 
void InitADCON1_REGISTER(void)
{
	// D/A 설정 
	ADCON1bits.PCFG2 = 1;
	ADCON1bits.PCFG1 = 0;
	ADCON1bits.PCFG0 = 0; // RA0,1,3 = Analog, RA2 = Digital
}


void	InitAD(void)
{
	ADCON0 = 0x00;
	ADCON1 = 0x00;

	// ADON: A/D On bit
	ADCON0bits.ADON = 1;

	// ADCS1:ADCS0: A/D Conversion Clock(1Tad) 시간 값 설정 Select bits 
	// Fosc = 4 Mhz / 4 = 1Mhz(1us)
	// 1Tad 는 최소 1.6us 이상이여야 한다. 
	// ADCS1:ADCS1 = 00 = F OSC /2 = 2us = 1Tad
	// 8비트 ADRES를 사용 하므로 8Tad + 2Tad = 10Tad 이상이 되어야 한다.  
	ADCON0bits.ADCS1 = 0;
	ADCON0bits.ADCS0 = 0;
	// CHS2:CHS0: Analog Channel Select bits
	ADCON0bits.CHS2 = 0;
	ADCON0bits.CHS1 = 0;
	ADCON0bits.CHS0 = 0;
	// GO/DONE: A/D Conversion Status bit
	ADCON0bits.GO_DONE = 0;

	// PCFG2:PCFG0: A/D Port Configuration Control bits
	ADCON1bits.PCFG2 = 1;
	ADCON1bits.PCFG1 = 0;
	ADCON1bits.PCFG0 = 0; // RA0,1,3 = Analog, RA2 = Digital


	ADRES = 0; // 8비트 AD 저장 레지스터 
	ADIF = 0; // ADIF: A/D Converter Interrupt Flag bit
	ADIE = 1; // ADIE: A/D Converter Interrupt Enable bit
	// acquisition time 1Tad = 2 usec >= 1.6usec
}






void  InitCPU(void)
{
	STATUS = 0x00;


}


