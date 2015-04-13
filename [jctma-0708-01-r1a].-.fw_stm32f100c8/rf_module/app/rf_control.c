/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:rf_control.c
 * @author	:mashuai
 * @version	:v1.0 
 * @date	:2012.4.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 	20130116:
	   V0x32:section of .constdata
	   V0x40:1)The bug in the data initialize.when modify the gloabe or static data
	    	 in the rf_control.c,the code in the head of firmware will change by 
			 the data initialize process.
			 2)Add the temperature compensate.
			 3)Modify that upgrade can only active in the same hardware version.
			 4)hotStandby modification.
	   V0x50:Optimize the upgrade function
	   V0x51:modify	the hotStandby rollback
	   V0x52:Optimize the Reset Temperature Max&Min process.
	   V0x60:11.0592MHz(HSE_VALUE) -> 8.000000MHz(HSI_VALUE)
		 V0x61:temperature perhapes will be 0xFF,so to screen this situation.
 ******************************************************************************/
#ifndef __RF_CONTROL_C__
#define __RF_CONTROL_C__
#endif
/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct __pack
{
	BOOL test;					//
	U8 path;                    //
                                //
	U8 power15;                 //
	U8 power25;                 //
	U8 power19;                 //
	U8 power29;	                //
                                //
	U16 curLimitLow15;          //
	U16 curLimitHigh15;         //
	U16 curLimitLow25;          //
	U16 curLimitHigh25;         //
	U16 curLimitLow19;          //
	U16 curLimitHigh19;         //
	U16 curLimitLow29;          //
	U16 curLimitHigh29;         //
								//
	U16 vdcLimitLow12;			//
	U16 vdcLimitHigh12;			//
	U16 vdcLimitLow09;			//
	U16 vdcLimitHigh09;			//
	U16 vdcLimitLow05;			//
	U16 vdcLimitHigh05;	   		//
                                //
	U16 curDrain15;             //
	U16 curDrain25;             //
	U16 curDrain19;             //
	U16 curDrain29;             //
								//
	U16 vdc12V;                 //
	U16 vdc9V;                  //
	U16 vdc5V;                  //
                                //
	U8 attGain1;                //
	U8 attGain2;                //
								//
	U8 rfSwitch1;				//
	U8 rfSwitch2;				//
	U8 rfSwitch3;				//
	U8 rfSwitch4;				//
	U8 rfSwitch5;				//
                                //
	S16 curTemp;                //
	S16 tempLow;                //
	S16 tempHigh;	            //
	s16 tempAlarmLow;			//
	s16 tempAlarmHigh;			//
                                //
	U16 alarm;                  //                                
	U8 addr;                    //
	                            //
	U8 reset;                   //
                                //
	U8 upgrade;					//
	U8 bootV;                   //
	U8 softV;                   //
	U8 hardV;                   //
	U16 checksum;               //
                                //
	U8 model[20];	            //
	U8 sn[20];                  //
}SGLOBAL_VAR;

typedef struct
{
	U8 cmd;
	U8 sub;
	U8 type;
	U8 *var;
	U8 varLen;	
	BOOL (*proc)(U8*,U16);		
}STABLE_COMMAND;
/* Private define ------------------------------------------------------------*/                                          	     	                                                       
//#define RFPOWER_SYC_5V_1(x)		do{RFPOWER_5V_1(x)	  ;gFctPara.power15   = x;}while(0)
//#define RFPOWER_SYC_5V_2(x)		do{RFPOWER_5V_2(x)	  ;gFctPara.power25   = x;}while(0)
//#define RFPOWER_SYC_9V_1(x)		do{RFPOWER_9V_1(x)	  ;gFctPara.power19   = x;}while(0)
//#define RFPOWER_SYC_9V_2(x)		do{RFPOWER_9V_2(x)	  ;gFctPara.power29   = x;}while(0)

//#define RFSWITCH_SYC_PATH1(x) 	do{RFSWITCH_PATH1(x)  ;gFctPara.rfSwitch1 = x;}while(0)
//#define RFSWITCH_SYC_PATH2(x) 	do{RFSWITCH_PATH2(x)  ;gFctPara.rfSwitch2 = x;}while(0)
//#define RFSWITCH_SYC_BYPASS1(x) do{RFSWITCH_BYPASS1(x);gFctPara.rfSwitch3 = x;}while(0)
//#define RFSWITCH_SYC_BYPASS2(x) do{RFSWITCH_BYPASS2(x);gFctPara.rfSwitch4 = x;}while(0)
//#define RFSWITCH_SYC_TEST(x) 	do{RFSWITCH_TEST(x)   ;gFctPara.rfSwitch5 = x;}while(0)

