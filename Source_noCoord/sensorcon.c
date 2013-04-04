
#include "hal_board.h"
#include "hal_defs.h"
#include "hal_mcu.h"
#include "hal_types.h"
#include "sensorcon.h"
#include  "hal_adc.h"

#define		SCK_HIGH()		P1 |= 0x20
#define		SCK_LOW()		P1 &= 0xDF

#define		CS_HIGH()		P2 |= 0x01
#define		CS_LOW()		P2 &= 0xFE

#define		SPI_MISO		P1_7



/*****************************************************************************
 ��������
*****************************************************************************/
void TC77_PIN_INT(void);
void Sensor_PIN_INT(void);
uint8 ReadTc77(void);
void ACC_PIN_INT(void);
void SET_ADC_IO_SLEEP_MODE(void);
void SET_ADC_IO_ADC_MODE(void);


/*****************************************************************************
 void SET_ADC_IO_SLEEP_MODE(void)

  ����ADC I/O��Ϊ�͹���ģʽ.
*****************************************************************************/
void SET_ADC_IO_SLEEP_MODE(void)
{
	P0SEL &= ~(1<<0);    // 
	P0    &= ~(1<<0);
	P0DIR &= ~(1<<0);
	P0INP |= (1<<0);
           
	P0SEL &= ~(1<<1);    // adc�ر�
	P0    &= ~(1<<1);
	P0DIR &= ~(1<<1);
	P0INP |= (1<<1);
  
	P0SEL &= ~(1<<6);    // 
	P0    &= ~(1<<6);
	P0DIR &= ~(1<<6);
	P0INP |= (1<<6);
           
	P0SEL &= ~(1<<7);    // 
	P0    &= ~(1<<7);
	P0DIR &= ~(1<<7);
	P0INP |= (1<<7);
           
	APCFG &= ~0xC3;
}



/*****************************************************************************
 void SET_ADC_IO_ADC_MODE(void)

  ����ADC I/O��ΪADCģʽ.
*****************************************************************************/
void SET_ADC_IO_ADC_MODE(void)
{
	P0SEL |= (1<<0);    
	P0DIR &= ~(1<<0);
	
	P0SEL |= (1<<1);    
	P0DIR &= ~(1<<1);
	
	P0SEL |= (1<<6);    
	P0DIR &= ~(1<<6);
	
	P0SEL |= (1<<7);    
	P0DIR &= ~(1<<7);
	
	APCFG |= 0xC3;
}







/*****************************************************************************
 void TC77_PIN_INT(void)

  TC77 I/O�ڳ�ʼ��.
*****************************************************************************/
void TC77_PIN_INT(void)
{
	P2SEL &= ~(1<<0);    
	P2    &= ~(1<<0);
	P2DIR &= ~(1<<0);
	P2INP |= (1<<0);  
 	//TC77_CS
	
	
	P1SEL &= ~(1<<5);   
	P1    &= ~(1<<5); 
	P1DIR &= ~(1<<5);  
	P1INP |= (1<<5);
 	//TC77_SCK
	
	
	P1SEL &= ~(1<<7);    
	P1DIR &= ~(1<<7);
	P1    &= ~(1<<7);
	P1INP |= (1<<7);  
	//TC77_MISO
}


/*****************************************************************************
 void ACC_PIN_INT(void)

  ���ٶȴ�������ADC I/O�ڳ�ʼ��.
*****************************************************************************/
void ACC_PIN_INT(void)
{
	P1SEL &= ~(1<<2);    
	P1DIR |= (1<<2);  
	P1    |= (1<<2); 	//SLEEP
	P1INP |= (1<<2); 
	
	SET_ADC_IO_SLEEP_MODE();//ADC����������͹���ģʽ
	
}


/*****************************************************************************
  void Sensor_PIN_INT(void)

  ��������ADC I/O�ڳ�ʼ��.
*****************************************************************************/
void Sensor_PIN_INT(void)
{
	P1SEL &= ~(1<<3);    
	P1DIR |= (1<<3);
	P1INP |= (1<<3); //Sensor power con int
	
	SensorPowerOff();//��������Դ��
	
	ACC_PIN_INT();//���ٶȴ�����I/O�ڳ�ʼ��

	ACC_SLEEP();//���ٶȴ���������˯��ģʽ
	
	TC77_PIN_INT();//TC77 I/O��ʼ��
}







/*****************************************************************************
  ReadTc77

  SPIģʽ��ȡ�¶ȴ�����
*****************************************************************************/
uint8 ReadTc77(void)
{
	uint16 temp=0;
	uint8 i;
	
	SCK_LOW();   
	CS_LOW();
	
	for(i=0; i<16; i++)
	{
		temp <<= 1;
		
		SCK_HIGH();
		
		asm("nop");
		
		if(SPI_MISO)temp++;
		
		SCK_LOW();   
		
		asm("nop");
	}
	
	CS_HIGH();
	
        i = temp >> 7;
        
	return i;
}