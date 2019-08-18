#include "remote_ctr.h"
#include "trace.h"
#include "obstacle_avoid.h"
#include "ps2.h"
#include "car_control.h"
#include "usart.h"	
extern float UltrasonicWave_Distance;
void Remote_Ctr_Init(void)
{
	
}


void Remote_Ctr(void)
{
	static int pss_data[2];
	static int pulse_run;
	static int turn_rate;
	pss_data[0]=PS2_AnologData(PSS_LY);
	pss_data[1]=PS2_AnologData(PSS_RX);
	if(pss_data[0]<64)  //高速前进，利用一侧减速进行转弯
	{
		pulse_run=(int)(3.91*pss_data[0]);
		drive_pulse(pulse_run);
		if(pss_data[1]<128)
		{
			turn_rate=(int)(1-pss_data[1]/128);
			turn_pulse(pulse_run*(1+turn_rate),pulse_run);  //左侧减速
		
		}
		else
		{
			turn_rate=(int)((pss_data[1]+1)/128-1);
			turn_pulse(pulse_run,pulse_run*(1+turn_rate));  //右侧减速
		}
	}
	else if(pss_data[0]<128)  //低速前进，利用一侧加速进行转弯
	{
		pulse_run=(int)(3.91*pss_data[0]);
		drive_pulse(pulse_run);
		if(pss_data[1]<128)		
		{
			turn_rate=(int)(1-pss_data[1]/128);
			turn_pulse(pulse_run,pulse_run*(1-turn_rate));		//右侧加速
		
		}
		else
		{
			turn_rate=(int)((pss_data[1]+1)/128-1);
			turn_pulse(pulse_run*(1-turn_rate),pulse_run);		//左侧加速
		}
	}
		
	
	
	
	else if(pss_data[0]<192)		//低速后退，利用一侧加速进行转弯
	{
		pulse_run=(int)(500-3.91*(pss_data[0]-128));
		reverse_pulse(pulse_run);
		/*if(pss_data[1]<128)
		{
			turn_rate=(int)(1-pss_data[1]/128);
			turn_pulse(pulse_run,pulse_run*(1-turn_rate));  //右侧加速
		
		}
		else
		{
			turn_rate=(int)((pss_data[1]+1)/128-1);
			turn_pulse(pulse_run*(1-turn_rate),pulse_run);  //左侧加速
		}
		*/
	}
	else  //高速后退，利用一侧减速进行转弯
	{
		pulse_run=(int)(500-3.91*(pss_data[0]-128));
		reverse_pulse(pulse_run);
		/*if(pss_data[1]<128)		
		{
			turn_rate=(int)(1-pss_data[1]/128);
			turn_pulse(pulse_run*(1+turn_rate),pulse_run);		//左侧减速
		
		}
		else
		{
			turn_rate=(int)((pss_data[1]+1)/128-1);
			turn_pulse(pulse_run,pulse_run*(1+turn_rate));		//右侧减速
		}
		*/
	}
}


u8 Mode_select(u8 key)
{
	switch(key)
	{
		case 13://REMOTE_CTR_MODE

			break;
		case 15://BLUETOOTH_MODE

			break;
		case 16://TRACE_MODE
				Trace_Init();	
				//TRACE_Implement();
			break;
		case 14://OBSTACLE_AVOID_MODE
				TIM4_PWM_Init(200-1,7199-1);
				HCSR04_Init();
				//Obstacle_avoid(UltrasonicWave_Distance);
			break;
		default:
			break;
		
	}
	return 0;
}

void Mode_run(u8 mode)
{
	switch(mode)
	{
		case 13://REMOTE_CTR_MODE
				Remote_Ctr();
			break;
		case 15://BLUETOOTH_MODE

			break;
		case 16://TRACE_MODE
				TRACE_Implement();
			break;
		case 14://OBSTACLE_AVOID_MODE
				Obstacle_avoid(UltrasonicWave_Distance);
			break;
		default:
			break;
	}
}

/*
void Remote_Ctr_Gear(u8 key)
{
	extern u16 gear_run;
	extern u16 gear_turn;
	if(key==5)
	{
		gear_run+=1;
		if(gear_run>4)
			gear_run=4;
		
	}
	if(key==7)
	{
		gear_run-=1;
		if(gear_run<0)
			gear_run=0;
		
	}
	if(key==8)
	{
		gear_turn-=1;
		if(gear_turn<0)
			gear_turn=0;
		
	}
	if(key==6)
	{
		gear_turn+=1;
		if(gear_turn>4)
			gear_turn=4;
		
	}

}
*/