//#define RFSWITCH_SYC_BYPASS(x)	do{RFSWITCH_SYC_BYPASS1(x);RFSWITCH_SYC_BYPASS2(x);}while(0)															  
//#define RFSWITCH_SYC_PATH(x)	do{								  \
//									RFPOWER_SYC_5V_1(FALSE);	  \
//									RFPOWER_SYC_5V_2(FALSE);      \
//									RFPOWER_SYC_9V_1(FALSE);	  \
//									RFPOWER_SYC_9V_2(FALSE);	  \
//									RFSWITCH_SYC_BYPASS(FALSE);	  \
//									if ( x == BYPASS )			  \
//									{							  \
//										RFSWITCH_SYC_BYPASS(TRUE);\
//									}							  \
//									else if ( x == PATH1 )		  \
//									{							  \
//										RFPOWER_SYC_5V_1(TRUE);	  \
//										RFPOWER_SYC_9V_1(TRUE);	  \
//										RFSWITCH_SYC_PATH1(1);	  \
//										RFSWITCH_SYC_PATH2(1);	  \
//									}							  \
//									else if ( x == PATH2 )		  \
//									{							  \
//										RFPOWER_SYC_5V_2(TRUE);	  \
//										RFPOWER_SYC_9V_2(TRUE);	  \
//										RFSWITCH_SYC_PATH1(2);	  \
//										RFSWITCH_SYC_PATH2(2);	  \
//									}							  \
//									else if ( x == HOT_STANDBY )  \
//									{							  \
//										RFPOWER_SYC_5V_1(TRUE);	  \
//										RFPOWER_SYC_9V_1(TRUE);	  \
//										RFPOWER_SYC_5V_2(TRUE);	  \
//										RFPOWER_SYC_9V_2(TRUE);	  \
//										RFSWITCH_SYC_PATH1(1);	  \
//										RFSWITCH_SYC_PATH2(1);	  \
//									}							  \
//								}while(0)	

#define STATIC					//steady static in the funtion with upgradeSct
#define gFctPara				(*(SGLOBAL_VAR*)mallocFct)
/* Private macro -------------------------------------------------------------*/
const U8 BootloaderV 	__attribute__((at(ADDR_BYTE_BOOTLOADERV))) = 0x11;
const U8 SoftwareV 		__attribute__((at(ADDR_BYTE_SOFTWAREV  ))) = 0x62;
const U8 HardwareV 		__attribute__((at(ADDR_BYTE_HARDWAREV  ))) = 0x50;
/* Private variables ---------------------------------------------------------*/
static U8 mallocFct[500];		//don`t modify this	with upgradeSct
static U8 mallocBuf[500];		//don`t modify this with upgradeSct
/* Private function prototypes -----------------------------------------------*/
static BOOL	ProcSetMode(U8*buf,U16 len);
static BOOL	ProcSetPower1(U8*buf,U16 len);
static BOOL	ProcSetPower2(U8*buf,U16 len);
static BOOL	ProcSetPower3(U8*buf,U16 len);
static BOOL	ProcSetPower4(U8*buf,U16 len);
static BOOL ProcSwitchPath1(U8*buf,U16 len);
static BOOL ProcSwitchPath2(U8*buf,U16 len);
static BOOL ProcSwitchBypass1(U8*buf,U16 len);
static BOOL ProcSwitchBypass2(U8*buf,U16 len);
static BOOL ProcSwitchTest(U8*buf,U16 len);
static BOOL	ProcSetGain(U8*buf,U16 len);
static BOOL	ProcSetReset(U8*buf,U16 len);
static BOOL ProcSetTemperatorReset(U8*buf,U16 len);

