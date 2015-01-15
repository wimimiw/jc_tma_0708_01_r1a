/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:rf_parameter.c
 * @author	:mashuai
 * @version	:v1.0 
 * @date	:2012.4.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __RF_PARAMETER_C__
#define __RF_PARAMETER_C__
#endif
/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FIRMWARE_VERSION		(U16)0x0100				//v1.0
//CAT24WC256中0x7000开始存放数据
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  :Read the code from the irom,and calculate the chksum of the firmware
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
//static U16 CalcCheckSum(void)
//{
//	U8  *pFlash;	
//	U16 checksum= 0;
//
//	for (pFlash = (U8*)BANK_WORK_START_ADDR; pFlash < (U8*)(BANK_WORK_START_ADDR + 0x8000); pFlash++)
//	{
//		checksum += *pFlash;
//	}
//	
//	return (checksum - 0x8000);
//}
/**
  * @brief  :Check the firmware whether that the firmware has been upgrade. 
  * @param  :None
  * @retval :BOOL
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL CheckDownFlag(void)
{
//	if( TRUE == ReadE2prom(EE_DOWN_FLAG,&gFctParam.upgrade,sizeof(gFctParam.upgrade)) )
//	{
//		if( 0xFF == gFctParam.upgrade )
//		{
//			gFctParam.upgrade = 0x00;
//			if ( TRUE == WriteE2prom(EE_DOWN_FLAG,&gFctParam.upgrade,sizeof(gFctParam.upgrade)) )
//			{
//	        	return TRUE;
//			}
//	    }		    
//	}
	return FALSE;
}/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
void SetDownFlag(void)
{
	//gFctParam.upgrade = 0xFF;
	//WriteE2prom(EE_DOWN_FLAG,&gFctParam.upgrade,sizeof(gFctParam.upgrade));	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL SaveParam(U16 iaddr, U8 *pbuf, U16 plen)
{
	//return WriteE2prom(EE_OFFSET + iaddr,pbuf,plen);
	return TRUE; 
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL FetchParam(U16 iaddr, U8 *pbuf, U16 plen)
{
	//return ReadE2prom(EE_OFFSET + iaddr,pbuf,plen);
	return TRUE;	
}
/**
  * @brief  :Initialize the parameters of the rf module.
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
void InitParam(void)
{
}
/**
  * @brief  :parameter aask routine
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
void TaskParameter(void)
{
	//do nothing!	
}
/********************************END OF FILE***********************************/
