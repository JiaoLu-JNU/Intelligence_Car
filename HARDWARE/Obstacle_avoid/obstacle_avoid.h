#ifndef _OBSTACLE_AVOID
#define _OBSTACLE_AVOID
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
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