static const STABLE_COMMAND tabCommand[]=
{
	{ID_SET_MODE			,0xFF,PARAM_WTE	,(U8*)&gFctPara.test			,2,	ProcSetMode		,},
 	{ID_GET_MODE            ,0xFF,PARAM_RTE	,(U8*)&gFctPara.test			,2,	NULL,},

 	{ID_SET_POWER           ,0x15,PARAM_WTE	,(U8*)&gFctPara.power15			,1,	ProcSetPower1	,},
 	{ID_SET_POWER           ,0x25,PARAM_WTE	,(U8*)&gFctPara.power25			,1,	ProcSetPower2	,},
 	{ID_SET_POWER           ,0x19,PARAM_WTE	,(U8*)&gFctPara.power19			,1,	ProcSetPower3	,},
 	{ID_SET_POWER           ,0x29,PARAM_WTE	,(U8*)&gFctPara.power29			,1,	ProcSetPower4	,},
                                                                    		
 	{ID_GET_POWER           ,0x15,PARAM_RTE	,(U8*)&gFctPara.power15			,1,	NULL,},
 	{ID_GET_POWER           ,0x25,PARAM_RTE	,(U8*)&gFctPara.power25			,1,	NULL,},
 	{ID_GET_POWER           ,0x19,PARAM_RTE	,(U8*)&gFctPara.power19			,1,	NULL,},
 	{ID_GET_POWER           ,0x29,PARAM_RTE	,(U8*)&gFctPara.power29			,1,	NULL,},

 	{ID_SET_VI_LIMIT   		,0x15,PARAM_WTE	,(U8*)&gFctPara.curLimitLow15	,4,	NULL,},
 	{ID_SET_VI_LIMIT   		,0x25,PARAM_WTE	,(U8*)&gFctPara.curLimitLow25	,4,	NULL,},
 	{ID_SET_VI_LIMIT   		,0x19,PARAM_WTE	,(U8*)&gFctPara.curLimitLow19	,4,	NULL,},
 	{ID_SET_VI_LIMIT   		,0x29,PARAM_WTE	,(U8*)&gFctPara.curLimitLow29	,4,	NULL,},
 	{ID_SET_VI_LIMIT   		,0x12,PARAM_WTE	,(U8*)&gFctPara.vdcLimitLow12	,4,	NULL,},
 	{ID_SET_VI_LIMIT   		,0x09,PARAM_WTE	,(U8*)&gFctPara.vdcLimitLow09	,4,	NULL,},
 	{ID_SET_VI_LIMIT   		,0x05,PARAM_WTE	,(U8*)&gFctPara.vdcLimitLow05	,4,	NULL,},
                                                                         	
 	{ID_GET_VI_LIMIT   		,0x15,PARAM_RTE	,(U8*)&gFctPara.curLimitLow15	,4,	NULL,},
 	{ID_GET_VI_LIMIT   		,0x25,PARAM_RTE	,(U8*)&gFctPara.curLimitLow25	,4,	NULL,},
 	{ID_GET_VI_LIMIT   		,0x19,PARAM_RTE	,(U8*)&gFctPara.curLimitLow19	,4,	NULL,},
 	{ID_GET_VI_LIMIT   		,0x29,PARAM_RTE	,(U8*)&gFctPara.curLimitLow29	,4,	NULL,},
 	{ID_GET_VI_LIMIT   		,0x12,PARAM_RTE	,(U8*)&gFctPara.vdcLimitLow12	,4,	NULL,},
 	{ID_GET_VI_LIMIT   		,0x09,PARAM_RTE	,(U8*)&gFctPara.vdcLimitLow09	,4,	NULL,},
 	{ID_GET_VI_LIMIT   		,0x05,PARAM_RTE	,(U8*)&gFctPara.vdcLimitLow05	,4,	NULL,},

 	{ID_GET_VI			   	,0x15,PARAM_RT	,(U8*)&gFctPara.curDrain15		,2,	NULL,},
 	{ID_GET_VI   			,0x25,PARAM_RT	,(U8*)&gFctPara.curDrain25		,2,	NULL,},
 	{ID_GET_VI   			,0x19,PARAM_RT	,(U8*)&gFctPara.curDrain19		,2,	NULL,},
 	{ID_GET_VI   			,0x29,PARAM_RT	,(U8*)&gFctPara.curDrain29		,2,	NULL,}, 	
	{ID_GET_VI   			,0x12,PARAM_RT	,(U8*)&gFctPara.vdc12V			,2,	NULL,},
 	{ID_GET_VI   			,0x09,PARAM_RT	,(U8*)&gFctPara.vdc9V			,2,	NULL,},
 	{ID_GET_VI   			,0x05,PARAM_RT	,(U8*)&gFctPara.vdc5V			,2,	NULL,},
 	
	{ID_SET_ATT_GAIN        ,0x01,PARAM_WTE	,(U8*)&gFctPara.attGain1		,1,	ProcSetGain		,},
	{ID_SET_ATT_GAIN        ,0x02,PARAM_WTE	,(U8*)&gFctPara.attGain2		,1,	ProcSetGain		,},

	{ID_GET_ATT_GAIN        ,0x01,PARAM_RTE	,(U8*)&gFctPara.attGain1		,1,	NULL,},
	{ID_GET_ATT_GAIN        ,0x02,PARAM_RTE	,(U8*)&gFctPara.attGain2		,1,	NULL,},

	{ID_SET_SWITCH        	,0x01,PARAM_WT	,(U8*)&gFctPara.rfSwitch1       ,1,	ProcSwitchPath1	,},
	{ID_SET_SWITCH        	,0x02,PARAM_WT	,(U8*)&gFctPara.rfSwitch2       ,1,	ProcSwitchPath2	,},
	{ID_SET_SWITCH        	,0x03,PARAM_WT	,(U8*)&gFctPara.rfSwitch3       ,1,	ProcSwitchBypass1,},
	{ID_SET_SWITCH        	,0x04,PARAM_WT	,(U8*)&gFctPara.rfSwitch4       ,1,	ProcSwitchBypass2,},
	{ID_SET_SWITCH        	,0x05,PARAM_WT	,(U8*)&gFctPara.rfSwitch5       ,1,	ProcSwitchTest	,},

	{ID_GET_SWITCH        	,0x01,PARAM_RT	,(U8*)&gFctPara.rfSwitch1		,1,	NULL,},
	{ID_GET_SWITCH        	,0x02,PARAM_RT	,(U8*)&gFctPara.rfSwitch2		,1,	NULL,},
	{ID_GET_SWITCH        	,0x03,PARAM_RT	,(U8*)&gFctPara.rfSwitch3		,1,	NULL,},
	{ID_GET_SWITCH        	,0x04,PARAM_RT	,(U8*)&gFctPara.rfSwitch4		,1,	NULL,},
	{ID_GET_SWITCH        	,0x05,PARAM_RT	,(U8*)&gFctPara.rfSwitch5		,1,	NULL,},
 	
	{ID_GET_TEMP_CUR        ,0xFF,PARAM_RT	,(U8*)&gFctPara.curTemp			,2,	NULL,},
 	{ID_GET_TEMP_SCOPE      ,0xFF,PARAM_RTE	,(U8*)&gFctPara.tempLow			,4,	NULL,},
 	{ID_SET_TEMP_SCOPE_CUR  ,0xFF,PARAM_WTE	,(U8*)0							,4,	ProcSetTemperatorReset,},
	{ID_SET_TEMP_ALARM_VALUE,0xFF,PARAM_WTE	,(U8*)&gFctPara.tempAlarmLow	,4,	NULL,},
	{ID_GET_TEMP_ALARM_VALUE,0xFF,PARAM_RTE	,(U8*)&gFctPara.tempAlarmLow	,4,	NULL,},

 	{ID_GET_ALARM           ,0xFF,PARAM_RT	,(U8*)&gFctPara.alarm			,2,	NULL,},
 	{ID_ADDR_WD             ,0xFF,PARAM_WTE	,(U8*)&gFctPara.addr			,1,	NULL,},
 	{ID_ADDR_RD             ,0xFF,PARAM_RTE	,(U8*)&gFctPara.addr			,1,	NULL,},
 	{ID_RESET               ,0xFF,PARAM_WT	,(U8*)&gFctPara.reset			,1,	ProcSetReset	,},
// 	{ID_LOAD_START          ,0xFF,PARAM_WT	,(U8*)0							,0,	ProcSetLoadStart,},
// 	{ID_LOADING             ,0xFF,PARAM_WT	,(U8*)0							,0,	ProcSetLoading	,},
// 	{ID_LOAD_END            ,0xFF,PARAM_WT	,(U8*)0							,0,	ProcSetLoadEnd	,},
 	{ID_INQUIRE_STATE       ,0xFF,PARAM_RT	,(U8*)&gFctPara.upgrade			,6,	NULL,},
 	{ID_SET_INFO            ,0xFF,PARAM_WTE	,(U8*)&gFctPara.model			,40,NULL,},
 	{ID_GET_INFO            ,0xFF,PARAM_RTE	,(U8*)&gFctPara.model			,40,NULL,},
};

