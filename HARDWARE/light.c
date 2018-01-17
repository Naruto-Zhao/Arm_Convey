#include "stm32f10x.h"
#include "light.h"
#include "led.h"
#include "delay.h"
#include "encode.h"


/* 进入停机状态 */
void Enter_StopMode(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	   //使能PWR外设时钟
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}


/* 待机模式 */
void Standby(void){
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	   //使能PWR外设时钟
	PWR_WakeUpPinCmd(ENABLE);                              //使能唤醒管脚功能
	PWR_EnterSTANDBYMode();	  
}

/* 进入待机状态 */
void Enter_Standby(void){
	RCC_APB2PeriphResetCmd(0x01FC, DISABLE);	               //复位所有IO口
	Standby();
}


u8 Check_WKUP(void){                //端口扫描
	if (!WAKEUP) {
		delay_ms(100);
		if(!WAKEUP){
      return 1;			
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}  


/* 中断函数 */
void EXTI0_IRQHandler(void)
{ 		    		    				     		   
  EXTI_ClearITPendingBit(EXTI_Line0);	
	if(Check_WKUP()){
		SystemInit();                     //系统时钟初始化
	}  
} 


/* 中断初始化 */
void LIGHT_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;  		  
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOA和复用功能时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	                      //PA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                   //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);               	          //初始化IO
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	    //中断线0连接GPIOA.0

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	                    //设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			        //设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;         //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	                                // 初始化外部中断

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;                //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;       //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;              //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
	
	if (Check_WKUP())  Standby();                                   //一开始进入待机模式
}



