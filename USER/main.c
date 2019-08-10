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
//ALIENTEK ̽����STM32F407������ ʵ��9
//PWM���ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


u16 GEAR=2;
u16 gear_run;
u16 gear_turn;

int main(void)
{
	extern float UltrasonicWave_Distance;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.    
	TIM4_PWM_Init(200-1,7199-1);
	CAR_Init();
	HCSR04_Init();
	LED_Init();				  //��ʼ��LED�˿�  
	LED0=0;					    //�ȵ������
	LED1=0;					    //�ȵ������
	drive(GEAR);
	TIM_SetCompare1(TIM4, 186);//ԭ90�ȣ��˴�����Ϊ���������ģ�����λ
	//stop();

	while(1)
	{           
		Obstacle_avoid(UltrasonicWave_Distance);
		
	}

}