///
static U8 CmdTabCnt(void){return sizeof(tabCommand)/sizeof(STABLE_COMMAND);}
///
static void RFPOWER_SYC_5V_1(U8 x)		{RFPOWER_5V_1(x)	;gFctPara.power15   = x;}
static void RFPOWER_SYC_5V_2(U8 x)		{RFPOWER_5V_2(x)	;gFctPara.power25   = x;}
static void RFPOWER_SYC_9V_1(U8 x)		{RFPOWER_9V_1(x)	;gFctPara.power19   = x;}
static void RFPOWER_SYC_9V_2(U8 x)		{RFPOWER_9V_2(x)	;gFctPara.power29   = x;}

static void RFSWITCH_SYC_PATH1(U8 x) 	{RFSWITCH_PATH1(x)  ;gFctPara.rfSwitch1 = x;}
static void RFSWITCH_SYC_PATH2(U8 x) 	{RFSWITCH_PATH2(x)  ;gFctPara.rfSwitch2 = x;}
static void RFSWITCH_SYC_BYPASS1(U8 x)  {RFSWITCH_BYPASS1(x);gFctPara.rfSwitch3 = x;}
static void RFSWITCH_SYC_BYPASS2(U8 x)  {RFSWITCH_BYPASS2(x);gFctPara.rfSwitch4 = x;}
static void RFSWITCH_SYC_TEST(U8 x) 	{RFSWITCH_TEST(x)   ;gFctPara.rfSwitch5 = x;}

static void RFSWITCH_SYC_BYPASS(U8 x)	{RFSWITCH_SYC_BYPASS1(x);RFSWITCH_SYC_BYPASS2(x);}

static void RFSWITCH_SYC_PATH(U8 x)	
{								  
	RFPOWER_SYC_5V_1(FALSE);	  
	RFPOWER_SYC_5V_2(FALSE);      
	RFPOWER_SYC_9V_1(FALSE);	  
	RFPOWER_SYC_9V_2(FALSE);	  
	RFSWITCH_SYC_BYPASS(FALSE);	  
	if ( x == BYPASS )			  
	{							  
		RFSWITCH_SYC_BYPASS(TRUE);
	}							  
	else if ( x == PATH1 )		  
	{							  
		RFPOWER_SYC_5V_1(TRUE);	  
		RFPOWER_SYC_9V_1(TRUE);	  
		RFSWITCH_SYC_PATH1(1);	  
		RFSWITCH_SYC_PATH2(1);	  
	}							  
	else if ( x == PATH2 )		  
	{							  
		RFPOWER_SYC_5V_2(TRUE);	  
		RFPOWER_SYC_9V_2(TRUE);	  
		RFSWITCH_SYC_PATH1(2);	  
		RFSWITCH_SYC_PATH2(2);	  
	}							  
	else if ( x == HOT_STANDBY )  
	{							  
		RFPOWER_SYC_5V_1(TRUE);	  
		RFPOWER_SYC_9V_1(TRUE);	  
		RFPOWER_SYC_5V_2(TRUE);	  
		RFPOWER_SYC_9V_2(TRUE);	  
		RFSWITCH_SYC_PATH1(1);	  
		RFSWITCH_SYC_PATH2(1);	  
	}							  
}	

