#include "obstacle_avoid.h"
#include "delay.h"
#include "usart.h"
#include "car_control.h"
#include "lcd.h"
/*******************************************************************************
* �� �� ��         : pwm_init
* ��������		   : IO�˿ڼ�TIM4��ʼ������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;   //����һ���ṹ�������������ʼ��GPIO

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����һ���ṹ�������������ʼ����ʱ��

	TIM_OCInitTypeDef TIM_OCInitStructure;//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	/* ����ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOF9����Ϊ��ʱ��14	
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;// PC6
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//�����������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//TIM3��ʱ����ʼ��
	TIM_TimeBaseInitStructure.TIM_Period = arr; //PWM Ƶ��=72000/(199+1)=36Khz//�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//�ı�ָ���ܽŵ�ӳ��	//pC6

	//PWM��ʼ��	  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM���ʹ��
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;

	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	//ע��˴���ʼ��ʱTIM_OC1Init������TIM_OCInit������������Ϊ�̼���İ汾��һ����
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM4,ENABLE);//ʹ�ܻ���ʧ��TIMx����
}



/*���ó���
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
  ����ƽ̨��STM32F103ZET6��Сϵͳ
  �������ӣ�TRIG--PD13   ECHO--PD11
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

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
    GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(SONAR_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ECHO_PIN;                   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
		//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����	
    GPIO_Init(SONAR_PORT, &GPIO_InitStructure); 
	
    //GPIO_ResetBits(SONAR_PORT, ECHO_PIN);
		//GPIO_ResetBits(SONAR_PORT, TRIG_PIN);
		
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, GPIO_PinSource3);  // �ж����Լ��жϳ�ʼ������
    //SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, GPIO_PinSource13);  // �ж����Լ��жϳ�ʼ������

    EXTI_ClearITPendingBit(EXTI_Line3);
    //EXTI_ClearITPendingBit(EXTI_Line13);
		
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);     
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�4
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure);//����

    /*
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;                   //  
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;                  // ����Ԥ��Ƶ,F=72MHz/72=1MHz,T=1us
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;                 // ����ʱ�ӷ�Ƶϵ��,����Ƶ
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // ���ϼ���ģʽ
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);              // ��ʼ��TIME5
		*/
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM_TimeBaseInitStructure.TIM_Prescaler = 8399;
    TIM_TimeBaseInitStructure.TIM_Period = 10000;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
		
		TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
		
		TIM_Cmd(TIM2, DISABLE); 

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�1��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

    TIM_TimeBaseInitStructure.TIM_Prescaler = 83;
    TIM_TimeBaseInitStructure.TIM_Period = 100000;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
		
		TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM5, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
		
		TIM_Cmd(TIM5, ENABLE); 

    TIM_ClearFlag(TIM5, TIM_FLAG_Update);

		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�1��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    //TIM_Cmd(TIM5, DISABLE); 
}

/**
 * ���������ź�
 */
void HCSR04_StartMeasure(void)
{
    GPIO_SetBits(SONAR_PORT, TRIG_PIN);
    delay_us(20);   //  The width of trig signal must be greater than 10us
    GPIO_ResetBits(SONAR_PORT, TRIG_PIN);
		printf("Test start!");
}

/**
 * ���ݹ�ʽ������� 
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
				{  // �ȴ��ߵ�ƽ�ز�
            TIM_Cmd(TIM5, ENABLE); 
						while(GPIO_ReadInputDataBit(SONAR_PORT,ECHO_PIN));//�ȴ��͵�ƽ
            TIM_Cmd(TIM5, DISABLE);  
            measurement = TIM_GetCounter(TIM5);
            TIM_SetCounter(TIM5, 0); 
						printf("2222");
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line11);
}
*/

void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}

void TIM5_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			HCSR04_StartMeasure();
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}


void EXTI3_IRQHandler(void)
{
    //printf("1111");
		//delay_us(10);		                      //��ʱ10us
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
			{
            TIM_SetCounter(TIM2,0);
						TIM_Cmd(TIM2, ENABLE);                                             //����ʱ��
						while(GPIO_ReadInputDataBit(SONAR_PORT,ECHO_PIN));	                 //�ȴ��͵�ƽ
						TIM_Cmd(TIM2, DISABLE);		  
            UltrasonicWave_Distance = HCSR04_GetDistance(TIM_GetCounter(TIM2));//ֱ�����жϺ���������룬���������жϺ�����ȡ������ֵ���жϺ����ⲿ���� 
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
		TIM_SetCompare1(TIM4, 181);//̽������ת45��
		delay_ms(5000);
		//HCSR04_StartMeasure();	
		//delay_ms(50);
		distance=UltrasonicWave_Distance;
		if(distance<15)
		{
				TIM_SetCompare1(TIM4, 191);//̽������ת45��
				delay_ms(5000);
				//HCSR04_StartMeasure();	
				//delay_ms(50);
				distance=UltrasonicWave_Distance;
				if(distance<15)
				{
						TIM_SetCompare1(TIM4, 186);//̽���ǻ���λ
						reverse(GEAR);
						delay_ms(5000);
						left_move(GEAR+1);
						delay_ms(3000);
						drive(GEAR);
				}
				else
				{		TIM_SetCompare1(TIM4, 186);//̽���ǻ���λ
						right_move(GEAR+1);
						delay_ms(5000);
						drive(GEAR);
				}
		}
		else
		{
				TIM_SetCompare1(TIM4, 186);//̽���ǻ���λ
				left_move(GEAR+1);
				delay_ms(5000);
				drive(GEAR);
				
		}
		
		
	}
	//drive(GEAR);

}



