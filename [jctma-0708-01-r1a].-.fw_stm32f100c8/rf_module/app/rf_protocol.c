/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:rf_protocol.c
 * @author	:mashuai
 * @version	:v1.0 
 * @date	:2012.4.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __RF_PROTOCOL_C__
#define __RF_PROTOCOL_C__
#endif
/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct 
{
	U8 flag;
	U8 mode;
	U8 addr;
	U8 cmd;
	U8 ack;
	U8 para[1];	
}SLAYER2;
/* Private define ------------------------------------------------------------*/
#define MODE_Nocheck					0x00
#define MODE_UseChecksum				0x06
#define MODE_UseCRC						0x09

#define ACK_Success                		0x00	
#define ACK_IllegalCheckAlgorithm		0x01	
#define ACK_CheckError            		0x02	
#define ACK_IllegalSubunit        		0x03	
#define ACK_IllegalCommand        		0x04	
#define ACK_IllegalACKSymbol     		0x05	
#define ACK_IllegalDataLength    		0x06	
#define ACK_IllegalParameter      		0x07	
#define ACK_Fault                  		0x08
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static U16 loadFwCnt;
/* Private function prototypes -----------------------------------------------*/
static BOOL ProcSetLoadStart(U8*buf,U16 len);
static BOOL ProcSetLoading(U8*buf,U16 len);
static BOOL ProcSetLoadingPrev(U8*buf,U16 len);
static BOOL ProcSetLoadEnd(U8*buf,U16 len);
static BOOL ProcSetLoadEndPrev(U8*buf,U16 len);
/**
  * @brief  :半字节查表法之CRC16
  			 CRC16-CCITT 
			 polynomial:x16+x12+x5+1  1021      
			 references:ISO HDLC, ITU X.25, V.34/V.41/V.42, PPP-FCS
  * @param  :
  * @retval :
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
U16	CalcCRC16Value(U8 *ptr,U16 len)
{ 
	U16	 const crc16Table[16]={    
		0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7, 
		0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef, 
	}; 	
	U16  crc; 
	U8   da; 
	 
	for(crc = 0;len--!=0;ptr++)  { 
		da = crc >> 12;    
		crc <<=4;    
		crc^=crc16Table[da^(*ptr>>4)];    
		da = crc >> 12;    
		crc <<=4;    
		crc^= crc16Table[da^(*ptr&0x0f)];    
	} 

	return(crc); 
}
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  :
  			references 3GPP 25462-651 A.5 Option 15.1
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 HdlcConvertData(U8 type,U8 ptr[],U16 len)
{
	U8 buf[512];	
	U16 i,j;

	for(i=0;i<len;i++)buf[i]=ptr[i];

	if ( type == 0x00 )
	{
		for (i=0,j=0;j<len;i++,j++)
		{
			if ( buf[i] == 0x7E && i!=0 && j!=len-1 )
			{
				ptr[j] = 0x7D; 
				j++,len++;
				ptr[j] = 0x5E;
			}
			else if ( buf[i] == 0x7D)
			{
				ptr[j] = 0x7D; 
				j++,len++;
				ptr[j] = 0x5D;				
			}
			else
			{
				ptr[j] = buf[i];
			}
		}		
	}
	else if ( type == 0x01 )
	{
		for (i=0,j=0;j<len;i++,j++)
		{
			if ( buf[i] == 0x7D && buf[i+1] == 0x5E )
			{
				ptr[j] = 0x7E;i++,len--;
			}
			else if ( buf[i] == 0x7D && buf[i+1] == 0x5D )
			{
				ptr[j] = 0x7D;i++,len--;	
			}	
			else
			{
				ptr[j] = buf[i];
			}	
		}
	}

	return len;		
}	
/**
  * @brief  :Return the upgrade flag
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static void ReturnUpgradeFlag(U8 port)
{
	U8 *buf;
	U16 len,crc16;

	/*在应用编程完成以后，MCU重启后向主机发送确认消息*/
 	GetUartBufInfo(port,(U8**)&buf,NULL);

	buf[0] = 0x7E;
	buf[1] = MODE_UseCRC;
	buf[2] = modelAddr;
	buf[3] = ID_LOAD_END;
	buf[4] = ACK_Success;
	buf[5] = 6;
	buf[6] = (U8)modelFwCrc;
	buf[7] = modelFwCrc>>8;
	buf[8] = 0;
	buf[9] = 0;
	buf[10] = 0;
	buf[11] = 0;

	crc16 = CalcCRC16Value(&buf[1],11);

	buf[12] = crc16;
	buf[13] = crc16/256;
	buf[14] = 0x7E;
		
	len = HdlcConvertData(0x00,buf,15);		
	UartTxOpen(port,len);
}
/**
  * @brief  :Return the upgrade flag
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static void ReturnResetFlag(U8 port)
{
	U8 *buf;
	U16 len,crc16;

 	GetUartBufInfo(port,(U8**)&buf,NULL);

	buf[0] = 0x7E;
	buf[1] = MODE_UseCRC;
	buf[2] = modelAddr;
	buf[3] = ID_RESET;
	buf[4] = ACK_Success;
	buf[5] = 6;
	buf[6] = 0;
	buf[7] = 0;
	buf[8] = 0;
	buf[9] = 0;
	buf[10] = 0;
	buf[11] = 0;

	crc16 = CalcCRC16Value(&buf[1],11);

	buf[12] = crc16;
	buf[13] = crc16/256;
	buf[14] = 0x7E;
		
	len = HdlcConvertData(0x00,buf,15);		
	UartTxOpen(port,len);
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static void TMAPktHandle(U8 port)
{
	SLAYER2 *layer2;
	U8 *buf;
	U16 len,crc16;
	int *argv[3],tmpBuf,tmpCmd,tmpLen;
	UserTimerDef tDelay;

	GetUartBufInfo(port,(U8**)&buf,&len);
	
	if ( len > 6 && buf[0] == 0x7E && buf[len-1] == 0x7E )
	{
		layer2 = (SLAYER2*)buf;
		len = HdlcConvertData(0x01,buf,len);		

		if ( layer2->addr != modelAddr && layer2->addr != 0x00 )
		{
			ResetUartBuf(port);
			return;
		}
				
		layer2->ack	= ACK_Success;

		if ( layer2->mode != MODE_Nocheck && layer2->mode != MODE_UseCRC && layer2->mode != MODE_UseChecksum )
		{
			layer2->ack = ACK_IllegalCheckAlgorithm;
			goto PACKET2CONTROL;		
		}

		if ( layer2->mode == MODE_UseCRC )
		{
			crc16 = buf[len-2]*256 + buf[len-3];

			if ( crc16 != CalcCRC16Value(&buf[1],len-4) )
			{
				layer2->ack = ACK_CheckError;
				goto PACKET2CONTROL; 
			}
		}

		if ( layer2->para[0] + sizeof(SLAYER2) + 3 != len )
		{
			layer2->ack = ACK_IllegalDataLength;
			goto PACKET2CONTROL;	
		}

		tmpBuf = (U32)&layer2->para[1];
		tmpCmd = layer2->cmd;
		tmpLen = layer2->para[0];

		argv[0] = (int*)&tmpCmd;
		argv[1] = (int*)&tmpBuf;
		argv[2] = (int*)&tmpLen;		

		//firmware upgrade
		switch (layer2->cmd)
		{
			case ID_LOAD_START:
			case ID_UPGRADEALL_START:
				if ( FALSE == ProcSetLoadStart(&layer2->para[1],layer2->para[0]) )
				{
					layer2->ack = ACK_IllegalParameter;
				}
				break;
			case ID_LOADING:
				if ( FALSE == ProcSetLoadingPrev(&layer2->para[1],layer2->para[0]))
				{
					layer2->ack = ACK_IllegalParameter;
				}
				break;				
			case ID_UPGRADEALL_DOING:
				if ( FALSE == ProcSetLoading(&layer2->para[1],layer2->para[0]))
				{
					layer2->ack = ACK_IllegalParameter;
				}
				break;
			case ID_LOAD_END:
				if ( FALSE == ProcSetLoadEndPrev(&layer2->para[1],layer2->para[0]))
				{
					layer2->ack = ACK_IllegalParameter;
				}
				break;
			case ID_UPGRADEALL_END:
				if ( FALSE == ProcSetLoadEnd(&layer2->para[1],layer2->para[0]))
				{
					layer2->ack = ACK_IllegalParameter;
				}
				break;
			default:
				if ( (BOOL)TaskControlEnter(argv,3) == FALSE )
				{
					layer2->ack = ACK_IllegalParameter;				
				}
				break;
		}	

		if ( layer2->cmd == ID_LOADING || ID_UPGRADEALL_DOING == layer2->cmd )
		{
			layer2->para[0] = 6;
			len = sizeof(SLAYER2) + 6 + 2 + 1; // + parameter`s length + crc16`s length + flag`length		
		}			

		goto PACKET2CONTROL;
	}

	return;

PACKET2CONTROL:
	crc16 = CalcCRC16Value(&buf[1],len - 4);

	buf[len - 1] = 0x7E;
	buf[len - 2] = crc16/256;
	buf[len - 3] = crc16;

	len = HdlcConvertData(0x00,buf,len);

	for ( UserTimerReset(TASK_TIM_HANDLE,&tDelay);
		  UserTimerOver(TASK_TIM_HANDLE,&tDelay,USER_TIMER_1MS(5))!=TRUE;);

	UartTxOpen(port,len);			
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ProcSetLoadStart(U8*buf,U16 len)
{
	loadFwCnt = 0;
	return InitCode2Flash();
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ProcSetLoading(U8*buf,U16 len)
{	
	BOOL result;

	result = Code2Flash(buf,len);
	
	if ( result == TRUE )
	{
		loadFwCnt += len;
		buf[0] = (U8)loadFwCnt;
		buf[1] = loadFwCnt/256;
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
		buf[5] = 0;
	}
	
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
static BOOL ProcSetLoadingPrev(U8*buf,U16 len)
{	
	BOOL result;

	if ( (len + loadFwCnt) > SIZEOF_BUFFER )
	{
		len = SIZEOF_BUFFER - loadFwCnt;		
	}

	if ( len == 0 )return FALSE;	

	result = Code2Flash(buf,len);
	
	if ( result == TRUE )
	{
		loadFwCnt += len;
		buf[0] = (U8)loadFwCnt;
		buf[1] = loadFwCnt/256;
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
		buf[5] = 0;
	}
	
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
static BOOL ProcSetLoadEndPrev(U8*buf,U16 len)
{
	U16 crc16;

	crc16 = *(U16*)buf;

	//hardware version is consistency
	if ( HARDWARE_VERSION != HARDWARE_VERSION_BAK )return FALSE;
	//UpgradeAll version is consistency
	if ( (SOFTWARE_VERSION&0xF0) != (SOFTWARE_VERSION_BAK&0xF0) )return FALSE;

	if ( modelFwCrc == crc16 ) return TRUE;

	if( crc16 != CalcFirmwareBufCRC16())return FALSE;

	return EndCode2FlashRrev();
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static BOOL ProcSetLoadEnd(U8*buf,U16 len)
{
	U16 crc16;

	crc16 = *(U16*)buf;

	//hardware version is consistency
	if ( HARDWARE_VERSION != HARDWARE_VERSION_BAK )return FALSE;

	if ( modelFwCrc == crc16 ) return TRUE;

	if( crc16 != CalcFirmwareBufCRC16())return FALSE;
	
	return EndCode2Flash();
}
/**
  * @brief  :Init the protocol
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
void InitProtocol(void)
{	
	InitUartBuf(UART1);
	InitUartBuf(UART2);

	if ( TRUE == RebootFlagCheck() )
	{
		ReturnResetFlag(UART1);
		ReturnResetFlag(UART2);
	}
	if ( TRUE == DownFlagCheck() )
	{
		ReturnUpgradeFlag(UART1);		
		ReturnUpgradeFlag(UART2);		
	}
}
/**
  * @brief  :protocol task routine
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
int TaskProtocol(int*argv[],int argc)
{
	TMAPktHandle(UART1);	
	TMAPktHandle(UART2);
	return 0;
}
/********************************END OF FILE***********************************/
