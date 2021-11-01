#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h" 
#include "dma.h"
#include "key.h" 
//ALIENTEK 阿波罗STM32F429开发板 实验23
//DMA 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 



u8 SendBuff[SEND_BUF_SIZE];	    //发送数据缓冲区
u8 ReceBuff[RECV_BYF_SIZE];	    //接收数据缓冲区

static u32 SumResult = 0; 
int main(void)
{   
	u8 senddata_len,i;
	Stm32_Clock_Init(360,25,2,8);	//设置时钟,180Mhz
	delay_init(180);			    //初始化延时函数 
	uart_init(90,115200);		    //初始化串口波特率为115200
 	LED_Init();					    //初始化与LED连接的硬件接口

 	MYDMA_Config(DMA2_Stream7,4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE,0);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
	MYDMA_Config(DMA2_Stream2,4,(u32)&USART1->DR,(u32)ReceBuff,RECV_BYF_SIZE,1);//DMA2,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:SEND_BUF_SIZE.
 	MYDMA_Enable(DMA2_Stream2,RECV_BYF_SIZE);//开始一次DMA传输！
	memset(ReceBuff,0,sizeof(ReceBuff));
	while(1)
	{
		if(USART_RX_STA == 1)
		{
			//将输入数据赋值输出
			 for(i = 0; i < USART_RX_LEN;i++){
			 	SendBuff[i] = ReceBuff[i];
			 }
			USART1->CR3=1<<7;           //使能串口1的DMA发送
			MYDMA_Enable(DMA2_Stream7,USART_RX_LEN);
			USART_RX_STA = 0;
			SumResult = 0;
		}
		if((DMA2->HISR)&(1<<27))	//等待DMA2_Steam7传输完成
		{
			DMA2->HIFCR|=1<<27;	//清除DMA2_Steam7传输完成标志
			//break;
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//提示系统正在运行	
			i=0;
		}		   
	}		    
}

















