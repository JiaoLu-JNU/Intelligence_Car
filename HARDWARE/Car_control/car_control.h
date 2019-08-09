#ifndef __CAR_CONT
#define __CAR_CONT
#include "sys.h"


#define TRACE_MODE 0x01;
#define OBSTACLE_AVOID_MODE 0x02;
#define REMOTE_CTR_MODE 0x03;
#define BLUETOOTH_MODE 0x04;
#define STILL_MODE 0x05;


void CAR_Init(void);
void drive(u16 gear);
void reverse(u16 gear);
void stop(void);
void left_move(u16 gear_change);
void right_move(u16 gear_change);
//void Mode_select(u16 mode);


#endif

