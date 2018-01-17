#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "usart.h"
#include "light.h"
#include "delay.h"
#include "string.h"
#include "encode.h"


int main(void)
{	
	unsigned char times;
  delay_init();	    	                              //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //中断分组   
	uart_init(115200);                                //串口初始化
	LED_Init();		
  LIGHT_Init();                                   	//停机/待机初始化
	
	
  for(; ;){
		LED0 = LED1 = 0;
		if (WAKEUP){                                    //检测是否是高电平
		  times = 5;
			while(times--){                               //设置延时
				delay_ms(1000);
			}
			if (WAKEUP){
				data_encode("1 1");                         //发送数据，车来了
				delay_ms(200);
				LED0 = LED1 = 1; 
				Enter_StopMode();                           //进入停机模式
			} else {
				Enter_Standby();                            //进入待机模式
			}
		} else {
			data_encode("0 1");                           //车离开了
			delay_ms(2000);
			Enter_Standby();                              //进入待机模式
		}
	}
}

