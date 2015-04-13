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
#ifndef __DEV_SPI_H__
#define __DEV_SPI_H__

#ifndef __DEV_SPI_C__
#define	__DEV_SPI_EXT__ 	 extern
#else
#define	__DEV_SPI_EXT__
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
//SPI�ܽŶ���
typedef struct {
    U16 cs[2];	  	//Ƭѡ�ܽţ��ֽ�0=�ܽ������ַ���ֽ�1=�ܽ����
    U16 clk[2];  	//ʱ�ӹܽţ��ֽ�0=�ܽ������ַ���ֽ�1=�ܽ����
    U16 data[2]; 	//���ݹܽţ��ֽ�0=�ܽ������ַ���ֽ�1=�ܽ����
}SPI_BUS;
//SPI���ݸ�ʽ
typedef struct _SPI_TYPE_ {
    U8 order;
    U8 type;
    U8 len;
    U32 mask;
} SPI_TYPE;
/* Public define -------------------------------------------------------------*/
#define MSB_FIRST           1
#define LSB_FIRST           0

#define SPI_PULSE_HIGH      0
#define SPI_PULSE_LOW       1
#define SPI_LEVEL_HIGH      2
#define SPI_LEVEL_LOW       3
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/
__DEV_SPI_EXT__ BOOL WriteSpiOneWord(const SPI_BUS*spiBus,SPI_TYPE*spiType, U32 spiWord);

#endif/*__DEV_SPI_H*/
/********************************END OF FILE***********************************/
