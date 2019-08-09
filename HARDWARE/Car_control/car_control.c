#include "car_control.h"
#include "trace.h"
#include "obstacle_avoid.h"
#include "remote_ctr.h"
#include "bluetooth_ctr.h"

	/****************************************************************************
	 Ӳ������
	   P1_6 ������ģ��ENA	ʹ�ܶˣ�����PWM�źŵ����ٶ�
     P1_7 ������ģ��ENB	ʹ�ܶˣ�����PWM�źŵ����ٶ�

     P3_4 P3_5 ��IN1  IN2    �� P3_4=1,P3_5=0; ʱ������ת	 ������ɫ�����OUT1 OUT2������ 
     P3_4 P3_5 ��IN1  IN2    �� P3_4=0,P3_5=1; ʱ������ת                
     P3_6 P3_7 ��IN3  IN4	 �� P3_6=1,P3_7=0; ʱ�ҵ����ת	 ������ɫ�����OUT3 OUT4���ҵ��
     P3_6 P3_7 ��IN3  IN4	 �� P3_6=0,P3_7=1; ʱ�ҵ����ת

     P1_0����·Ѱ��ģ��ӿڵ�һ·����źż��пذ�������ΪOUT1
     P1_1����·Ѱ��ģ��ӿڵڶ�·����źż��пذ�������ΪOUT2	
     P1_2����·Ѱ��ģ��ӿڵ���·����źż��пذ�������ΪOUT3
	 P1_3����·Ѱ��ģ��ӿڵ���·����źż��пذ�������ΪOUT4
	 ��·Ѱ�����������ź�(���ߣ�Ϊ0  û���źţ����ߣ�Ϊ1
	 ��·Ѱ����������Դ+5V GND ȡ���ڵ�Ƭ���忿��Һ�����ڶԱȶȵĵ�Դ����ӿ�
																							 
	 ���ڵ�Ƭ����Դ����������ģ���ڴ�LDO��ѹоƬ�������������͵ĵ�ѹ6Vʱ���������ȶ���5V
	 �ֱ�����ű�+5 ��GND �������Դ������Ϊ��Ƭ��ϵͳ�Ĺ����Դ��
	****************************************************************************/
	




void CAR_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;//LED0��LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_SetBits(GPIOF,GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);//GPIOF9,F10���øߣ�����

}
/*
void Mode_select(u16 mode)
{
	CAR_Init();
	switch(mode)
	{
		case 0x01://TRACE_MODE
			Trace_Init();	
			
			break;
		case 0x02://OBSTACLE_AVOID_MODE
			TIM4_PWM_Init(200-1,7199-1);
			//HCSR04_Init();
			break;
		case 0x03://REMOTE_CTR_MODE
			Remote_Ctr_Init();
			break;
		case 0x04://BLUETOOTH_MODE
			Bluetooth_Ctr_Init();
			break;
		case 0x05://STILL_MODE
			stop();
			break;

	}
}
*/

//��λת��Ϊ�ٶ�
u16 gear_trans(u16 gear)
{
	u16 Pulse=0;
	switch(gear)
	{
		case 0x00:
			Pulse=500;
			break;
		case 0x01:
			Pulse=350;
			break;
		case 0x02:
			Pulse=300;
			break;
		case 0x03:
			Pulse=200;
			break;
		case 0x04:
			Pulse=100;
			break;
		default:
			break;
	}
	return Pulse;

}

/************************************************************************/
void drive(u16 gear)	//ǰ������
{
		u16 Pulse;
		Pulse=gear_trans(gear);
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);  //LED0��Ӧ����GPIOF.9���ͣ���  ��ͬLED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_3);   //LED1��Ӧ����GPIOF.10���ߣ��� ��ͬLED1=1;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_5);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_7); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_12);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_11); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_14);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_13); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		TIM_SetCompare1(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare2(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare3(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare4(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�

}

void reverse(u16 gear)	//���˺���
{
		u16 Pulse;
		Pulse=gear_trans(gear);
		GPIO_SetBits(GPIOF,GPIO_Pin_1);  //LED0��Ӧ����GPIOF.9���ͣ���  ��ͬLED0=0;
		GPIO_ResetBits(GPIOF,GPIO_Pin_3);   //LED1��Ӧ����GPIOF.10���ߣ��� ��ͬLED1=1;
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_5);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_7); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_12);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_11); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_14);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_13); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		TIM_SetCompare1(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare2(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare3(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare4(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�

}

void stop(void)	//���˺���
{
		GPIO_SetBits(GPIOF,GPIO_Pin_1);  //LED0��Ӧ����GPIOF.9���ͣ���  ��ͬLED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_3);   //LED1��Ӧ����GPIOF.10���ߣ��� ��ͬLED1=1;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_5);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_7); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_12);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_11); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_14);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_13); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;

}

void left_move(u16 gear_change)
{
		u16 Pulse;
		Pulse=gear_trans(gear_change);
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);  //LED0��Ӧ����GPIOF.9���ͣ���  ��ͬLED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_3);   //LED1��Ӧ����GPIOF.10���ߣ��� ��ͬLED1=1;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_5);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_7); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_12);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_11); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_14);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_13); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		TIM_SetCompare1(TIM3,500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare2(TIM3,500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare3(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare4(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
}

void right_move(u16 gear_change)
{
		u16 Pulse;
		Pulse=gear_trans(gear_change);
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);  //LED0��Ӧ����GPIOF.9���ͣ���  ��ͬLED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_3);   //LED1��Ӧ����GPIOF.10���ߣ��� ��ͬLED1=1;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_5);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_7); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_12);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_11); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_14);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_13); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
	
		TIM_SetCompare1(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare2(TIM3,Pulse);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare3(TIM3,500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
		TIM_SetCompare4(TIM3,500);	//�޸ıȽ�ֵ���޸�ռ�ձ�
}


