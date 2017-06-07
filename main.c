
//////////////////////////////////////
/*
 * 아름 삼공사 프로젝트
 * 선박용 온도 입력, 릴레이 출력 보드
 * 2016-07-25 프로젝트 시작

 * 작성자 : 한동훈

 */
//////////////////////////////////////



#include <pic.h>

#include "main.h"
#include "16F716.h"






// if val = 1 이면, 35 us
void delay(unsigned int val)
{
    unsigned int i;

    for (i = 0; i < val; i++);
}


unsigned char SetADChannel(unsigned char ch)
{
    switch (ch)
    {
    case 0:
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS0 = 0;
        break;
    case 1:
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS0 = 1;
        break;
    case 2:
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS1 = 1;
        ADCON0bits.CHS0 = 0;
        break;
    case 3:
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS1 = 1;
        ADCON0bits.CHS0 = 1;
        break;
    default:
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS0 = 0;
        break;
    }

    return ch;
}

bit	IsADUpdated(unsigned int* arADmV, unsigned char* arADmV_updated, unsigned char ch)
{
    static unsigned char SumCnt = 0;
    static unsigned	long ADSum = 0;
    unsigned long ADmV;
    static bit ret;

    ret = FALSE;

    ADSum = ADSum + (unsigned long)ADBuf; // 8비트 AD
    SumCnt++;

    if (SumCnt >= 10)
    {
        ADmV = ((ADSum * 196) / (SumCnt * 10)); // 8 비트 5V 기준
      	ADmV = ADmV + 25; // 보정을 위하여추가 하였다.	

        arADmV[ch] = (unsigned int)ADmV;
        arADmV_updated[ch] = TRUE;

        ADSum = 0;
        SumCnt = 0;

        ret = TRUE;
    }

    return ret;
}



// Temp_mV 값이 1,800 기준 값에 따른 SET LED 상태 표현하기

void Chk1800mvSetTemp(unsigned int Temp_mV)
{

    if (Temp_mV < T32)
    {
        PIN_TEMP_SET_LED = OFF_LED;
    }
    else
    {
        PIN_TEMP_SET_LED = ON_LED;
    }

}


// Temp_mV 값이 4000 이상이거나 50 이하이면 '고장 LED' 에 표현하기

bit IsError(unsigned int Temp_mV)
{

    PIN_ERR_ALARM_LED_short = OFF_LED;
	PIN_ERR_ALARM_LED_open = OFF_LED;

	// 쇼트 (short)
	if (Temp_mV <= 100)
	{
		PIN_ERR_ALARM_LED_short = ON_LED;

		return TRUE;
	}

	
	// 단선 (open)
    if (Temp_mV >= 4900)	
    {
        PIN_ERR_ALARM_LED_open = ON_LED;
		
		return TRUE;
    }
	
	

	return FALSE;

}






		
// BCD 스위치 값을 입력 받아서  
// OFF 온도 값과 ON 온도 값을 출력한다.  
bit GetOnOffTemp(unsigned int nBCD, 
			tag_TempValueDef *OnTemp, 
			tag_TempValueDef *OffTemp)
{
	switch (nBCD)
	{
	case	0:
		*OnTemp	 = T25;
		*OffTemp = T30;
		break;
	case	1:
		*OnTemp	 = T26;
		*OffTemp = T31;
		break;
	case	2:
		*OnTemp	 = T27;
		*OffTemp = T32;
		break;
	case	3:
		*OnTemp	 = T28;
		*OffTemp = T33;
		break;
	case	4:
		*OnTemp	 = T29;
		*OffTemp = T34;
		break;
	case	5:
		*OnTemp	 = T30;
		*OffTemp = T35;
		break;
	case	6:
		*OnTemp	 = T31;
		*OffTemp = T36;
		break;
	case	7:
		*OnTemp	 = T32;
		*OffTemp = T37;
		break;
	case	8:
		*OnTemp	 = T33;
		*OffTemp = T38;
		break;
	case	9:
		*OnTemp	 = T34;
		*OffTemp = T39;
		break;
	default:			
		return	FALSE;
	}
	
	return TRUE;
}


bit OnHeaterWhenPwOn(unsigned int Temp_mV, tag_TempValueDef HighOffTemp)
{

	if (Temp_mV < (HighOffTemp - 20)) // 헌팅 때문에 -20mv(약 1도) 하여 주었다. 
	{
		return ON_RELAY;
	}

	return OFF_RELAY;	
	
}


