
#ifndef _MAIN_H_
#define _MAIN_H_

//#define __TEMP_1000		1
#define	__TEMP_100		1	// PT-100옴 to Voltage(MCP-602),,,SGPT101 용 


#define TRUE	1
#define FALSE 	0


#define ADCH_MAX	2
#define TEMP_CH		0

unsigned int arADmV[ADCH_MAX] = {0,}; // 각 채널에서 읽어드린 AD 값 
unsigned char arADmV_updated[ADCH_MAX] = {FALSE,};
unsigned char ADChannel	= 0;

unsigned int PwOnDelay = 0;




unsigned char ADBuf = 0;



bit bAdConversioned = 0;

bit bPowerOn;



#if defined(__TEMP_1000)
	typedef enum{
		T21 = 2668,	T22 = 2698,	T23 = 2729,	T24 = 2759, T25 = 2790,
		T26 = 2820, T27 = 2850,	T28 = 2880,	T29 = 2910,	T30 = 2940,
		T31 = 2970, T32 = 3000,	T33 = 3029,	T34 = 3059,	T35 = 3089,
		T36 = 3118, T37 = 3148,	T38 = 3177,	T39 = 3206,	T40 = 3235,
		T41 = 3264, T42 = 3293, T43 = 3322, T44 = 3351, T45 = 3380,
		T46 = 3409, T47 = 3437, T48 = 3466, T49 = 3494, T50 = 3523
	} tag_TempValueDef;


#elif defined(__TEMP_100) 
	typedef enum{
		T19 = 2106, T20 = 2136,
		T21 = 2167,	T22 = 2197,	T23 = 2228,	T24 = 2258, T25 = 2288,
		T26 = 2318, T27 = 2349,	T28 = 2379,	T29 = 2410,	T30 = 2439,
		T31 = 2470, T32 = 2500,	T33 = 2530,	T34 = 2560,	T35 = 2590,
		T36 = 2621, T37 = 2650,	T38 = 2680,	T39 = 2710,	T40 = 2740,
		T41 = 2771, T42 = 2800, T43 = 2830, T44 = 2860, T45 = 2890,
		T46 = 2920, T47 = 2950, T48 = 2980, T49 = 3009, T50 = 3040
	} tag_TempValueDef;

#endif



extern void delay(unsigned int val);
extern unsigned char SetADChannel(unsigned char ch);
extern bit	IsADUpdated(unsigned int* arADmV, unsigned char* arADmV_updated, unsigned char ch);
extern void CalcuTemp(unsigned int InputmV, unsigned int* pTemp, unsigned char* pTempSign);
extern bit IsOnOffHeaterProc(unsigned int Temp_mV);
extern unsigned int GetBCDSwitch(void);

#endif