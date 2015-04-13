/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:dev_temp.c
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __DEV_TEMP_C__
#define __DEV_TEMP_C__
#endif
/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  :debug driver
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
//#define __DEBUG_THIS
#ifdef __DEBUG_THIS 
int main(void)
{
	static S16 tempe,tempi,diff;

	InitTarget();
	tempi = ReadInternalTemperatureSensor()/2;	
	tempe = ReadExternalTemperatureSensor()/2;	

	diff = tempi - tempe;
	diff = diff;		
	for(;;);
}
#endif
/**
  * @brief  :ר�����ڶ�LM75�ĺ���
  * @param  :pbuf     �Ĵ�����ַ
  * @param  :plen   �Ĵ���ֵ��ָ��
  * @retval :temperature value
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static S16 readLm75(void)
{
#define LM75_TEMPERATURE_VALUE_ADDR			0x00
#define LM75_TEMPERATURE_VALUE_ADDR_SIZE	1	
	U8	value[2];
	U8	iaddr = LM75_TEMPERATURE_VALUE_ADDR;
	S16 temperature;	

	I2cReadDevice(	I2C1,I2C_LM75_ADDR1,
					&iaddr,
					LM75_TEMPERATURE_VALUE_ADDR_SIZE,
					value,sizeof(value));

    temperature  = value[0] << 8;
    temperature |= value[1] >> 0;
	temperature /= 128;

	return temperature;
}
/**
  * @brief  :��ȡMCU�ڲ��¶ȴ�����
  * @param  :None
  * @retval :float temperature value
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
S16 ReadInternalTemperatureSensor(void)
{
	return 	ReadAdcTemperature();
}
/**
  * @brief  :��ȡ�ⲿ�¶ȴ�����
  * @param  :None
  * @retval :signed short temperature value
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
S16 ReadExternalTemperatureSensor(void)
{
	return  readLm75();;
}
/********************************END OF FILE***********************************/
