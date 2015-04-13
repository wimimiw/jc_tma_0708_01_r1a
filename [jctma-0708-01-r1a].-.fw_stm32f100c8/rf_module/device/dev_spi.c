/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:dev_spi.h
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __DEV_SPI_C__
#define __DEV_SPI_C__
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
  * @brief  :operater the gpio
  * @param  :group 
  * @param  :pin
  * @param 	:level
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static void WriteGpioPin(U8 group, U16 pin, BOOL level)
{		
	GPIO_TypeDef*gpio;
	switch (group)
	{
		case GPIO_PortSourceGPIOA:gpio = GPIOA;break;
		case GPIO_PortSourceGPIOB:gpio = GPIOB;break;
		case GPIO_PortSourceGPIOC:gpio = GPIOC;break;
		case GPIO_PortSourceGPIOD:gpio = GPIOD;break;
		default:return;
	}
	GPIO_WriteBit(gpio,pin,(HIGH == level)?Bit_SET:Bit_RESET);
}
/**
  * @brief  :���ڼ�������λ��Ӧ��ֵ
  * @param  :None
  * @param	:data   ��Ҫ��ȡ���������
  * @param	:mask   ��Ҫ������λ���ݵ�����
  * @param 	:order  ���ݷ��͵�˳��
  * @param 	:num    ��Ҫ���͵�λ  
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static U8 ChkMaskBit(U32 data, U32 mask, U8 num, U8 order)
{

    if (MSB_FIRST == order)
    {
        mask >>= num;
    }
    else
    {
        mask <<= num;
    }

    return ((data & mask) == 0 ? LOW : HIGH);

}
/**
  * @brief  :����PSI����
  * @param  :spiType SPI�ӿڵ�����
  *	@param	:spiBus  SPI���ߵ�ͨ��
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static void EndSpiBus(const SPI_BUS * spiBus,SPI_TYPE * spiType)
{
    switch ( spiType->type )
    {
        case SPI_LEVEL_LOW:
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  HIGH);          /* ����ʹ�ܽ�                   */
            break;

        case SPI_LEVEL_HIGH:
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  LOW);           /* ����ʹ�ܽ�                   */
            break;

        case SPI_PULSE_LOW:
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  LOW);           /* ����ʹ�ܽ�                   */
            usdelay(40);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  HIGH);          /* ����ʹ�ܽ�                   */
            break;

        case SPI_PULSE_HIGH:
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  HIGH);          /* ����ʹ�ܽ�                   */
            usdelay(40);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  LOW);           /* ����ʹ�ܽ�                   */
            break;

        default:
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  HIGH);          /* ����ʹ�ܽ�                   */
            break;
    }
}
/**
  * @brief  :����PSI����
  * @param  :spiType SPI�ӿڵ�����
  * @param	:spiBus  SPI���ߵ�ͨ��
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static void StartSpiBus(const SPI_BUS* spiBus,SPI_TYPE* spiType)
{
    switch ( spiType->type )
    {
        case SPI_LEVEL_LOW:
            WriteGpioPin(spiBus->clk[0],spiBus->clk[1], LOW);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  HIGH);          /* ����ʹ�ܽ�                   */
            usdelay(120);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  LOW);           /* ����ʹ�ܽ�                   */
            break;

        case SPI_LEVEL_HIGH:
            WriteGpioPin(spiBus->clk[0],spiBus->clk[1], LOW);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  LOW);           /* ����ʹ�ܽ�                   */
            usdelay(120);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  HIGH);          /* ����ʹ�ܽ�                   */
            break;

        case SPI_PULSE_LOW:
            WriteGpioPin(spiBus->clk[0],spiBus->clk[1], LOW);
            usdelay(120);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  HIGH);          /* ����ʹ�ܽ�                   */
            break;

        case SPI_PULSE_HIGH:
            WriteGpioPin(spiBus->clk[0],spiBus->clk[1], LOW);
            usdelay(120);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  LOW);           /* ����ʹ�ܽ�                   */
            break;

        default:
            WriteGpioPin(spiBus->clk[0],spiBus->clk[1], LOW);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  HIGH);          /* ����ʹ�ܽ�                   */
            usdelay(120);
            WriteGpioPin(spiBus->cs[0],spiBus->cs[1],  LOW);           /* ����ʹ�ܽ�                   */
            break;
    }
}
/**
  * @brief  :��PSI�ӿ�дһ����
  * @param  :spiType SPI�ӿڵ�����
  * @param	:spiBus  SPI���ߵ�ͨ��
  * @param	:spiWord SPI��Ҫ���͵�����
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL WriteSpiOneWord(const SPI_BUS* spiBus, SPI_TYPE* spiType, U32 spiWord)
{
    U8 i;
    U8 data;

    /* ����SPI����  */
    StartSpiBus(spiBus,spiType);

    /* ��������λ   */
    for (i = 0; i < spiType->len; i++)                                              /* SPI���ݷ���                  */
    {

        data = ChkMaskBit(spiWord, spiType->mask, i, spiType->order);               /* �������ݽŵ�ƽ               */

        usdelay(120);
        WriteGpioPin(spiBus->data[0],spiBus->data[1], data);
        usdelay(120);
        WriteGpioPin(spiBus->clk[0],spiBus->clk[1],  HIGH);            /* ����ʱ������                 */
        usdelay(120);
        WriteGpioPin(spiBus->clk[0],spiBus->clk[1],   LOW);
    }

    /* ����SPI����  */
    usdelay(240);
    EndSpiBus( spiBus,spiType );

    return TRUE;
}
/********************************END OF FILE***********************************/
