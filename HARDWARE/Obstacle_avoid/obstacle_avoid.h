#ifndef _OBSTACLE_AVOID
#define _OBSTACLE_AVOID
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/6/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void TIM4_PWM_Init(u16 arr,u16 psc);
void HCSR04_Init(void);
void HCSR04_StartMeasure(void);
float HCSR04_GetDistance(u32 count);
static void ECHO_EXTI_IRQHandler(void);
void EXTI1_IRQHandler(void);
void Obstacle_avoid(float distance);
/*
void Ultrasonic_Config(void);
void Timer2_Config(void);
u32 Distance_Calculate(u32 count);
*/
#endif