/**
  * @brief  :ADC Sample Filer
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static U16 ReadAdcValueFilter(U8 ch,U8 num)
{
	U8 	i;
	U32 adcSum = 0;
	U16 adcTmp = 0;

	for(i=0;i<num;i++)
	{
		if ( TRUE == ReadAdcValue(ch,&adcTmp) )
			adcSum += adcTmp;
	}
	return (U16)adcSum/i;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL StoreParamToEEprom(void*base,void*param,U8 len)
{
	U16 offset = (U32)param - (U32)base;
	return WriteE2prom(offset,param,len);
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ProcSwitchPath1(U8*buf,U16 len)
{
	RFSWITCH_SYC_PATH1(buf[1]);	
	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ProcSwitchPath2(U8*buf,U16 len)
{
	RFSWITCH_SYC_PATH2(buf[1]);	
	return TRUE;
}/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ProcSwitchBypass1(U8*buf,U16 len)
{
	RFSWITCH_SYC_BYPASS1(buf[1]);	
	return TRUE;
}/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ProcSwitchBypass2(U8*buf,U16 len)
{
	RFSWITCH_SYC_BYPASS2(buf[1]);	
	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ProcSwitchTest(U8*buf,U16 len)
{
	RFSWITCH_SYC_TEST(buf[1]);	
	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL	ProcSetMode(U8*buf,U16 len)
{
	U8 test,path;

	test = buf[0];
	path = buf[1];

	if ( test != 0 && test != 1 )return FALSE;
	if ( path != BYPASS && path != PATH1 && path != PATH2 && path != HOT_STANDBY )return FALSE;

	RFSWITCH_SYC_TEST(test);
	RFSWITCH_SYC_PATH(path);

//	if ( path == PATH1 ) gHotPath = TRUE;
//	if ( path == PATH2 ) gHotPath = FALSE;

	//store parameters
// 	StoreParamToEEprom(&gFctPara,&gFctPara.power15,sizeof(&gFctPara.power15));		
//	StoreParamToEEprom(&gFctPara,&gFctPara.power25,sizeof(&gFctPara.power25));	
//	StoreParamToEEprom(&gFctPara,&gFctPara.power19,sizeof(&gFctPara.power19));	
//	StoreParamToEEprom(&gFctPara,&gFctPara.power29,sizeof(&gFctPara.power29));
	
	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL	ProcSetPower1(U8*buf,U16 len)
{	
	RFPOWER_SYC_5V_1(buf[1]);
	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL	ProcSetPower2(U8*buf,U16 len)
{
	RFPOWER_SYC_5V_2(buf[1]);
	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL	ProcSetPower3(U8*buf,U16 len)
{
	RFPOWER_SYC_9V_1(buf[1]);
	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL	ProcSetPower4(U8*buf,U16 len)
{
	RFPOWER_SYC_9V_2(buf[1]);
	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL	ProcSetGain(U8*buf,U16 len)
{
	U8 id,gain;

	id = buf[0];
	gain = buf[1];

	if ( gain > ATT_VALUE_MAX )return FALSE;

	if ( id == 1 )
	{
		if (WritePe4302(&attBus[0],gain)==FALSE)return FALSE;	
	}
	else if ( id == 2 )
	{
		if (WritePe4302(&attBus[1],gain)==FALSE)return FALSE;	
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ProcSetTemperatorReset(U8*buf,U16 len)
{		
	gFctPara.tempHigh = gFctPara.tempLow = gFctPara.curTemp;	

	StoreParamToEEprom(&gFctPara,&gFctPara.tempLow ,sizeof(&gFctPara.tempLow ));
	StoreParamToEEprom(&gFctPara,&gFctPara.tempHigh,sizeof(&gFctPara.tempHigh));

	return TRUE;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL	ProcSetReset(U8*buf,U16 len)
{	
	RebootFlagSet();
	NVIC_SystemReset();
	return TRUE;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static void LoadConfigPara(void)
{
	U8 i;
	U16 offset;
	
	for ( i = 0 ; i < CmdTabCnt() ; i++ )
	{
		if ( tabCommand[i].type == PARAM_RTE )
		{
			offset = (U32)tabCommand[i].var - (U32)&gFctPara;				
			ReadE2prom(offset,tabCommand[i].var,tabCommand[i].varLen);		
		} 
	}	
}
/**													 
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ComResponsePcs(U8 cmdId,U8*buf,U16 len)
{	
	BOOL result = TRUE;
	U8 i,subId,*subData;

	subId = buf[0];	subData = &buf[1];
	
	for ( i = 0 ; i < CmdTabCnt() ; i++ )
	{
		if ( cmdId == tabCommand[i].cmd )
		{			
			if ( subId == tabCommand[i].sub || 0xFF == tabCommand[i].sub )
			{
				if ( tabCommand[i].proc != NULL )
				{
					if ( FALSE == tabCommand[i].proc(buf,len))
					{
						return FALSE;				
					}								
				}
			}
			
			if ( tabCommand[i].var == NULL )
			{
				continue;
			}

			if ( tabCommand[i].type == PARAM_RT || tabCommand[i].type == PARAM_RTE )
			{//read operation
				if ( tabCommand[i].sub == 0xFF)
				{
					memcpy(buf,tabCommand[i].var,tabCommand[i].varLen);														
				}	
				else
				{
					if ( subId == tabCommand[i].sub )
					{
						memcpy(subData,tabCommand[i].var,tabCommand[i].varLen);																			
					}
					else
					{
						continue;
					}
				}			
			}
			else if ( tabCommand[i].type == PARAM_WTE || tabCommand[i].type == PARAM_WT )
			{//write EEPROM operation
				if ( 0xFF == tabCommand[i].sub )
				{
					memcpy(tabCommand[i].var,buf,tabCommand[i].varLen);												
				}
				else
				{
					if ( subId == tabCommand[i].sub )
					{
						memcpy(tabCommand[i].var,subData,tabCommand[i].varLen);																			
					}
					else
					{
						continue;
					}	
				}
				//store the parameter
				if ( tabCommand[i].type == PARAM_WTE )
				{
					result = StoreParamToEEprom(&gFctPara,tabCommand[i].var,tabCommand[i].varLen);					
				}
			}												 
		}
	}
	
	modelAddr = gFctPara.addr;
		
	return result;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static void InitControl(void)
{
	int i;
	
	memset(mallocFct,0,sizeof(mallocFct));
	memset(mallocBuf,0,sizeof(mallocBuf));
			
	MAX9947_CONFIG(COM_BAUDRATE);
	LoadConfigPara();

 	modelAddr = gFctPara.addr;

	gFctPara.bootV		= BOOTLOADER_VERSION;
	gFctPara.softV		= SOFTWARE_VERSION;
	gFctPara.hardV		= HARDWARE_VERSION;
	gFctPara.checksum 	= CalcFirmwareAllCRC16();

	modelFwCrc = gFctPara.checksum;
	
	if ( gFctPara.test == 0xFF )gFctPara.test = 0x00;
	if ( gFctPara.path == 0xFF )gFctPara.path = 0x00;

	RFSWITCH_SW6(FALSE);
	for( i = 0 ; i < 200 ; i++ );
	
	RFPOWER_5V_1(gFctPara.power15);
	RFPOWER_5V_2(gFctPara.power25);
	RFPOWER_9V_1(gFctPara.power19);
	RFPOWER_9V_2(gFctPara.power29);

	RFSWITCH_SYC_TEST(gFctPara.test);
	RFSWITCH_SYC_PATH(gFctPara.path);

	WritePe4302(&attBus[0],gFctPara.attGain1);	
	WritePe4302(&attBus[1],gFctPara.attGain2);	
	
	return;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static void TaskSample(void)
{
#define SWAP_TEMP_CNT 10
//	U8 i,j;
//	S16 swapSum,swapTemp,swapBuf[SWAP_TEMP_CNT];
	U16 adv;
//	static U8 tPath,cPath;
//	static BOOL tempLs = TRUE,tempHs = TRUE;
//	static BOOL first = TRUE;
//	static S16 tempLow,s;
#define STATIC_tempLs		 			mallocBuf[0]
#define STATIC_tempHs		 			mallocBuf[1]
//#define STATIC_first		 		mallocBuf[2]
//#define STATIC_tempLow 		 (*(S16*)&mallocBuf[4])
//#define STATIC_tempHigh		 (*(S16*)&mallocBuf[6])
#define STATIC_hotTimerBak   (*(U16*)&mallocBuf[8])
#define STATIC_hotFlag		 		mallocBuf[12]
#define STATIC_hotTimer		 		(*(UserTimerDef*)&mallocBuf[16])
#define STATIC_sampleTimer 		mallocBuf[30]	

	/*Temperature Detection*/
	gFctPara.curTemp = 5*ReadExternalTemperatureSensor();  //temperature*10	
