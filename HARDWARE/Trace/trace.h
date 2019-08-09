#ifndef _TRACE_H
#define _TRACE_H
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
/*下面的方式是通过直接操作库函数方式读取IO*/
#define TRACE1 	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0) //左
#define TRACE2 	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)	//右
#define TRACE3 	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4) //中左
#define TRACE4 	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)	//中右

#define TRACE1_SCAN 	1
#define TRACE2_SCAN	  2
#define TRACE3_SCAN	  3
#define TRACE4_SCAN   4

void Trace_Init(void);
u8 TRACE_Scan(u8);  		//按键扫描函数	
void TRACE_Implement(void);
#endif

