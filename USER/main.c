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
/*
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
*/



int main(void)
{								  
	u8 key;  
	u8 mode;
	int Start_flag=0;
	//Stm32_Clock_Init(336,8,2,7); //系统时钟设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);	     //延时初始化
	uart_init(115200);  //串口1初始化 
	LED_Init();
	PS2_Init();			 //驱动端口初始化
	PS2_SetInit();		 //配配置初始化,配置“红绿灯模式”，并选择是否可以修改
	                     //开启震动模式
	TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.    
	TIM4_PWM_Init(200-1,7199-1);
	CAR_Init();
	//HCSR04_Init();
	LED_Init();				  //初始化LED端口  
	LED0=0;					    //先点亮红灯
	LED1=0;					    //先点亮红灯
	TIM_SetCompare1(TIM4, 186);//原90度，此处设置为超声波测距模块的零位
	stop();
	//drive_pulse(200);
	while(1)
	{
		LED0 =! LED0;
		key=PS2_DataKey();
		if(key==13|key==14|key==15|key==16)                   //模式选择功能有按键按下
    	{
			printf("  \r\n   %d  is  light  \r\n",Data[1]);//ID
			printf("  \r\n   %d  is  pressed  \r\n",key);
			Mode_select(key);
			mode=key;
			Start_flag=0;
			if(key == 11)
			{
				PS2_Vibration(0xFF,0x00);  //发出震动后必须有延时  delay_ms(1000);
				delay_ms(500);
			}
			else if(key == 12)
			{
				PS2_Vibration(0x00,0xFF);  //发出震动后必须有延时  delay_ms(1000);
				delay_ms(500);
			}
			else
				 PS2_Vibration(0x00,0x00); 
    	}
		//按下start键，标志位置1，进入执行状态
		if(key==4)   
		{
			Start_flag=1;
		}
		if(Start_flag==1)
		{
			Mode_run(mode);
		}
		printf(" %5d %5d %5d %5d\r\n",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY),
		                              PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY) );
		delay_ms(50);
		
	}	 

}