// 	for(i=0;i<SWAP_TEMP_CNT;i++)
// 	{
// 		swapBuf[i] = 5*ReadExternalTemperatureSensor();  //temperature*10		
// 	}	
// 	
// 	for(i=0;i<SWAP_TEMP_CNT;i++)
// 	{
// 		for(j=0;j<SWAP_TEMP_CNT-i-1;j++)
// 		{
// 			if ( swapBuf[j] < swapBuf[j+1] )
// 			{
// 				swapTemp = swapBuf[j];
// 				swapBuf[j] = swapBuf[j+1];
// 				swapBuf[j+1] = swapTemp;
// 			}
// 		}
// 	}		

// 	for(i=1,swapSum = 0;i<SWAP_TEMP_CNT-1;i++)
// 	{
// 		swapSum += swapBuf[i];
// 	}
// 	
// 	gFctPara.curTemp = swapSum/(SWAP_TEMP_CNT - 2);
	
	if ( gFctPara.curTemp >= -55*10 && gFctPara.curTemp <= 125*10 )
	{
		if ( gFctPara.tempLow > gFctPara.curTemp || (U16)gFctPara.tempLow == 0xFFFF )// || (U16)gFctPara.tempLow == 0xFB00 )//-128
		{
			gFctPara.tempLow = gFctPara.curTemp;
			StoreParamToEEprom(&gFctPara,&gFctPara.tempLow,sizeof(&gFctPara.tempLow));		
		}
		
		if ( gFctPara.tempHigh < gFctPara.curTemp || (U16)gFctPara.tempHigh == 0xFFFF )// || (U16)gFctPara.tempHigh == 0x04F6 )//127
		{
			gFctPara.tempHigh = gFctPara.curTemp;
			StoreParamToEEprom(&gFctPara,&gFctPara.tempHigh,sizeof(&gFctPara.tempHigh));		
		}	
	}

	//Temperature compensate
	if ( gFctPara.curTemp < -20 && STATIC_tempLs == FALSE )
	{
		STATIC_tempLs = TRUE;STATIC_tempHs = FALSE;
		WritePe4302(&attBus[0],gFctPara.attGain1 + 1);	
		WritePe4302(&attBus[1],gFctPara.attGain2 + 1);			
	}
	else if ( gFctPara.curTemp >= -20 && STATIC_tempHs == FALSE )
	{
		STATIC_tempHs = TRUE;STATIC_tempLs = FALSE;
		WritePe4302(&attBus[0],gFctPara.attGain1);
		WritePe4302(&attBus[1],gFctPara.attGain2);
	}

	/*Voltage Detection
	  V = 3.3*(adv/4096)*(RA/RL)*1000 mV
	  RA为总电阻，RL为分压电阻。*/
	adv = ReadAdcValueFilter(ADC_CHANNEL_5V,AD_SAMPLE_TIME);	
	gFctPara.vdc5V = 11*3300*adv/OUT_AD_MAX;

	adv = ReadAdcValueFilter(ADC_CHANNEL_9V,AD_SAMPLE_TIME);
	gFctPara.vdc9V = 11*3300*adv/OUT_AD_MAX;
	 
	adv = ReadAdcValueFilter(ADC_CHANNEL_12V,AD_SAMPLE_TIME);
	gFctPara.vdc12V = 11*3300*adv/OUT_AD_MAX;

	/*Current Detection
	  Vout = (Ishunt*Rshunt)*20*/
	adv = ReadAdcValueFilter(ADC_CHANNEL_5V_1,AD_SAMPLE_TIME);
	gFctPara.curDrain15 = 9900*adv/40960;  //mA

	adv = ReadAdcValueFilter(ADC_CHANNEL_5V_2,AD_SAMPLE_TIME);
	gFctPara.curDrain25 = 9900*adv/40960;  //mA

	adv = ReadAdcValueFilter(ADC_CHANNEL_9V_1,AD_SAMPLE_TIME);
	gFctPara.curDrain19 = 9900*adv/40960;  //mA

	adv = ReadAdcValueFilter(ADC_CHANNEL_9V_2,AD_SAMPLE_TIME);
	gFctPara.curDrain29 = 9900*adv/40960;  //mA
	
	//gFctPara.alarm = 0x0000;

	if ( STATIC_sampleTimer++ > 30 )   //300ms
	{
		STATIC_sampleTimer = 0;
		//12V
		if ( gFctPara.vdc12V < gFctPara.vdcLimitLow12 )
		{
			gFctPara.alarm |= ALARM_VOLTAGE_UNDER_12;	
		}
		else if ( gFctPara.vdc12V > gFctPara.vdcLimitHigh12 )
		{
			gFctPara.alarm |= ALARM_VOLTAGE_OVER_12;	
		}
		else
		{
			gFctPara.alarm &=~(ALARM_VOLTAGE_UNDER_12|ALARM_VOLTAGE_OVER_12);				
		}
		//9V
		if ( gFctPara.vdc9V < gFctPara.vdcLimitLow09 )
		{
			gFctPara.alarm |= ALARM_VOLTAGE_UNDER_09;	
		}
		else if ( gFctPara.vdc9V > gFctPara.vdcLimitHigh09 )
		{
			gFctPara.alarm |= ALARM_VOLTAGE_OVER_09;	
		}
		else
		{
			gFctPara.alarm &=~(ALARM_VOLTAGE_UNDER_09|ALARM_VOLTAGE_OVER_09);				
		}

		//5V
		if ( gFctPara.vdc5V < gFctPara.vdcLimitLow05 )
		{
			gFctPara.alarm |= ALARM_VOLTAGE_UNDER_05;	
		}
		else if ( gFctPara.vdc5V > gFctPara.vdcLimitHigh05 )
		{
			gFctPara.alarm |= ALARM_VOLTAGE_OVER_05;	
		}
		else
		{
			gFctPara.alarm &=~(ALARM_VOLTAGE_UNDER_05|ALARM_VOLTAGE_OVER_05);				
		}

		//1_5V
		if ( gFctPara.power15 == TRUE )
		{
			if ( gFctPara.curDrain15 < gFctPara.curLimitLow15 )
			{
				gFctPara.alarm |= ALARM_CURRENT_UNDER_15;
				//RFPOWER_SYC_5V_1(FALSE);	
			}
			else if ( gFctPara.curDrain15 > gFctPara.curLimitHigh15 )
			{
				gFctPara.alarm |= ALARM_CURRENT_OVER_15;
				//if (gFctPara.path == HOT_STANDBY) 
				RFPOWER_SYC_5V_1(FALSE);	
			}
			else
			{
				gFctPara.alarm &=~(ALARM_CURRENT_UNDER_15|ALARM_CURRENT_OVER_15);
			}
		}
		//2_5V
		if ( gFctPara.power25 == TRUE )
		{
			if ( gFctPara.curDrain25 < gFctPara.curLimitLow25 )
			{
				gFctPara.alarm |= ALARM_CURRENT_UNDER_25;
				//RFPOWER_SYC_5V_2(FALSE);	
			}
			else if ( gFctPara.curDrain25 > gFctPara.curLimitHigh25 )
			{
				gFctPara.alarm |= ALARM_CURRENT_OVER_25;
				//if (gFctPara.path == HOT_STANDBY) 
				RFPOWER_SYC_5V_2(FALSE);	
			}
			else
			{
				gFctPara.alarm &=~(ALARM_CURRENT_UNDER_25|ALARM_CURRENT_OVER_25);
			}
		}
		//1_9V
		if ( gFctPara.power19 == TRUE  )
		{
			if ( gFctPara.curDrain19 < gFctPara.curLimitLow19 )
			{
				gFctPara.alarm |= ALARM_CURRENT_UNDER_19;
				//RFPOWER_SYC_9V_1(FALSE);	
			}
			else if ( gFctPara.curDrain19 > gFctPara.curLimitHigh19 )
			{
				gFctPara.alarm |= ALARM_CURRENT_OVER_19;
				//if (gFctPara.path == HOT_STANDBY) 
				RFPOWER_SYC_9V_1(FALSE);	
			}
			else
			{
				gFctPara.alarm &=~(ALARM_CURRENT_UNDER_19|ALARM_CURRENT_OVER_19);
			}
		}
		//2_9V
		if ( gFctPara.power29 == TRUE )
		{
			if ( gFctPara.curDrain29 < gFctPara.curLimitLow29 )
			{
				gFctPara.alarm |= ALARM_CURRENT_UNDER_29;
				//RFPOWER_SYC_9V_2(FALSE);	
			}
			else if ( gFctPara.curDrain29 > gFctPara.curLimitHigh29 )
			{
				gFctPara.alarm |= ALARM_CURRENT_OVER_29;
				//if (gFctPara.path == HOT_STANDBY) 
				RFPOWER_SYC_9V_2(FALSE);	
			}
			else
			{
				gFctPara.alarm &=~(ALARM_CURRENT_UNDER_29|ALARM_CURRENT_OVER_29);
			}
		}

		//Temperature Alarm
		if ( gFctPara.curTemp < gFctPara.tempAlarmLow )
		{
			gFctPara.alarm |= ALARM_TEMPERATURE_LOW;
		}
		else if ( gFctPara.curTemp > gFctPara.tempAlarmHigh )
		{
			gFctPara.alarm |= ALARM_TEMPERATURE_HIGH;
		}
		else
		{
			gFctPara.alarm &=~(ALARM_TEMPERATURE_LOW|ALARM_TEMPERATURE_HIGH);
		}
	}

	//Hot Standby
	//a.PATH 1 = FAULT gives Switches to Path 2.
	//b.PATH 2 = FAULT gives Stays on Path 2.	
	//c.PATH 1 =OK gives Stays on Path 2 - It should revert to Path 1.
	if ( gFctPara.path == HOT_STANDBY && gFctPara.test == 0 )
	{		
		RFSWITCH_SYC_BYPASS(FALSE);		

		if ( STATIC_hotFlag == TRUE )
		{
			STATIC_hotFlag = FALSE;
			RFPOWER_SYC_5V_1(TRUE);
			RFPOWER_SYC_9V_1(TRUE);											
		}
			
		if ( (gFctPara.alarm & (ALARM_CURRENT_OVER_15|ALARM_CURRENT_UNDER_15|ALARM_CURRENT_OVER_19|ALARM_CURRENT_UNDER_19)) &&
			((gFctPara.alarm & (ALARM_CURRENT_OVER_25|ALARM_CURRENT_UNDER_25|ALARM_CURRENT_OVER_29|ALARM_CURRENT_UNDER_29)) == 0x0000) )
		{//path1 failed,path2 ok.
			STATIC_hotTimerBak = 0;
			STATIC_hotFlag = FALSE;				
//			UserTimerReset(TIM2,&STATIC_hotTimer);					
			RFSWITCH_SYC_PATH1(2);
			RFSWITCH_SYC_PATH2(2);
		}	
		else if ( (gFctPara.alarm & (ALARM_CURRENT_OVER_15|ALARM_CURRENT_UNDER_15|ALARM_CURRENT_OVER_19|ALARM_CURRENT_UNDER_19)) == 0x0000  )
		{//path1 ok.
			RFSWITCH_SYC_PATH1(1);
			RFSWITCH_SYC_PATH2(1);											
		}
		else if ( STATIC_hotTimerBak++ > 60*100 )		//60s
//		else if ( UserTimerOver(TIM2,&STATIC_hotTimer,USER_TIMER_1S(5)))
		{//path1 & path2 failed.
//			UserTimerReset(TIM2,&STATIC_hotTimer);
			STATIC_hotTimerBak = 0;				
			STATIC_hotFlag = TRUE;				
		}
	}

	if ( gFctPara.path == BYPASS )
		RFSWITCH_SW6(TRUE);
	else
		RFSWITCH_SW6(FALSE);

	return;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
int TaskControl(int*argv[],int argc) __attribute__((section("POINTER")));
int TaskControl(int*argv[],int argc)
{
	U8 cmdId,*buf;
	U16 len;

	if ( argc == 0x01 )
	{
		InitControl();		
	}
	else if ( argc == 0x03 )
	{
		cmdId = (U8 )*argv[0];	
		buf	  = (U8*)*argv[1];
		len   = (U16)*argv[2];
		return (int)ComResponsePcs(cmdId,buf,len);	
	}
	else if ( argc == 0x00 )
	{
		TaskSample();
	}
	
	return 0;					
}
