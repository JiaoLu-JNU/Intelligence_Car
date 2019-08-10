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
	//u16 led0pwmval=0;    
	//u8 dir=1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
 	//TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.     
	TIM4_PWM_Init(200-1,7199-1);

   while(1) //实现比较值从0-300递增，到300后从300-0递减，循环
	{
		delay_ms(100);
		TIM_SetCompare1(TIM4, 186);//原90度，此处设置为超声波测距模块的零位
		delay_ms(100);
		TIM_SetCompare1(TIM4, 191);//右45度
		delay_ms(100);
		TIM_SetCompare1(TIM4, 181);//左45度
		delay_ms(100);
		

	}
}
*/

/* 
int main(void)
{ 
//	u8 lcd_id[12];				//存放LCD ID字符串
	
	extern float UltrasonicWave_Distance;
	u8 trace_scan; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
	TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.    
	//TIM4_PWM_Init(200-1,7199-1);
	CAR_Init();
	//HCSR04_Init();
	Trace_Init();
	LED_Init();				  //初始化LED端口  
	LED0=0;					    //先点亮红灯
	LED1=0;					    //先点亮红灯
	drive(GEAR);
	TIM_SetCompare1(TIM4, 186);//原90度，此处设置为超声波测距模块的零位
	//stop();
	
	//LCD_Init();           //初始化LCD FSMC接口
	//POINT_COLOR=RED;      //画笔颜色：红色
	//sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。	
	
	while(1) //实现比较值从0-300递增，到300后从300-0递减，循环
	{           
		//HCSR04_StartMeasure();		
		
		//LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      				
 		//delay_ms(10);	
		//Obstacle_avoid(UltrasonicWave_Distance);
		//printf("ok");
		trace_scan=TRACE_Scan(0);		//得到键值
	  if(trace_scan)
		{						   
			switch(trace_scan)
			{				 
				case TRACE1_SCAN:	//控制蜂鸣器
					left_move(GEAR+2);
					break;
				case TRACE2_SCAN:	//控制LED0翻转
					right_move(GEAR+2);
					LED0=!LED0;
					break;
				case TRACE3_SCAN:	//控制LED1翻转	 
					left_move(GEAR);
					LED1=!LED1;
					break;
				case TRACE4_SCAN:	//同时控制LED0,LED1翻转 
					right_move(GEAR);
					LED0=!LED0;
					LED1=!LED1;
					break;
			}
		}else delay_ms(10); 
	}

}
*/
/*
int main(void)
{
    u32 Distance = 0;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
		delay_init(168);  //初始化延时函数
		uart_init(115200);//初始化串口波特率为115200
		//TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.     
		//TIM4_PWM_Init(200-1,7199-1);
		CAR_Init();
		//drive();
		stop();
		HCSR04_Init();
		LED_Init();		            //LED端口初始化
    
    while(1)
    {
			
				HCSR04_StartMeasure();
				LED0=!LED0;
        delay_ms(1000);
    }
}
*/
/*
int main(void)
{
    u32 count = 0;
    u32 Distance = 0;
		uart_init(115200);//初始化串口波特率为115200
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
		delay_init(168);  //初始化延时函数
    LED_Init();
    Ultrasonic_Config();
    Timer2_Config();
    GPIO_ResetBits(GPIOF, GPIO_Pin_9);//点亮LED
    printf("Test start!");
    while(1)
    {
        GPIO_ResetBits(GPIOG, GPIO_Pin_2);//预先拉低Trig引脚
        GPIO_SetBits(GPIOG, GPIO_Pin_2);
				printf("0001!");
        delay_us(10);
				printf("0002!");
        GPIO_ResetBits(GPIOG, GPIO_Pin_2);//发出10us的脉冲
			printf("0003!");
        TIM2->CNT = 0;
			printf("0004!");
        while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3) == 0);
			printf("0005!");
        TIM_Cmd(TIM2, ENABLE);
        while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3) == 1);
			printf("0006!");
        TIM_Cmd(TIM2, DISABLE);
        count = TIM2->CNT;
        Distance = Distance_Calculate(count);
        printf("Distance = %d.", Distance / 100);
        printf("%d cm\r\n", Distance % 100);
        delay_ms(500);
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

