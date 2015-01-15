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
 ******************************************************************************/
#ifndef __RF_CONTROL_C__
#define __RF_CONTROL_C__
#endif
/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Private typedef -----------------------------------------------------------*/
#define TMA_NUM_OF_SUBUNIT			0x02
typedef struct 
{
	U8 mode;
	U8 funcFlag;					//Function flags
	U8 gain;
	U8*deviceData;
	U8 alarm;	
}STMA_INFO;

typedef struct
{
	char antModeNum[15];	        //0x01  15  ASCII       Antenna model number                                            
	char antSerNum[17];             //0x02  17  ASCI        Antenna serial number                                      
	U16  antOperBand;               //0x03  2   16-bit 		unsigned   Antenna operating band(s) *             I                                                    
	U16  beamWidth[4];              //0x04  8   4 x 16-bit  unsigned   Beamwidth for each operating band in band order (deg)                                                         
	U8	 gain[4];	                //0x05  4   4 x 8-bit   unsigned   Gain for each operating band in band order (dBi * 10)                     
	char installDate[6];	        //0x21  6   ASCII  		Installation date                                                                                                       
	char installerID[5];	        //0x22  5   ASCII  		Installer's ID                                                                                                                     
	char baseID[32];	            //0x23  32  ASCII  		Base station ID                                                                
	char sectorID[32];              //0x24  32  ASCII  		Sector ID                                                                                                          
	U16  antBear;	                //0x25  2   16-bit  	unsigned  Antenna bearing                                                                                           
	S16	 intalledTilt; 	            //0x26  2   16-bit  	signed  Installed mechanical tilt (degrees * 10)                           	                                  	                                  	                                  	                               	                              	                                  	
	//Assigned fields for additional data
	U8   subType;					//0x13  1  8-bit 	  	unsigned TMA subunit type (see table C.3) 
	U16  rxFreqBand[2];				//0x14  4  2 x 16-bit 	unsigned integer TMA subunit receive frequency band (see Table C.4)     
	U16  txFreqBand[2];				//0x15  4  2 x 16-bit 	unsigned integer TMA subunit transmit frequency band (see Table C.4)   
	U8	 gainMax;					//0x16  1  			  	Unsigned integer Maximum supported gain given as gain figure expressed in dB/4. 
	U8	 gainMin;					//0x17  1  			  	Unsigned integer Minimum supported gain given as gain figure expressed in dB/4. 
	U8	 gainRes;					//0x18  1  			  	Unsigned integer Gain resolution given as the gain resolution figure expressed in  dB/4
}SDEVICE_DATA;

typedef struct
{
	U8	fieldNo;
	U8 *data;	
	U8 	len;	
}SDEVICE_DATA_FIELD;
/* Private define ------------------------------------------------------------*/	                                                    	
#define TMA_NORMAL_MODE					0x00
#define TMA_BYPASS_MODE					0x01

#define TMA_SUBUNIT_BYPASS				0x01
#define TMA_SUBUNIT_VSWR				0x02

