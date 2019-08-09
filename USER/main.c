#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "pwm.h"
#include "car_control.h"
#include "obstacle_avoid.h"
#include "trace.h"

//ALIENTEK ̽����STM32F407������ ʵ��9
//PWM���ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


//u16 GEAR=2;
u16 gear_run;
u16 gear_turn;

/*
int main(void)
{ 
	//u16 led0pwmval=0;    
	//u8 dir=1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
 	//TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.     
	TIM4_PWM_Init(200-1,7199-1);

   while(1) //ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	{
		delay_ms(100);
		TIM_SetCompare1(TIM4, 186);//ԭ90�ȣ��˴�����Ϊ���������ģ�����λ
		delay_ms(100);
		TIM_SetCompare1(TIM4, 191);//��45��
		delay_ms(100);
		TIM_SetCompare1(TIM4, 181);//��45��
		delay_ms(100);
		

	}
}
*/


int main(void)
{ 
//	u8 lcd_id[12];				//���LCD ID�ַ���
	
	extern float UltrasonicWave_Distance;
	u8 trace_scan; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.    
	//TIM4_PWM_Init(200-1,7199-1);
	CAR_Init();
	//HCSR04_Init();
	Trace_Init();
	LED_Init();				  //��ʼ��LED�˿�  
	LED0=0;					    //�ȵ������
	LED1=0;					    //�ȵ������
	drive(GEAR);
	TIM_SetCompare1(TIM4, 186);//ԭ90�ȣ��˴�����Ϊ���������ģ�����λ
	//stop();
	
	//LCD_Init();           //��ʼ��LCD FSMC�ӿ�
	//POINT_COLOR=RED;      //������ɫ����ɫ
	//sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣	
	
	while(1) //ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	{           
		//HCSR04_StartMeasure();		
		
		//LCD_ShowString(30,110,200,16,16,lcd_id);		//��ʾLCD ID	      				
 		//delay_ms(10);	
		//Obstacle_avoid(UltrasonicWave_Distance);
		//printf("ok");
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

}

/*
int main(void)
{
    u32 Distance = 0;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
		delay_init(168);  //��ʼ����ʱ����
		uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
		//TIM3_PWM_Init(500-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.     
		//TIM4_PWM_Init(200-1,7199-1);
		CAR_Init();
		//drive();
		stop();
		HCSR04_Init();
		LED_Init();		            //LED�˿ڳ�ʼ��
    
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
		uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
		delay_init(168);  //��ʼ����ʱ����
    LED_Init();
    Ultrasonic_Config();
    Timer2_Config();
    GPIO_ResetBits(GPIOF, GPIO_Pin_9);//����LED
    printf("Test start!");
    while(1)
    {
        GPIO_ResetBits(GPIOG, GPIO_Pin_2);//Ԥ������Trig����
        GPIO_SetBits(GPIOG, GPIO_Pin_2);
				printf("0001!");
        delay_us(10);
				printf("0002!");
        GPIO_ResetBits(GPIOG, GPIO_Pin_2);//����10us������
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
