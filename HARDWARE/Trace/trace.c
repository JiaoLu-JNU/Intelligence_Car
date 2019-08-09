#include "trace.h"
#include "delay.h"
#include "sys.h"
#include "car_control.h"
#include "led.h"

/*
u8 Tracing_Right=0;//Tracing_Right指右侧传感器状态，默认为0，当右侧检测到黑线时，置1，驱动小车右移
u8 Tracing_Left=0;//Tracing_Right指左侧传感器状态，默认为0，当左侧检测到黑线时，置1，驱动小车左移
void EXTI2_IRQHandler(void)
{	
	extern u16 GEAR;
	GEAR=1;
	drive(GEAR);
	//printf("yizhongduan");
	delay_ms(10);
	LED0=0;					    //先点亮红灯
	LED1=1;					    //先点亮红灯	
	Tracing_Right=1;
	left_move(GEAR+Tracing_Right);
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE3上的中断标志位  
	Tracing_Left=0;
}
 
void EXTI4_IRQHandler(void)
{
	extern u16 GEAR;
	GEAR=1;
	drive(GEAR);
	//printf("erzhongduan");
	delay_ms(10);
	LED0=1;				    //先点亮红灯
	LED1=0;			    //先点亮红灯	
	Tracing_Left=1;
	right_move(GEAR+Tracing_Left);
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE3上的中断标志位  
	Tracing_Right=0;
}


 //最初开关指示灯常亮的原因是选错了IO口，用了PC0和PC2，换成PF2和PF4就好啦，why？
 
void Trace_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //初始化PF9
 
	GPIO_SetBits(GPIOF,GPIO_Pin_2);
	GPIO_SetBits(GPIOF,GPIO_Pin_4);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource2);//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource4);//PE3 连接到中断线3
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
}
*/


void Trace_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //初始化PF9
	
}


u8 TRACE_Scan(u8 mode)
{	 
	static u8 trace_up=1;//按键按松开标志
	if(mode)trace_up=1;  //支持连按		  
	if(trace_up&&(TRACE1==1||TRACE2==1||TRACE3==1||TRACE4==1))
	{
		delay_ms(10);//去抖动 
		trace_up=0;
		if(TRACE1==1)return 1;
		else if(TRACE2==1)return 2;
		else if(TRACE3==1)return 3;
		else if(TRACE4==1)return 4;
	}
	else if(TRACE1==0&&TRACE2==0&&TRACE3==0&&TRACE4==0)trace_up=1;
		return 0;// 无按键按下
}


void TRACE_Implement(void)
{
		extern u16 GEAR;
		u8 trace_scan; 
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
