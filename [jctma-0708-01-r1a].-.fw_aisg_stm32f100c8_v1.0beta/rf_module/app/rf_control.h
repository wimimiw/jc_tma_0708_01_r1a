/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:rf_control.h
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __RF_CONTROL_H__
#define __RF_CONTROL_H__
#endif

#ifndef __RF_CONTROL_C__
#define	__RF_CONTROL_EXT__ 	 extern
#else
#define	__RF_CONTROL_EXT__
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
typedef  struct
{
	U8	procID;
	U8	issued;
	U8	require;
	U8	procClass;
	U8	procMode;
	U8	pwrMode;
	U8	lenType;
	//set/get,buf pointer,buf length	
	U8	action;
	U16 (*proc)(U8,U8*,U16);
}SLAYER7_AISG;	 
/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
__RF_CONTROL_EXT__ const SLAYER7_AISG gLayer7AISGV11[];
__RF_CONTROL_EXT__ const SLAYER7_AISG gLayer7AISGV20[];
/* Public variables ----------------------------------------------------------*/            
/* Public function prototypes ------------------------------------------------*/
__RF_CONTROL_EXT__ U8	NumItemOfAISG11(void);
__RF_CONTROL_EXT__ U8	NumItemOfAISG20(void);
/********************************END OF FILE***********************************/
