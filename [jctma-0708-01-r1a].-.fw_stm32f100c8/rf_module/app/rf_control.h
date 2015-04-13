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

#ifndef __RF_CONTROL_C__
#define	__RF_CONTROL_EXT__ 	 extern
#else
#define	__RF_CONTROL_EXT__
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/	 
/* Public define -------------------------------------------------------------*/
#define ID_SET_MODE						0x10
#define ID_GET_MODE                     0x11
#define ID_SET_POWER                    0x12
#define ID_GET_POWER                    0x13
#define ID_SET_VI_LIMIT            		0x14
#define ID_GET_VI_LIMIT            		0x15
#define ID_GET_VI          				0x16
#define ID_SET_ATT_GAIN                 0x17
#define ID_GET_ATT_GAIN                 0x18
#define ID_GET_TEMP_CUR                 0x19
#define ID_GET_TEMP_SCOPE               0x1A
#define ID_SET_TEMP_SCOPE_CUR 	        0x1B
#define ID_GET_ALARM                    0x1C
#define ID_SET_SWITCH                   0x1D
#define ID_GET_SWITCH					0x1E	
#define ID_RESERVED1                    0x1F
#define ID_ADDR_WD                      0x20
#define ID_ADDR_RD                      0x21
#define ID_SET_TEMP_ALARM_VALUE			0x22
#define ID_GET_TEMP_ALARM_VALUE			0x23
#define ID_RESET                        0x60
#define ID_LOAD_START                   0x61
#define ID_LOADING                      0x62
#define ID_LOAD_END                     0x63
#define ID_INQUIRE_STATE                0x64
#define ID_SET_INFO                     0x65
#define ID_GET_INFO                     0x66

#define ID_UPGRADEALL_START				0xF1
#define ID_UPGRADEALL_DOING				0xF2
#define ID_UPGRADEALL_END				0xF3

/*Alarm Detection*/
#define ALARM_CURRENT_OVER_15	(U16)(1<<0)
#define ALARM_CURRENT_UNDER_15	(U16)(1<<1)
#define ALARM_CURRENT_OVER_25	(U16)(1<<2)
#define ALARM_CURRENT_UNDER_25	(U16)(1<<3)
#define ALARM_CURRENT_OVER_19	(U16)(1<<4)
#define ALARM_CURRENT_UNDER_19	(U16)(1<<5)
#define ALARM_CURRENT_OVER_29	(U16)(1<<6)
#define ALARM_CURRENT_UNDER_29	(U16)(1<<7)

#define ALARM_VOLTAGE_OVER_12	(U16)(1<<8)
#define ALARM_VOLTAGE_UNDER_12	(U16)(1<<9)
#define ALARM_VOLTAGE_OVER_09	(U16)(1<<10)
#define ALARM_VOLTAGE_UNDER_09	(U16)(1<<11)
#define ALARM_VOLTAGE_OVER_05	(U16)(1<<12)
#define ALARM_VOLTAGE_UNDER_05	(U16)(1<<13)

#define ALARM_TEMPERATURE_HIGH	(U16)(1<<14)
#define ALARM_TEMPERATURE_LOW	(U16)(1<<15)

#define NORMAL_STATE			0x00
#define HOT_STANDBY				0x03

#define TaskControlEnter				((Procf)(BANK_CTRL_START_ADDR|0x1UL))
#define InitTaskCtrl()					TaskControlEnter(NULL,1)
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/       
/* Public function prototypes ------------------------------------------------*/
//__RF_CONTROL_EXT__ void InitControl(void);
//__RF_CONTROL_EXT__ int TaskControl(int*argv[],int argc);

#endif/*__RF_CONTROL_H__*/
/********************************END OF FILE***********************************/
