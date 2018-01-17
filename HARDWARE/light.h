#ifndef __LIGHT_H
#define __LIGHT_H
#include "stm32f10x.h"


#define WAKEUP PAin(0)

void Enter_StopMode(void);
void Standby(void);
void Enter_Standby(void);
void LIGHT_Init(void);
u8 Check_WKUP(void);

#endif
