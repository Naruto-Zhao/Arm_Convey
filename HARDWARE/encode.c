#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"


char carry[50];

/* 数据传送格式编码 */
void data_encode(char *data){
	unsigned char length = strlen(data);
	unsigned char sum = 0x00;
	unsigned char i;
	carry[0] = 0x3C;       //起始位            
  carry[1] = 0x02;       //发送位
	carry[2] = length;     //传送的数据位数
	
  /* 传送的数据 */	
	for (i = 0; i < length; ++i)
		carry[i + 3] = data[i];
	for (i = 0; i < length + 3; ++i)
		sum += carry[i];
	carry[length + 3] = sum;  //校验和
	carry[length + 4] = 0x0D; //结尾
			
	for (i = 0; i <= length + 4; ++i)
		printf("%c", carry[i]);	
}

