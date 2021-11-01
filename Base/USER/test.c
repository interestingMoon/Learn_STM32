#include "sys.h"
#include "delay.h" 
#include "led.h"  
#include "usart.h" 
#include "lcd.h" 
#include "ltdc.h"   
#include "sdram.h" 
#include "dma.h"
#include "key.h" 
//ALIENTEK ������STM32F429������ ʵ��23
//DMA ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 



u8 SendBuff[SEND_BUF_SIZE];	    //�������ݻ�����
u8 ReceBuff[RECV_BYF_SIZE];	    //�������ݻ�����

static u32 SumResult = 0; 
int main(void)
{   
	u8 senddata_len,i;
	Stm32_Clock_Init(360,25,2,8);	//����ʱ��,180Mhz
	delay_init(180);			    //��ʼ����ʱ���� 
	uart_init(90,115200);		    //��ʼ�����ڲ�����Ϊ115200
 	LED_Init();					    //��ʼ����LED���ӵ�Ӳ���ӿ�

 	MYDMA_Config(DMA2_Stream7,4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE,0);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
	MYDMA_Config(DMA2_Stream2,4,(u32)&USART1->DR,(u32)ReceBuff,RECV_BYF_SIZE,1);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
 	MYDMA_Enable(DMA2_Stream2,RECV_BYF_SIZE);//��ʼһ��DMA���䣡
	memset(ReceBuff,0,sizeof(ReceBuff));
	while(1)
	{
		if(USART_RX_STA == 1)
		{
			//���������ݸ�ֵ���
			 for(i = 0; i < USART_RX_LEN;i++){
			 	SendBuff[i] = ReceBuff[i];
			 }
			USART1->CR3=1<<7;           //ʹ�ܴ���1��DMA����
			MYDMA_Enable(DMA2_Stream7,USART_RX_LEN);
			USART_RX_STA = 0;
			SumResult = 0;
		}
		if((DMA2->HISR)&(1<<27))	//�ȴ�DMA2_Steam7�������
		{
			DMA2->HIFCR|=1<<27;	//���DMA2_Steam7������ɱ�־
			//break;
		}
		i++;
		delay_ms(10);
		if(i==20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}		   
	}		    
}

















