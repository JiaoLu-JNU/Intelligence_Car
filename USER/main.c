#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "pwm.h"
#include "car_control.h"
#include "obstacle_avoid.h"
#include "trace.h"
#include "ps2.h"
#include "remote_ctr.h"
//ALIENTEK 探索者STM32F407开发板 实验9
//PWM输出实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


u16 GEAR=2;
u16 gear_run;
u16 gear_turn;

int main(void)
{
	extern float UltrasonicWave_Distance;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
	TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.    
	TIM4_PWM_Init(200-1,7199-1);
	CAR_Init();
	HCSR04_Init();
	LED_Init();				  //初始化LED端口  
	LED0=0;					    //先点亮红灯
	LED1=0;					    //先点亮红灯
	drive(GEAR);
	TIM_SetCompare1(TIM4, 186);//原90度，此处设置为超声波测距模块的零位
	//stop();

	while(1)
	{           
		Obstacle_avoid(UltrasonicWave_Distance);
		
	}

}
