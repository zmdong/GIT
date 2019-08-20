#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
/************************************************
 ALIENTEK ������STM32F429������ʵ��13
 SDRAMʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

u16 testsram[250000] __attribute__((at(0XC01F4444)));//����������

//SDRAM�ڴ����	    
void fsmc_sdram_test(u16 x,u16 y)
{  
	u32 i=0;  	  
	volatile u32 temp=0;	   
	volatile u32 sval=0;	//�ڵ�ַ0����������	  				   
  	LCD_ShowString(x,y,180,y+16,16,"Ex Memory Test:    0KB "); 
	//ÿ��16K�ֽ�,д��һ������,�ܹ�д��2048������,�պ���32M�ֽ�
	for(i=0;i<32*1024*1024;i+=16*1024)
	{
		*(vu32*)(Bank5_SDRAM_ADDR+i)=temp; 
		temp++;
	}
	//���ζ���֮ǰд�������,����У��		  
 	for(i=0;i<32*1024*1024;i+=16*1024) 
	{	
  		temp=*(vu32*)(Bank5_SDRAM_ADDR+i);
		if(i==0)sval=temp;
 		else if(temp<=sval)break;//�������������һ��Ҫ�ȵ�һ�ζ��������ݴ�.	   		   
		LCD_ShowxNum(x+15*8,y,(u16)(temp-sval+1)*16,5,16,0);	//��ʾ�ڴ�����  
		printf("SDRAM Capacity:%dKB\r\n",(u16)(temp-sval+1)*16);//��ӡSDRAM����
 	}					 
}	

int main(void)
{
	u8 key;		 
	u8 i=0;	     
	u32 ts=0; 
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    LCD_Init();	                    //��ʼ��LCD 
    POINT_COLOR=RED;                //��������Ϊ��ɫ
    LCD_ShowString(30,50,200,16,16,"APOLLO STM32F4/F7");	
	LCD_ShowString(30,70,200,16,16,"SDRAM TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/12/11");   
	LCD_ShowString(30,130,200,16,16,"KEY0:Test Sram");
	LCD_ShowString(30,150,200,16,16,"KEY1:TEST Data");
	POINT_COLOR=BLUE;       //��������Ϊ��ɫ 
	for(ts=0;ts<250000;ts++)
	{
		testsram[ts]=ts;//Ԥ���������	 
	} 	
	while(1)
	{
		key=KEY_Scan(0);//��֧������	
		if(key==KEY0_PRES)fsmc_sdram_test(30,170);//����SRAM����
		else if(key==KEY1_PRES)//��ӡԤ���������
		{
			for(ts=0;ts<250000;ts++)
			{
				LCD_ShowxNum(30,190,testsram[ts],6,16,0);//��ʾ��������	
				//printf("testsram[%d]:%d\r\n",ts,testsram[ts]);
			}
		}else delay_ms(10);   
		i++;
		if(i==20)//DS0��˸.
		{
			i=0;
			LED0=!LED0;
		}
	}
}
