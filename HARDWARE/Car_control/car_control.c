#include "car_control.h"
#include "trace.h"
#include "obstacle_avoid.h"
#include "remote_ctr.h"
#include "bluetooth_ctr.h"

	/****************************************************************************
	 硬件连接
	   P1_6 接驱动模块ENA	使能端，输入PWM信号调节速度
     P1_7 接驱动模块ENB	使能端，输入PWM信号调节速度

     P3_4 P3_5 接IN1  IN2    当 P3_4=1,P3_5=0; 时左电机正转	 驱动蓝色输出端OUT1 OUT2接左电机 
     P3_4 P3_5 接IN1  IN2    当 P3_4=0,P3_5=1; 时左电机反转                
     P3_6 P3_7 接IN3  IN4	 当 P3_6=1,P3_7=0; 时右电机正转	 驱动蓝色输出端OUT3 OUT4接右电机
     P3_6 P3_7 接IN3  IN4	 当 P3_6=0,P3_7=1; 时右电机反转

     P1_0接四路寻迹模块接口第一路输出信号即中控板上面标记为OUT1
     P1_1接四路寻迹模块接口第二路输出信号即中控板上面标记为OUT2	
     P1_2接四路寻迹模块接口第三路输出信号即中控板上面标记为OUT3
	 P1_3接四路寻迹模块接口第四路输出信号即中控板上面标记为OUT4
	 四路寻迹传感器有信号(白线）为0  没有信号（黑线）为1
	 四路寻迹传感器电源+5V GND 取自于单片机板靠近液晶调节对比度的电源输出接口
																							 
	 关于单片机电源：本店驱动模块内带LDO稳压芯片，当电池输入最低的电压6V时候可以输出稳定的5V
	 分别在针脚标+5 与GND 。这个电源可以作为单片机系统的供电电源。
	****************************************************************************/
	




void CAR_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(GPIOF,GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);//GPIOF9,F10设置高，灯灭

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

//档位转化为速度
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
void drive(u16 gear)	//前进函数
{
		u16 Pulse;
		Pulse=gear_trans(gear);
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_3);   //LED1对应引脚GPIOF.10拉高，灭 等同LED1=1;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_5);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_7); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_12);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_11); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_14);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_13); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		TIM_SetCompare1(TIM3,Pulse);	//修改比较值，修改占空比
		TIM_SetCompare2(TIM3,Pulse);	//修改比较值，修改占空比
		TIM_SetCompare3(TIM3,Pulse);	//修改比较值，修改占空比
		TIM_SetCompare4(TIM3,Pulse);	//修改比较值，修改占空比

}

void reverse(u16 gear)	//倒退函数
{
		u16 Pulse;
		Pulse=gear_trans(gear);
		GPIO_SetBits(GPIOF,GPIO_Pin_1);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
		GPIO_ResetBits(GPIOF,GPIO_Pin_3);   //LED1对应引脚GPIOF.10拉高，灭 等同LED1=1;
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_5);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_7); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_12);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_11); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_14);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_13); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		TIM_SetCompare1(TIM3,Pulse);	//修改比较值，修改占空比
		TIM_SetCompare2(TIM3,Pulse);	//修改比较值，修改占空比
		TIM_SetCompare3(TIM3,Pulse);	//修改比较值，修改占空比
		TIM_SetCompare4(TIM3,Pulse);	//修改比较值，修改占空比

}

void stop(void)	//倒退函数
{
		GPIO_SetBits(GPIOF,GPIO_Pin_1);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_3);   //LED1对应引脚GPIOF.10拉高，灭 等同LED1=1;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_5);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_7); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_12);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_11); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_14);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_SetBits(GPIOF,GPIO_Pin_13); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;

}

void left_move(u16 gear_change)
{
		u16 Pulse;
		Pulse=gear_trans(gear_change);
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_3);   //LED1对应引脚GPIOF.10拉高，灭 等同LED1=1;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_5);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_7); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_12);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_11); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_14);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_13); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		TIM_SetCompare1(TIM3,500);	//修改比较值，修改占空比
		TIM_SetCompare2(TIM3,500);	//修改比较值，修改占空比
		TIM_SetCompare3(TIM3,Pulse);	//修改比较值，修改占空比
		TIM_SetCompare4(TIM3,Pulse);	//修改比较值，修改占空比
}

void right_move(u16 gear_change)
{
		u16 Pulse;
		Pulse=gear_trans(gear_change);
	
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_3);   //LED1对应引脚GPIOF.10拉高，灭 等同LED1=1;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_5);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_7); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_12);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_11); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		GPIO_SetBits(GPIOF,GPIO_Pin_14);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_13); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
	
		TIM_SetCompare1(TIM3,Pulse);	//修改比较值，修改占空比
		TIM_SetCompare2(TIM3,Pulse);	//修改比较值，修改占空比
		TIM_SetCompare3(TIM3,500);	//修改比较值，修改占空比
		TIM_SetCompare4(TIM3,500);	//修改比较值，修改占空比
}


