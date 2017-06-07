
#ifndef _16F716_H_
#define _16F716_H_



/////////////////////////////////////////////////////////////////////////////
// 핀 정의

// PORT_A
#define PIN_TEMP_IN				RA0 // 온도 입력 
#define TRIS_PIN_TEMP_IN	 	TRISA0

#define PIN_TEMP_SET_LED	 	RA4	// 온도 SET 상태 LED 
#define TRIS_PIN_TEMP_SET_LED	TRISA4 


// PORT_B
#define PIN_SW_BCD_1			RB0 // SW BCD 1
#define TRIS_PIN_SW_BCD_1 		TRISB0 

#define PIN_SW_BCD_2	 		RB1	// SW BCD 2
#define TRIS_PIN_SW_BCD_2	 	TRISB1 	

#define PIN_SW_BCD_3			RB2	// SW BCD 3 
#define TRIS_PIN_SW_BCD_3 		TRISB2 	 

#define PIN_SW_BCD_4			RB3	// SW BCD 4
#define TRIS_PIN_SW_BCD_4 		TRISB3 

#define PIN_HEATER_OUT			RB4 // 히터 출력 릴레이 
#define TRIS_PIN_HEATER_OUT	 	TRISB4 

#define PIN_HEATER_LED			RB5 // 히터 상태 LED 
#define TRIS_PIN_HEATER_LED		TRISB5

#define PIN_ERR_ALARM_LED 		RB6		// PGC , 고장 알람 LED 
#define TRIS_PIN_ERR_ALARM_LED 	TRISB6

/////////////////////////////////////////////////////////////////////////////////////

#define TRUE	1
#define FALSE 	0

#define ON_RELAY	0
#define OFF_RELAY	1

#define ON_LED		0
#define OFF_LED		1



#define     TMR0_MSEC    131 // 35 -> 1 PwOnDelay


extern void InitPort(void);
extern void InitADCON1_REGISTER(void);
extern void  InitCPU(void);
extern void  InitTMR0_REGISTER(void);
extern void	InitAD(void);


#endif