// Temp_mV 값이 기준값에 도달하면 히터 출력 OFF
// 기준값보다 5도 낮은 값 이하이면 , 히터 출력 ON

bit OnOffHeater(unsigned int Temp_mV, 
				tag_TempValueDef OnTemp,
				tag_TempValueDef OffTemp)
{
	
	if (Temp_mV < OnTemp)
	{
		return ON_RELAY;
	}

	if (Temp_mV > OffTemp)
	{
		return OFF_RELAY;
	}	

	return PIN_HEATER_OUT; // 이조 저도 아니면 기존 출력값 유지 
}


bit IsOnOffHeaterProc(unsigned int Temp_mV)
{
    unsigned int nBCD;
	static bit bError;
	tag_TempValueDef	LowOnTemp, HighOffTemp;	
	

    bError = IsError(Temp_mV);
	if (bError)
	{
		return OFF_RELAY;
	}

	// 기준값을 BCD SW 값에 따라서 변경하기
	nBCD = GetBCDSwitch();	
	GetOnOffTemp(nBCD, &LowOnTemp, &HighOffTemp);	
	
			
	if (bPowerOn) // 전원 껐다 켰을 때  
	{
		bPowerOn = FALSE;		

		return OnHeaterWhenPwOn(Temp_mV, HighOffTemp);
	}
	else
	{
		return OnOffHeater(Temp_mV, LowOnTemp, HighOffTemp);	
	}
	

	return PIN_HEATER_OUT;
	
}


unsigned int GetBCDSwitch(void)
{
    unsigned int nSetNumber = 0;
    unsigned char bit0 = 0, bit1 = 0, bit2 = 0, bit3 = 0;

    bit0 = ~PIN_SW_BCD_1;
    bit1 = ~PIN_SW_BCD_2;
    bit2 = ~PIN_SW_BCD_3;
    bit3 = ~PIN_SW_BCD_4;

    bit0 = (bit0 << 0);
    bit1 = (bit1 << 1);
    bit2 = (bit2 << 2);
    bit3 = (bit3 << 3);

    nSetNumber = (bit0 | bit1 | bit2 | bit3);

    return nSetNumber;
}



void AD_PROC(void)
{
    if (bAdConversioned)
    {
        if (IsADUpdated(arADmV, arADmV_updated, ADChannel))
        {
            ADChannel++;
            if (ADChannel > 1) ADChannel = 0;
            SetADChannel(ADChannel);
        }

        bAdConversioned = FALSE;
        delay(1);
        ADCON0bits.GO_DONE = 1; // AD컨버젼 시작해라 !!!
    }
}


void OnOffHeaterLED(void)
{
	if (PIN_HEATER_OUT == ON_RELAY)
	{
		PIN_HEATER_LED	= ON_LED;	
	}
	else
	{
		PIN_HEATER_LED	= OFF_LED;	
	}
}



void main(void)
{

    di();
    InitCPU();
    InitADCON1_REGISTER();
    InitPort();
    InitTMR0_REGISTER();
    InitAD();
    ei();


    delay(1);
    ADCON0bits.GO_DONE = 1;


	PwOnDelay = 0;
	PIN_HEATER_OUT	= OFF_RELAY;	

	bPowerOn = TRUE;
	
	
	
    while (1)
    {
        CLRWDT();

		

		// 전원을 껐다 키면 순간적으로 히터 릴레이 출력이 ON되는
		// 문제를 해결하기 위하여 딜레이 시간을 넣어 주었다. 
		if (PwOnDelay > 50) 
		{
        	AD_PROC();
		}
		
				
        if (arADmV_updated[TEMP_CH])
        {
            arADmV_updated[TEMP_CH] = FALSE;

			Chk1800mvSetTemp(arADmV[TEMP_CH]);

			PIN_HEATER_OUT = IsOnOffHeaterProc(arADmV[TEMP_CH]);
        }	

		OnOffHeaterLED();

    }
}




void interrupt isr(void)
{
    if (T0IF && T0IE)
    {
        T0IF = 0;
        TMR0 = TMR0_MSEC;

        if (PwOnDelay < 0xffff) PwOnDelay++;

    }

    if (ADIE && ADIF)
    {
        ADIF = 0;
        if (bAdConversioned == FALSE)
        {
            bAdConversioned = TRUE;
            ADBuf = ADRES; // 8비트
        }
        ADCON0bits.GO_DONE = 0;
    }
}
