/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __DEV_E2PROM_C__
#define __DEV_E2PROM_C__
#endif
/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EEPROM_DELAY        1000
#define EEPROM_PAGE_SIZE    64
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
	static U8 buf[64];

	InitTarget();
	ReadE2prom (0x0000,buf,64);
	WriteE2prom(0x0000,buf,64);
	ReadE2prom (0x0000,buf,64);	

	for(;;);
}
#endif
/**
  * @brief  :ר������дEEPROM�ĺ�����ʹ֮���Կ�ҳ(һҳ64�ֽ�,���������趨)д
  * @param	:iaddr  EEPROM�ڲ���ַ
  * @param	:pbuf   ��Ҫд������ݵ�ָ��
  * @param	:plen   ��Ҫд������ݵĸ���
  * @retval	:TRUE   �����ɹ�
  * @retval	:FALSE  ����ʧ��
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL WriteE2prom(U16 iaddr, U8 *pbuf, U16 plen)
{ 
    U8 	addr[2];
    U32 len, sentLen = 0;

    EEPROM_WP_UNPROTECT();                                                     
    while (sentLen < plen)
    {
        addr[0] = (iaddr + sentLen) >> 8;
        addr[1] = (iaddr + sentLen) & 0xFF;

        len = EEPROM_PAGE_SIZE - ((iaddr + sentLen) % EEPROM_PAGE_SIZE);        /* EEROM��ҳʣ��ռ�    */

        if (plen - sentLen < len)                                                /* δд�����ݱȽ���     */
        {
            len = plen - sentLen;
        }

        if (FALSE == I2cWriteDevice(I2C1,I2C_EEPROM_ADDR, addr, sizeof(addr), pbuf+sentLen, len))
        {
            usdelay(EEPROM_DELAY);
            break;
        }

        usdelay(EEPROM_DELAY);
        sentLen += len;
    }
    EEPROM_WP_PROTECT();
    
    return (sentLen == plen);
}
/**
  * @brief  :ר�����ڶ�EEPROM�ĺ���
  * @param	:iaddr  EEPROM�ڲ���ַ
  * @param	:pbuf   ��Ҫ���������ݵ�ָ��
  * @param	:plen   ��Ҫ���������ݵĸ���
  * @retval	:TRUE   �����ɹ�
  * @retval	:FALSE  ����ʧ��
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL ReadE2prom(U16 iaddr, U8 *pbuf, U16 plen)
{
    U8 addr[2];

    addr[0] = iaddr >> 8;
    addr[1] = iaddr & 0xff;

    return I2cReadDevice(I2C1, I2C_EEPROM_ADDR, addr, sizeof(addr), pbuf, plen);
}
/********************************END OF FILE***********************************/
