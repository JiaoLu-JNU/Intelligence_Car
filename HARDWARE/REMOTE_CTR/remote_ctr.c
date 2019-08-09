#include "remote_ctr.h"
#include "trace.h"
#include "obstacle_avoid.h"

extern float UltrasonicWave_Distance;
void Remote_Ctr_Init(void)
{
	
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
				TRACE_Implement();
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
		case 1://REMOTE_CTR_MODE
				
			break;
		case 2://BLUETOOTH_MODE

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










