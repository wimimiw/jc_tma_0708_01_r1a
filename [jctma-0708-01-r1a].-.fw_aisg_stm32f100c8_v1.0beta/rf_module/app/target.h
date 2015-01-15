/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:rf_target.h
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __TARGET_H__
#define __TARGET_H__
#endif	  

#ifndef __TARGET_C__
#define	__TARGET_EXT__ 	 extern
#else	  
#define	__TARGET_EXT__
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
#define I2C_EEPROM_ADDR     		0xA0
#define I2C_LM75_ADDR1      		0x90
#define I2C_LM75_ADDR2      		0x90

#define OUT_AD_MAX         			1023 //4095 the omt is limited
                         		
//operater the gpio
#define GPIORF_SW5V1(x)				GPIO_WriteBit(GPIOB,GPIO_Pin_9 ,LOW == x ? Bit_RESET:Bit_SET)
#define GPIORF_SW5V2(x)		    	GPIO_WriteBit(GPIOA,GPIO_Pin_11,LOW == x ? Bit_RESET:Bit_SET)
#define GPIORF_SW9V1(x)				GPIO_WriteBit(GPIOB,GPIO_Pin_8 ,LOW == x ? Bit_RESET:Bit_SET)
#define GPIORF_SW9V2(x)				GPIO_WriteBit(GPIOA,GPIO_Pin_15,LOW == x ? Bit_RESET:Bit_SET)
	
#define RF_GPIO_WR_SW1(x)			GPIO_WriteBit(GPIOA,GPIO_Pin_1,LOW == x ? Bit_RESET:Bit_SET)
#define RF_GPIO_WR_SW2(x)			GPIO_WriteBit(GPIOA,GPIO_Pin_2,LOW == x ? Bit_RESET:Bit_SET)
#define RF_GPIO_WR_RX2(x)			GPIO_WriteBit(GPIOA,GPIO_Pin_0,LOW == x ? Bit_RESET:Bit_SET)
	
#define RF_GPIO_WR_LNA1_ALARM(x)	GPIO_WriteBit(GPIOC,GPIO_Pin_13,LOW == x ? Bit_RESET:Bit_SET)
#define RF_GPIO_WR_LNA2_ALARM(x)	GPIO_WriteBit(GPIOC,GPIO_Pin_14,LOW == x ? Bit_RESET:Bit_SET)
	
#define RF_GPIO_SW_OUTPWR()			GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET	)		
#define RF_GPIO_SW_REFPWR()			GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET)

#define	RF_GPIO_RD_SW130DB()		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define	RF_GPIO_RD_SW230DB()		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)

#define EEPROM_WP_UNPROTECT()   	//GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET  )
#define EEPROM_WP_PROTECT()     	//GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET)
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
extern  const SPI_BUS attBus[];
__TARGET_EXT__ __IO U16 ADCConvertedValue;
/* Public function prototypes ------------------------------------------------*/
void InitTarget(void);
/********************************END OF FILE***********************************/