#define ADJUST_SUBUNIT_NUM(x)			(x-1)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
STMA_INFO gTMAinfo[TMA_NUM_OF_SUBUNIT];
SDEVICE_DATA gDevData[TMA_NUM_OF_SUBUNIT];
SDEVICE_DATA_FIELD const gDevDataTab[TMA_NUM_OF_SUBUNIT][17] = 
{
	{
		{0x01,(U8*)&gDevData[0].antModeNum[0]   ,sizeof(gDevData[0].antModeNum		),},
		{0x02,(U8*)&gDevData[0].antSerNum[0]    ,sizeof(gDevData[0].antSerNum      	),},
		{0x03,(U8*)&gDevData[0].antOperBand     ,sizeof(gDevData[0].antOperBand    	),},
		{0x04,(U8*)&gDevData[0].beamWidth[0]    ,sizeof(gDevData[0].beamWidth      	),},
		{0x05,(U8*)&gDevData[0].gain[0]	     	,sizeof(gDevData[0].gain	        ),},
		{0x21,(U8*)&gDevData[0].installDate[0]  ,sizeof(gDevData[0].installDate     ),},
		{0x22,(U8*)&gDevData[0].installerID[0]  ,sizeof(gDevData[0].installerID     ),},
		{0x23,(U8*)&gDevData[0].baseID[0]	    ,sizeof(gDevData[0].baseID          ),},
		{0x24,(U8*)&gDevData[0].sectorID[0]     ,sizeof(gDevData[0].sectorID        ),},
		{0x25,(U8*)&gDevData[0].antBear         ,sizeof(gDevData[0].antBear         ),},
		{0x26,(U8*)&gDevData[0].intalledTilt    ,sizeof(gDevData[0].intalledTilt    ),},
		{0x13,(U8*)&gDevData[0].subType		 	,sizeof(gDevData[0].subType		    ),},
		{0x14,(U8*)&gDevData[0].rxFreqBand[0]	,sizeof(gDevData[0].rxFreqBand	    ),},
		{0x15,(U8*)&gDevData[0].txFreqBand[0]	,sizeof(gDevData[0].txFreqBand		),},	
		{0x16,(U8*)&gDevData[0].gainMax		 	,sizeof(gDevData[0].gainMax		    ),},
		{0x17,(U8*)&gDevData[0].gainMin		 	,sizeof(gDevData[0].gainMin		    ),},
		{0x18,(U8*)&gDevData[0].gainRes		 	,sizeof(gDevData[0].gainRes			),},
	},
	{
		{0x01,(U8*)&gDevData[1].antModeNum[0]   ,sizeof(gDevData[1].antModeNum		),},
		{0x02,(U8*)&gDevData[1].antSerNum[0]    ,sizeof(gDevData[1].antSerNum      	),},
		{0x03,(U8*)&gDevData[1].antOperBand     ,sizeof(gDevData[1].antOperBand    	),},
		{0x04,(U8*)&gDevData[1].beamWidth[0]    ,sizeof(gDevData[1].beamWidth      	),},
		{0x05,(U8*)&gDevData[1].gain[0]	     	,sizeof(gDevData[1].gain	        ),},
		{0x21,(U8*)&gDevData[1].installDate[0]  ,sizeof(gDevData[1].installDate     ),},
		{0x22,(U8*)&gDevData[1].installerID[0]  ,sizeof(gDevData[1].installerID     ),},
		{0x23,(U8*)&gDevData[1].baseID[0]	    ,sizeof(gDevData[1].baseID          ),},
		{0x24,(U8*)&gDevData[1].sectorID[0]     ,sizeof(gDevData[1].sectorID        ),},
		{0x25,(U8*)&gDevData[1].antBear         ,sizeof(gDevData[1].antBear         ),},
		{0x26,(U8*)&gDevData[1].intalledTilt    ,sizeof(gDevData[1].intalledTilt    ),},
		{0x13,(U8*)&gDevData[1].subType		 	,sizeof(gDevData[1].subType		    ),},
		{0x14,(U8*)&gDevData[1].rxFreqBand[0]	,sizeof(gDevData[1].rxFreqBand	    ),},
		{0x15,(U8*)&gDevData[1].txFreqBand[0]	,sizeof(gDevData[1].txFreqBand		),},	
		{0x16,(U8*)&gDevData[1].gainMax		 	,sizeof(gDevData[1].gainMax		    ),},
		{0x17,(U8*)&gDevData[1].gainMin		 	,sizeof(gDevData[1].gainMin		    ),},
		{0x18,(U8*)&gDevData[1].gainRes		 	,sizeof(gDevData[1].gainRes			),},
	},
};
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcGetDeviceType(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
* @brief  :
* @param  :None
* @retval :None
* @author	:mashuai
* @version:v1.0
* @date	:2012.4.20
*/
static U16 V11ProcTMAMode(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcTMAGain(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcGetInfo(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcClearAlarms(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcAlarm(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcGetErrorStatus(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcEnableDevice(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcDisableDevice(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcSelfTest(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcGetSupportedBitRates(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcDeviceData(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcMemory(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcDownloadStart(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcStoreDataStart(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcStoreDataBlockSeg(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcDownloadEnd(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V11ProcSetBitRate(U8 setGet,U8*data,U16 rlen)
{
	return 0;
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V20ProcTMAMode(U8 setGet,U8*data,U16 rlen)
{
	U8 subNum,*retCode;
	U16 tlen = 0;

	if ( data == NULL )return 0;

	retCode = &data[1];

	subNum = ADJUST_SUBUNIT_NUM(data[0]);  //not adapter

	if ( subNum >= TMA_NUM_OF_SUBUNIT )return 0;

	if ( setGet == ACTION_GET )
	{//Get 
		*retCode = RETURN_3GPP_OK;
		data[2] = gTMAinfo[subNum].mode;		
		tlen = 3;
		/*
		FormatError 
		Busy 
		WorkingSoftwareMissing 					
		*/
	}
	else
	{//Set		
		if ( data[2] != TMA_NORMAL_MODE || data[2] != TMA_BYPASS_MODE )
		{
			*retCode = RETURN_3GPP_OutOfRange;		
		}
		else
		{
			gTMAinfo[subNum].mode = data[1];
			*retCode = RETURN_3GPP_OK;	
		}
		tlen = 2;
		/* 
		FormatError  
		Busy  
		HardwareError  
		WorkingSoftwareMissing 
		UnsupportedProcedure   
		OutOfRange    
		MajorTMAFault  
		MinorTMAFault
		*/
	}

	return tlen;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V20ProcTMAGetFunc(U8 setGet,U8*data,U16 rlen)
{
	U8 subNum,*retCode;
	U16 tlen = 0;

	if ( data == NULL )return 0;

	retCode = &data[1];

	subNum = ADJUST_SUBUNIT_NUM(data[0]);  //not adapter

	if ( subNum >= TMA_NUM_OF_SUBUNIT )return 0;

	if ( setGet == ACTION_GET )
	{//Get 
		*retCode = RETURN_3GPP_OK;
		data[2] = gTMAinfo[subNum].funcFlag;
		data[3] = gDevData[subNum].gainMin;
		data[4] = gDevData[subNum].gainMax;
		data[5] = gDevData[subNum].gainRes;		
		tlen = 6;
		/*
		FormatError 
		Busy 
		WorkingSoftwareMissing 
		*/			
	}

	return tlen;
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V20ProcTMAGain(U8 setGet,U8*data,U16 rlen)
{
	U8 subNum,*retCode;
	U16 tlen = 0;

	if ( data == NULL )return 0;

	retCode = &data[1];

	subNum = ADJUST_SUBUNIT_NUM(data[0]);  //not adapter

	if ( subNum >= TMA_NUM_OF_SUBUNIT )return 0;

	if ( setGet == ACTION_GET )
	{//Get 
		*retCode = RETURN_3GPP_OK;
		data[2] = gTMAinfo[subNum].gain;		
		tlen = 3;
		/*
		FormatError 
		Busy 
		WorkingSoftwareMissing  
		MajorTMAFault 
		MinorTMAFault
		BypassMode 							
		*/
	}
	else
	{//Set		
		if ( data[2] != TMA_NORMAL_MODE || data[2] != TMA_BYPASS_MODE )
		{
			*retCode = RETURN_3GPP_OutOfRange;		
		}
		else
		{
			gTMAinfo[subNum].gain = data[1];
			*retCode = RETURN_3GPP_OK;	
		}
		tlen = 2;
		/* 
		FormatError 
		Busy 
		HardwareError 
		WorkingSoftwareMissing 
		UnsupportedProcedure 
		UnsupportedValue  
		MajorTMAFault  
		MinorTMAFault 
		*/
	}

	return tlen;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V20ProcTMADevData(U8 setGet,U8*data,U16 rlen)
{
	U8 i,subNum,*retCode;
	U16 tlen = 0;

	if ( data == NULL )return 0;

	retCode = &data[1];

	subNum = ADJUST_SUBUNIT_NUM(data[0]);  //not adapter

	if ( subNum >= TMA_NUM_OF_SUBUNIT )return 0;

	for ( i = 0 ; i < sizeof(gDevDataTab[subNum])/sizeof(SDEVICE_DATA_FIELD) ; i++ )
	{
		if ( data[1] == gDevDataTab[subNum][i].fieldNo )
		{
			if ( setGet == ACTION_GET )
			{//Get
				memcpy(&data[2],gDevDataTab[subNum][i].data,gDevDataTab[subNum][i].len);
				tlen = 2+gDevDataTab[subNum][i].len;				
				*retCode = RETURN_3GPP_OK;						
				/*
				FormatError 
				Busy 
				WorkingSoftwareMissing 
				UnknownParameter  					
				*/
			}
			else
			{//Set
				if ( gDevDataTab[subNum][i].len != rlen - 2 )
				{
					//FormatError
				}
				else
				{		
					memcpy(gDevDataTab[subNum][i].data,&data[2],gDevDataTab[subNum][i].len);
					*retCode = RETURN_3GPP_OK;
				}
				tlen = 2;
				/* 				  
				FormatError 
				Busy 
				HardwareError 
				WorkingSoftwareMissing 
				ReadOnly 
				UnknownParameter 
				*/		
			}
			break; 	
		}
	}
	return tlen;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V20ProcTMAClearAlarm(U8 setGet,U8*data,U16 rlen)
{
	U8 subNum,*retCode;
	U16 tlen = 0;

	if ( data == NULL )return 0;

	retCode = &data[1];

	subNum = ADJUST_SUBUNIT_NUM(data[0]);  //not adapter

	if ( subNum >= TMA_NUM_OF_SUBUNIT )return 0;

	if ( setGet == ACTION_SET )
	{//Get
		//Clear
		 
		*retCode = RETURN_3GPP_OK;	
		tlen = 2;
		/*
		FormatError 
		Busy 
		WorkingSoftwareMissing 
		*/			
	}

	return tlen;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V20ProcTMAGetAlarm(U8 setGet,U8*data,U16 rlen)
{
	U8 subNum,*retCode;
	U16 tlen = 0;

	if ( data == NULL )return 0;

	retCode = &data[1];

	subNum = ADJUST_SUBUNIT_NUM(data[0]);  //not adapter

	if ( subNum >= TMA_NUM_OF_SUBUNIT )return 0;

	if ( setGet == ACTION_GET )
	{//Get
		//Clear
		 
		*retCode = RETURN_3GPP_OK;	
		//tlen = i;
		/*
		FormatError 
		Busy 
		WorkingSoftwareMissing 
		*/			
	}

	return tlen;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V20ProcTMAGetNumOfUnit(U8 setGet,U8*data,U16 rlen)
{
	U8 *retCode;
	U16 tlen = 0;

	if ( data == NULL )return 0;

	retCode = &data[0];

	if ( setGet == ACTION_GET )
	{//Get 
		if ( rlen != 0 )
		{
			*retCode = RETURN_3GPP_FormatError;
		}
		else
		{
			*retCode = RETURN_3GPP_OK;
			data[1] = TMA_NUM_OF_SUBUNIT;		
		}
		tlen = 2;			
	}
	//Busy 
	//WorkingSoftwareMissing
	return tlen;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V20ProcUserRead(U8 setGet,U8*data,U16 rlen)
{
	U8 *retCode;
	U16 tlen = 0;

	if ( data == NULL )return 0;

	retCode = &data[0];

	if ( setGet == ACTION_GET )
	{//Get 
		if ( rlen != 0 )
		{
			*retCode = RETURN_3GPP_FormatError;
		}
		else
		{
			*retCode = RETURN_3GPP_OK;
			data[1] = TMA_NUM_OF_SUBUNIT;		
		}
		tlen = 2;			
	}
	//Busy 
	//WorkingSoftwareMissing
	return tlen;	
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 V20ProcUserWrite(U8 setGet,U8*data,U16 rlen)
{
	U8 *retCode;
	U16 tlen = 0;

	if ( data == NULL )return 0;

	retCode = &data[0];

	if ( setGet == ACTION_GET )
	{//Get 
		if ( rlen != 0 )
		{
			*retCode = RETURN_3GPP_FormatError;
		}
		else
		{
			*retCode = RETURN_3GPP_OK;
			data[1] = TMA_NUM_OF_SUBUNIT;		
		}
		tlen = 2;			
	}
	//Busy 
	//WorkingSoftwareMissing
	return tlen;	
}

//Lookup the AISG infomation
const SLAYER7_AISG gLayer7AISGV11[] = 
{	
	{AISG_V11_GLOBAL_Reserved       	 ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,NULL,}, 
	{AISG_V11_GLOBAL_GetDeviceType       ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V11ProcGetDeviceType,},
	{AISG_V11_GLOBAL_Reset               ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,NULL,},
	{AISG_V11_GLOBAL_GetErrorStatus      ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V11ProcGetErrorStatus,},
	{AISG_V11_GLOBAL_GetInfo             ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V11ProcGetInfo,},
	{AISG_V11_GLOBAL_ClearAlarms         ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V11ProcClearAlarms,},
	{AISG_V11_GLOBAL_Alarm               ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V11ProcAlarm,},
	{AISG_V11_GLOBAL_EnableDevice        ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V11ProcEnableDevice,},
	{AISG_V11_GLOBAL_DisableDevice 		 ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V11ProcDisableDevice,},
	{AISG_V11_GLOBAL_SelfTest            ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V11ProcSelfTest,},
	{AISG_V11_GLOBAL_GetSupportedBitRates,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V11ProcGetSupportedBitRates,},
	{AISG_V11_GLOBAL_SetDeviceData       ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V11ProcDeviceData,},
	{AISG_V11_GLOBAL_GetDeviceData       ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V11ProcDeviceData,},
	{AISG_V11_OPTIONAL_ReadMemory        ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V11ProcMemory,},
	{AISG_V11_OPTIONAL_WriteMemory       ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V11ProcMemory,},
	{AISG_V11_OPTIONAL_DownloadStart     ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_DOWNLOAD	,NA			,LENGTH_FIX,ACTION_SET,V11ProcDownloadStart,},
	{AISG_V11_OPTIONAL_StoreDataStart    ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_DOWNLOAD	,NA			,LENGTH_FIX,ACTION_SET,V11ProcStoreDataStart,},
	{AISG_V11_OPTIONAL_StoreDataBlockSeg ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_DOWNLOAD	,NA			,LENGTH_FIX,ACTION_SET,V11ProcStoreDataBlockSeg,},
	{AISG_V11_OPTIONAL_DownloadEnd       ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_DOWNLOAD	,NA			,LENGTH_FIX,ACTION_SET,V11ProcDownloadEnd,},
	{AISG_V11_OPTIONAL_SetBitRate        ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V11ProcSetBitRate,},
	{AISG_V11_TMA_SetMode  				 ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V11ProcTMAMode,},
	{AISG_V11_TMA_GetMode  				 ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V11ProcTMAMode,},
	{AISG_V11_TMA_SetGain  				 ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V11ProcTMAGain,},
	{AISG_V11_TMA_GetGain  				 ,PRIMARY_DEVICE  ,NA		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V11ProcTMAGain,},			
};
U8	NumItemOfAISG11(void){return sizeof(gLayer7AISGV11)/sizeof(SLAYER7_AISG);};

const SLAYER7_AISG gLayer7AISGV20[] = 
{			
	{ID_3GPP_Reserved		          	 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,POWER_LOW	,LENGTH_FIX,ACTION_SET,NULL,},
	{ID_3GPP_ResetSoftware          	 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_DOWNLOAD	,POWER_LOW	,LENGTH_FIX,ACTION_SET,NULL,},
	{ID_3GPP_GetAlarmStatus         	 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,POWER_LOW	,LENGTH_VAR,ACTION_GET,NULL,},
	{ID_3GPP_GetInformation         	 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_DOWNLOAD	,POWER_LOW	,LENGTH_FIX,ACTION_GET,NULL,},
	{ID_3GPP_ClearActiveAlarms      	 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,POWER_LOW	,LENGTH_FIX,ACTION_SET,NULL,},
	{ID_3GPP_AlarmSubscribe         	 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,POWER_LOW	,LENGTH_FIX,ACTION_GET,NULL,},
	{ID_3GPP_ReadUserData           	 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,POWER_LOW	,LENGTH_FIX,ACTION_GET,V20ProcUserRead			,},
	{ID_3GPP_WriteUserData          	 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,POWER_LOW	,LENGTH_FIX,ACTION_SET,V20ProcUserWrite			,},
	{ID_3GPP_SelfTest               	 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,POWER_HIGH	,LENGTH_VAR,ACTION_SET,NULL,},
	{ID_3GPP_DownloadStart          	 ,PRIMARY_DEVICE  ,OPTIONAL		,PROC_CLASS1,MODE_DOWNLOAD	,POWER_LOW	,LENGTH_FIX,ACTION_SET,NULL,},
	{ID_3GPP_DownloadApplication    	 ,PRIMARY_DEVICE  ,OPTIONAL		,PROC_CLASS1,MODE_DOWNLOAD	,POWER_LOW	,LENGTH_VAR,ACTION_SET,NULL,},
	{ID_3GPP_DownloadEnd            	 ,PRIMARY_DEVICE  ,OPTIONAL		,PROC_CLASS1,MODE_DOWNLOAD	,POWER_LOW	,LENGTH_FIX,ACTION_SET,NULL,},
	{ID_3GPP_Vendorspecificprocedure	 ,PRIMARY_DEVICE  ,OPTIONAL		,PROC_CLASS1,MODE_NORMAL	,POWER_LOW	,LENGTH_VAR,ACTION_GET,NULL,},
	{AISG_V20_TMASetMode                 ,PRIMARY_DEVICE  ,OPTIONAL		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V20ProcTMAMode			,},
    {AISG_V20_TMAGetMode                 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V20ProcTMAMode			,},
    {AISG_V20_TMAGetSupportedFunctions   ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V20ProcTMAGetFunc		,},
    {AISG_V20_TMASetGain                 ,PRIMARY_DEVICE  ,OPTIONAL		,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V20ProcTMAGain			,},
    {AISG_V20_TMAGetGain                 ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V20ProcTMAGain			,},
    {AISG_V20_TMASetDeviceData           ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V20ProcTMADevData		,},
    {AISG_V20_TMAGetDeviceData           ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V20ProcTMADevData		,},
    {AISG_V20_TMAAlarmIndication         ,SECONDARY_DEVICE,MANDATORY	,PROC_CLASS2,MODE_NORMAL	,NA			,LENGTH_VAR,ACTION_GET,NULL,},
    {AISG_V20_TMAClearActiveAlarms       ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_SET,V20ProcTMAClearAlarm		,},
    {AISG_V20_TMAGetAlarmStatus          ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_VAR,ACTION_GET,V20ProcTMAGetAlarm		,},
    {AISG_V20_TMAGetNumberOfSubunits     ,PRIMARY_DEVICE  ,MANDATORY	,PROC_CLASS1,MODE_NORMAL	,NA			,LENGTH_FIX,ACTION_GET,V20ProcTMAGetNumOfUnit	,},
};
U8	NumItemOfAISG20(void){return sizeof(gLayer7AISGV20)/sizeof(SLAYER7_AISG);};
/********************************END OF FILE***********************************/
