#include "trace.h"
#include "delay.h"
#include "sys.h"
#include "car_control.h"
#include "led.h"

/*
u8 Tracing_Right=0;//Tracing_Rightָ�Ҳഫ����״̬��Ĭ��Ϊ0�����Ҳ��⵽����ʱ����1������С������
u8 Tracing_Left=0;//Tracing_Rightָ��ഫ����״̬��Ĭ��Ϊ0��������⵽����ʱ����1������С������
void EXTI2_IRQHandler(void)
{	
	extern u16 GEAR;
	GEAR=1;
	drive(GEAR);
	//printf("yizhongduan");
	delay_ms(10);
	LED0=0;					    //�ȵ������
	LED1=1;					    //�ȵ������	
	Tracing_Right=1;
	left_move(GEAR+Tracing_Right);
	EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE3�ϵ��жϱ�־λ  
	Tracing_Left=0;
}
 
void EXTI4_IRQHandler(void)
{
	extern u16 GEAR;
	GEAR=1;
	drive(GEAR);
	//printf("erzhongduan");
	delay_ms(10);
	LED0=1;				    //�ȵ������
	LED1=0;			    //�ȵ������	
	Tracing_Left=1;
	right_move(GEAR+Tracing_Left);
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE3�ϵ��жϱ�־λ  
	Tracing_Right=0;
}


 //�������ָʾ�Ƴ�����ԭ����ѡ����IO�ڣ�����PC0��PC2������PF2��PF4�ͺ�����why��
 
void Trace_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       //����
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //��ʼ��PF9
 
	GPIO_SetBits(GPIOF,GPIO_Pin_2);
	GPIO_SetBits(GPIOF,GPIO_Pin_4);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource2);//PE2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource4);//PE3 ���ӵ��ж���3
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
}
*/


void Trace_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;       //����
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //��ʼ��PF9
	
}


u8 TRACE_Scan(u8 mode)
{	 
	static u8 trace_up=1;//�������ɿ���־
	if(mode)trace_up=1;  //֧������		  
	if(trace_up&&(TRACE1==1||TRACE2==1||TRACE3==1||TRACE4==1))
	{
		delay_ms(10);//ȥ���� 
		trace_up=0;
		if(TRACE1==1)return 1;
		else if(TRACE2==1)return 2;
		else if(TRACE3==1)return 3;
		else if(TRACE4==1)return 4;
	}
	else if(TRACE1==0&&TRACE2==0&&TRACE3==0&&TRACE4==0)trace_up=1;
		return 0;// �ް�������
}


void TRACE_Implement(void)
{
		extern u16 GEAR;
		u8 trace_scan; 
		trace_scan=TRACE_Scan(0);		//�õ���ֵ
	  if(trace_scan)
		{						   
			switch(trace_scan)
			{				 
				case TRACE1_SCAN:	//���Ʒ�����
					left_move(GEAR+2);
					break;
				case TRACE2_SCAN:	//����LED0��ת
					right_move(GEAR+2);
					LED0=!LED0;
					break;
				case TRACE3_SCAN:	//����LED1��ת	 
					left_move(GEAR);
					LED1=!LED1;
					break;
				case TRACE4_SCAN:	//ͬʱ����LED0,LED1��ת 
					right_move(GEAR);
					LED0=!LED0;
					LED1=!LED1;
					break;
			}
		}else delay_ms(10); 
}
