#include "obstacle_avoid.h"
#include "delay.h"
#include "usart.h"
#include "car_control.h"
#include "lcd.h"
/*******************************************************************************
* 函 数 名         : pwm_init
* 函数功能		   : IO端口及TIM4初始化函数	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;   //声明一个结构体变量，用来初始化GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//声明一个结构体变量，用来初始化定时器

	TIM_OCInitTypeDef TIM_OCInitStructure;//根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	/* 开启时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOF9复用为定时器14	
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;// PC6
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//复用推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//TIM3定时器初始化
	TIM_TimeBaseInitStructure.TIM_Period = arr; //PWM 频率=72000/(199+1)=36Khz//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;//设置用来作为TIMx时钟频率预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//改变指定管脚的映射	//pC6

	//PWM初始化	  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;

	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	//注意此处初始化时TIM_OC1Init而不是TIM_OCInit，否则会出错。因为固件库的版本不一样。
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR1上的预装载寄存器
	TIM_Cmd(TIM4,ENABLE);//使能或者失能TIMx外设
}



/*可用程序
void Ultrasonic_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//Trig
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//Echo
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

    GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void Timer2_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,   ENABLE);

    TIM_TimeBaseInitStructure.TIM_Prescaler = 71;
    TIM_TimeBaseInitStructure.TIM_Period = 49999;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
		TIM_Cmd(TIM2, DISABLE); 

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}

u32 Distance_Calculate(u32 count)
{
    u32 Distance = 0;
    Distance = (u32)(((float)count / 58) * 100);
    return Distance;
}
*/









/*******************************************************************************
  Driver for HC-SR04 
  测试平台：STM32F103ZET6最小系统
  引脚连接：TRIG--PD13   ECHO--PD11
*******************************************************************************/

#define SONAR_PORT  GPIOG
#define TRIG_PIN    GPIO_Pin_2
#define ECHO_PIN    GPIO_Pin_3

float UltrasonicWave_Distance;


void HCSR04_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    EXTI_InitTypeDef EXTI_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);
	  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF , ENABLE);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
    GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(SONAR_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ECHO_PIN;                   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
		//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉	
    GPIO_Init(SONAR_PORT, &GPIO_InitStructure); 
	
    //GPIO_ResetBits(SONAR_PORT, ECHO_PIN);
		//GPIO_ResetBits(SONAR_PORT, TRIG_PIN);
		
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, GPIO_PinSource3);  // 中断线以及中断初始化配置
    //SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, GPIO_PinSource13);  // 中断线以及中断初始化配置

    EXTI_ClearITPendingBit(EXTI_Line3);
    //EXTI_ClearITPendingBit(EXTI_Line13);
		
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);     
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断4
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//子优先级2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
		NVIC_Init(&NVIC_InitStructure);//配置

    /*
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;                   //  
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;                  // 设置预分频,F=72MHz/72=1MHz,T=1us
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;                 // 设置时钟分频系数,不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);              // 初始化TIME5
		*/
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM_TimeBaseInitStructure.TIM_Prescaler = 8399;
    TIM_TimeBaseInitStructure.TIM_Period = 10000;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
		
		TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
		
		TIM_Cmd(TIM2, DISABLE); 

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级1级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

    TIM_TimeBaseInitStructure.TIM_Prescaler = 83;
    TIM_TimeBaseInitStructure.TIM_Period = 100000;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
		
		TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM5, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
		
		TIM_Cmd(TIM5, ENABLE); 

    TIM_ClearFlag(TIM5, TIM_FLAG_Update);

		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级1级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    //TIM_Cmd(TIM5, DISABLE); 
}

/**
 * 发出测试信号
 */
void HCSR04_StartMeasure(void)
{
    GPIO_SetBits(SONAR_PORT, TRIG_PIN);
    delay_us(20);   //  The width of trig signal must be greater than 10us
    GPIO_ResetBits(SONAR_PORT, TRIG_PIN);
		printf("Test start!");
}

/**
 * 根据公式计算距离 
 * @return distance units:cm 
 */

float HCSR04_GetDistance(u32 count)
{
		float distance;
    // distance = measurement/2/1000*340 = measurement/59 (cm)  measurement-units:us
    distance = (float)count *100 / 58.8;   // measurement-units:us

    return distance;
}


/*
static void ECHO_EXTI_IRQHandler(void)
{
		printf("1111");
	
    if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
        if (GPIO_ReadInputDataBit(SONAR_PORT, ECHO_PIN) != 0) 
				{  // 等待高电平回波
            TIM_Cmd(TIM5, ENABLE); 
						while(GPIO_ReadInputDataBit(SONAR_PORT,ECHO_PIN));//等待低电平
            TIM_Cmd(TIM5, DISABLE);  
            measurement = TIM_GetCounter(TIM5);
            TIM_SetCounter(TIM5, 0); 
						printf("2222");
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line11);
}
*/

void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		}
}

void TIM5_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			HCSR04_StartMeasure();
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		}
}


void EXTI3_IRQHandler(void)
{
    //printf("1111");
		//delay_us(10);		                      //延时10us
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
			{
            TIM_SetCounter(TIM2,0);
						TIM_Cmd(TIM2, ENABLE);                                             //开启时钟
						while(GPIO_ReadInputDataBit(SONAR_PORT,ECHO_PIN));	                 //等待低电平
						TIM_Cmd(TIM2, DISABLE);		  
            UltrasonicWave_Distance = HCSR04_GetDistance(TIM_GetCounter(TIM2));//直接在中断函数中求距离，而不是在中断函数中取出计数值在中断函数外部计算 
						//printf("2222");
						if(UltrasonicWave_Distance>0)
						{
								//static char str[12];
								//sprintf(str,"UltrasonicWave_Distance: %f",UltrasonicWave_Distance);
								//LCD_ShowString(40,260,200,16,16,(u8 *)str);
								printf("distance:%f cm\r\n",UltrasonicWave_Distance);
						}
						
			}
    EXTI_ClearITPendingBit(EXTI_Line3);
}


void Obstacle_avoid(float distance)
{
	extern u16 GEAR;
	if(distance<15)
	{
		stop();
		TIM_SetCompare1(TIM4, 181);//探测仪左转45度
		delay_ms(5000);
		//HCSR04_StartMeasure();	
		//delay_ms(50);
		distance=UltrasonicWave_Distance;
		if(distance<15)
		{
				TIM_SetCompare1(TIM4, 191);//探测仪右转45度
				delay_ms(5000);
				//HCSR04_StartMeasure();	
				//delay_ms(50);
				distance=UltrasonicWave_Distance;
				if(distance<15)
				{
						TIM_SetCompare1(TIM4, 186);//探测仪回零位
						reverse(GEAR);
						delay_ms(5000);
						left_move(GEAR+1);
						delay_ms(3000);
						drive(GEAR);
				}
				else
				{		TIM_SetCompare1(TIM4, 186);//探测仪回零位
						right_move(GEAR+1);
						delay_ms(5000);
						drive(GEAR);
				}
		}
		else
		{
				TIM_SetCompare1(TIM4, 186);//探测仪回零位
				left_move(GEAR+1);
				delay_ms(5000);
				drive(GEAR);
				
		}
		
		
	}
	//drive(GEAR);

}



