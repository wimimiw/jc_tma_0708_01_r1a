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
//AISG VERSION
typedef U8 AISG_VERSION;
//HDLC-Frame
typedef union 
{
	U8 buf[1];
	struct
	{
		U8 flag;		//the synchronous flag
		U8 addr;		//the address of the secondary station
		U8 ctl;			//the control field of the I-Frame
		U8 info[1];		//the INFO-field of the I-Frame
	}member;		
}UHdlcFrame,UPktLayer2;
//I format
typedef union
{
	U8 val;
	struct
	{
		U8 ID:1;
		U8 NS:3;
		U8 PF:1;
		U8 NR:3;	
	}meb;
}UIControl;
//S format
//U format
//HDLC-parameters
typedef struct
{	
	U32 bitLenTx;		//Maximum information field length - transmit (bits)
	U32 bitLenRx;		//Maximum information field length - receive (bits)
	U8	wndSizeTx;		//Window size - transmit (frames)
	U8	wndSizeRx;		//Window size - receive (frames)

	U8  vesn3GPP;		//3GPP Release ID
	U8	vesnAISG;

	U8	uniqueID[20];	//len(Byte) + Unique ID	  i.e 0x02 0x55 0x66
	U8	addr;			//HDLC Address
	//U8	bitMask[19];	//Bit Mask (for Unique ID), indicates a device scan
	U8	devType;		//Device Type (see table 4.7.1)
	U8	vndCode[2];		//Vendor Code as given in [4]
}SHdlcParam;
//layer7 data temporary
typedef struct
{
	U8 	flag;			//the Layer7 is valid
	U16 rLen;			//the receive length of the layer7
	U8 *info;			//the poniter of the layer7 buffer
}SPktLayer7;
/* Private define ------------------------------------------------------------*/
#define HDLC_PARA_RO			0
#define HDLC_PARA_WR			1
#define HDLC_PARA_CMP			2

#define DEFAULT_ADDRESS			0x00
#define BROADCAST_ADDRESS		0xFF

#define HDLC_FRAME_FLAG			(0x7E )
#define HDLC_FRAME_TX			0
#define HDLC_FRAME_RX			1
#define HDLC_FRAME_LEN_MIN		(4 + 2)
#define HDLC_FRAME_LEN_SUPPORT  (240+2)

#define FI						0x81	//Format identifier
#define GI_HDLC					0x80	//HDLC Parameters set
#define GI_USER					0xf0	//User defined parameter set
//Parameter ID
//State model
#define STATE_MODEL
//Layer2 state model
#define NO_ADDRESS				0
#define ADDRESS_ASSIGNED		1
#define CONNECTED				2 
//XID
#define XID						0xBF	   //P/F = 1
//NRM ??????????????????????????????
//#define UNC_I					N(R)  P/F N (S) 0
//#define UNC_RR				N(R)  P/F 0 0 0 1
//#define UNC_RNR				N(R)  P/F 0 1 0 1				1
//reference the ISO/IEC 13239:2002 5.5.3.3
#define UNC_CMD_SNRM			0x93	   //NRM(P = 1)
#define UNC_CMD_DISC		   	0x53	   //NRM(P = 1)
//reference the ISO/IEC 13239:2002 5.5.3.4
#define UNC_RES_UA				0x73	   //NRM(F = 1)
#define UNC_RES_DM				0x1F	   //NRM(F = 1)
#define UNC_RES_FRMR			0x87	   //NRM(F = 1)

#define POLL_BIT				( 1<<4 )
#define FINAL_BIT				( 1<<4 )

#define WITHOUT_INFO_LEN		0

#define IS_I_FRAME(B)			(!((B)&0x01))
#define IS_S_FRAME(B)			( ((B)&0x03) == 0x01 )
#define IS_U_FRAME(B)			( ((B)&0x03) == 0x03 )

#define IS_RR(B)				( ((B)&0x0C) == 0x00 )
#define IS_RNR(B)				( ((B)&0x0C) == 0x04 )
//Device Types
#define TMA_DEVICE				0x02 
#define SINGLE_ANT_RET_DEVICE	0x01
#define MULTI_ANT_RET_DEVICE	0x11
//AISG version 2.0 extend
#define AISG_V11
#define AISG_V20
#define PI_AISG_V20				 (20)
#define PI_AISG_V20_DEV_TYPE_SUB (21)
//min&max
#define MIN(a,b)	((a)<(b)?(a):(b))
#define MAX(a,b)	((a)>(b)?(a):(b))
//user
#define DIS_RESPONSE			0xffff
//version
#define AISG11_VERSION			0x11
#define AISG20_VERSION			0x20
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SHdlcParam gHdlcParam;
AISG_VERSION aisgVersion;
/* Private function prototypes -----------------------------------------------*/
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
	U8 buf[HDLC_FRAME_LEN_SUPPORT];	
	U16 i,j;

	for(i=0;i<len;i++)buf[i]=ptr[i];

	if ( type == HDLC_FRAME_TX )
	{
		for (i=0,j=0;j<len;i++,j++)
		{
			if ( buf[i] == HDLC_FRAME_FLAG && i!=0 && j!=len-1 )
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
	else if ( type == HDLC_FRAME_RX )
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
  * @brief  :接收第一层数据包任务
  			:reference the 3GPP 25462-651
  * @param  :None
  * @retval :接收到的数据包指针
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static U16 Layer2Unpack( U8 port , SPktLayer7 *const layer7)
{
	BOOL scanFlag = FALSE;
	U8 tFI,tGI,tGL,tPI,tPL,*pPV;
	U8 addr,ctl;	
	U16 i,len,checksum;		
	UHdlcFrame *layer2;
	UIControl *iCtl;
	static U8 bitMask[20],uniqueID[20];
	static BOOL reboot = FALSE;
	static U8 stateMode = NO_ADDRESS;
	static UserTimerDef linkTimeOut;

	tFI = tFI;

	if ( layer7 == NULL )return DIS_RESPONSE;
	/*Link timeout
	Whenever a secondary station receives an HDLC frame addressed to itself, i.e. not an 
	all-station address (0xFF), it shall restart a 3 minute timer. If this 3 minute timer 
	expires, the secondary station shall be reset.
	*/	
	if ( stateMode == CONNECTED && UserTimerOver(TIM1,&linkTimeOut,USER_TIMER_1S(60*3)) )
	{		
		NVIC_SystemReset();
	}	
	//fetch the uart buffer	 	
	GetUartBufInfo(port,(U8**)&layer2,&len);
	//Buffer is empty
	if ( len == 0 && reboot == TRUE )
	{
		reboot = FALSE;
		NVIC_SystemReset();
	}
	//the frame is valid?
	if ( len < HDLC_FRAME_LEN_MIN )return DIS_RESPONSE;
	//the sync flag is valid?
	if ( layer2->buf[0] != HDLC_FRAME_FLAG || layer2->buf[len - 1] != HDLC_FRAME_FLAG )return DIS_RESPONSE;	
	//Converted
	len = HdlcConvertData(HDLC_FRAME_RX,(U8*)layer2,len);
	//Calc the fcs
	checksum = (layer2->buf[len - 2]<<8)|layer2->buf[len - 3];	
	if ( checksum != CalcCRC16Value(&layer2->buf[1],len - 4))
	{
		ResetUartBuf(port);//reset the uart buffer
		return DIS_RESPONSE;	
	}
	/* Layer2 State Model AISG 2.0 
	The INFO field is only present in I frames and 	XID frames. The INFO field in an I frame 
	contains the layer 7 payload*/
	if ( stateMode == CONNECTED )
	{//connected		   		
		if ( layer2->member.ctl == XID )
		{
		 	return DIS_RESPONSE;
		}
		else if ( layer2->member.addr == gHdlcParam.addr )
		{
			UserTimerReset(TIM1,&linkTimeOut);
			
			ctl = layer2->member.ctl;
			//Frame type DM is not needed			
			if ( IS_I_FRAME(ctl) )
			{//Frame type I
				iCtl = (UIControl*)&layer2->member.ctl;
				iCtl->meb.PF = 1;
				iCtl->meb.NR = iCtl->meb.NS + 1;

				layer7->flag = TRUE;
				layer7->rLen = len - 6;
				layer7->info = layer2->member.info;				
				return layer7->rLen;						
			}
			else if ( IS_S_FRAME(ctl) )
			{//Frame type RR,RNR	
				if ( IS_RR(ctl) )
				{
					/*
					This command and response is used to inform the opposite station (primary or secondary)
					that the transmitting station has empty buffers, i.e. is ready to receive an I frame. 
					This aspect is used for flow control.
					*/
					//PROC
					return WITHOUT_INFO_LEN;		
				}
				else if ( IS_RNR(ctl) )
				{
					/*
					Just like RR, except it informs the opposite station that the transmitting st-
					ation does not have empty buffers, i.e. that it is not ready to receive an I 
					frame. This aspect is used for flow control.
					*/
					//PROC
					return WITHOUT_INFO_LEN;
				}
				else
				{					
					goto PROC_FRMR;
				}									
			}
			else if ( IS_U_FRAME(ctl) && layer2->member.ctl == UNC_CMD_DISC )
			{//Frame type DISC,Link Disconnection
				layer7->flag = FALSE;
				stateMode = NO_ADDRESS;				
				layer2->member.ctl = UNC_RES_UA;
				gHdlcParam.addr = DEFAULT_ADDRESS;
				return WITHOUT_INFO_LEN;									
			}
			else
			{//Frame type FRMR
				goto PROC_FRMR;	
			}				
		}					
	}	
	else if ( stateMode == NO_ADDRESS || stateMode == ADDRESS_ASSIGNED )
	{//the no address
		if ( layer2->member.ctl == UNC_CMD_SNRM && stateMode == ADDRESS_ASSIGNED )
		{//Frame type SNRM,address assigned	Link establishment
			layer2->member.ctl = UNC_RES_UA;
			stateMode = CONNECTED;
			return WITHOUT_INFO_LEN;
		}					
		else if ( layer2->member.ctl == XID )
		{//only support the XID
			tFI = layer2->member.info[0];
			tGI = layer2->member.info[1];
			tGL = layer2->member.info[2];

			if ( layer2->member.info[3] == 0x07 )
			{//Reset device
				/*
				If the XID command reset device is received as a broadcast (0xFF) by the secondary
				device, the secondary device shall reset without responding, otherwise the addres-
				sed secondary device shall reset after responding.
				The reset device parameter shall not be combined with other parameters in an XID command.
				*/		
				if ( layer2->member.addr == BROADCAST_ADDRESS )
				{//the no-addressed secondary device
					NVIC_SystemReset();	
				}
				else if ( layer2->member.addr == gHdlcParam.addr ) 
				{//the addressed secondary device
					reboot = TRUE;		
				}				
				return tGL + 4;//response
			}
			else if ( layer2->member.addr == BROADCAST_ADDRESS && tGI == GI_USER )
			{//Address assignment & Device scan
				/*
				The primary station broadcasts the XID commands. The secondary 
				station(s) which match shall respond. The primary shall ensure 
				that only one secondary matches the supplied parameter(s). See 
				below for details.
				Format Identifier (FI) shall be 0x81 and Group Identifier (GI) 
				shall be 0xF0. All secondary stations shall support the following parameters:
				
				An address assignment XID command shall contain at least PI=2 
				(HDLC Address) and shall not contain PI=3 (Bit Mask). During 
				an address assignment all secondary stations first assume a match 
				and then carry out the following steps:
				*/
				addr = DEFAULT_ADDRESS;
				for ( i = 3 ; i < tGL+3 ; )
				{	
					tPI = layer2->member.info[i];i++;
					tPL = layer2->member.info[i];i++;
					pPV =&layer2->member.info[i];i+=tPL;							
					/*NOTE:	The parameters may occur in any order in the XID command/response.*/
					switch (tPI)
					{
						case 1://Unique ID
							/*
							If PI=1 (Unique ID) is supplied, the right-most PL octets of the 
							secondary station's Unique ID are compared to the Unique ID in the 
							XID command. If they are different, the secondary station does not
							match, and the message is ignored. If the Unique ID in the XID 
							command is longer than the secondary station's Unique ID, the 
							secondary station does not match, and the message is ignored.
							*/	
							uniqueID[0] = tPL;
							memcpy(&uniqueID[1],pPV,tPL);
							//if ( tPL != gHdlcParam.uniqueID[0] )return DIS_RESPONSE;
							//if ( memcmp(&gHdlcParam.uniqueID[1],pPV,gHdlcParam.uniqueID[0]) )return DIS_RESPONSE;
							break;
						case 2://HDLC Address
							addr = *pPV;
							break;
						case 3://Bit Mask (for Unique ID), indicates a device scan							
							/*
							The device scan messages may be utilised by the primary to identify 
							all secondary stations in the NoAddress state on an interface.

							A device scan XID command shall only contain PI=1 (Unique ID) and PI=3 
							(Bit Mask), see table 4.8.3.1. PI=1 and PI=3 shall be of equal length 
							PL octets.

							In some situations it may be found that the Unique ID of a bus device 
							is unknown or has been inaccurately recorded. This HDLC command exch-
							ange is used by the primary station to perform a binary tree scan of 
							the bus, in order to identify all connected and disconnected devices.
							*/
							//the NoAddress state
							if ( gHdlcParam.addr == DEFAULT_ADDRESS )scanFlag = TRUE;
							bitMask[0] = tPL;
							memcpy(&bitMask[1],pPV,tPL);
							break;
						case 4://Device Type (see table 4.7.1)
							/*
							If PI=4 (Device Type) is supplied, the device type of the secondary 
							station is compared to the device type in the XID command. If they 
							are different, the secondary station does not match, and the message 
							is ignored.
							*/
							if (*pPV != gHdlcParam.devType)return WITHOUT_INFO_LEN;
							break;
						case 5://Protocol version
							*pPV = gHdlcParam.vesn3GPP;
							return tGL + 3;														
						case 6://Vendor Code as given in [4]
							/*
							If PI=6 (Vendor Code) is supplied, the vendor code of the secondary 
							station is compared to the vendor code in the XID command. If they 
							are different, the secondary station does not match, and the message 
							is ignored.
							*/
							if ( memcmp(pPV,gHdlcParam.vndCode,2))return DIS_RESPONSE;
							break;
						default:return DIS_RESPONSE;
					}														
				}
				/* there is two case about the address assignment and the device scan*/
				if ( scanFlag == TRUE && uniqueID[0] == bitMask[0])
				{//device scan response
					/*
					Only matching secondary stations in the NoAddress state shall respond to the 
					device scan messages.

					When each secondary station in the NoAddress state receives the command it 
					masks its Unique ID with the bit mask and compares the result with the Unique 
					ID supplied as described in clause 4.8.4. If they match, the secondary station
					responds using XID format frame according to table 8 of section 5.5 of [2].
					
					If in the NoAddress state, the secondary station masks the min(PL,2) left-most
					octets of its own unique ID with the min(PL,2) left-most octets of the bit ma-
					sk in the XID command and compares the result with the min(PL,2) left-most oc-
					tets the unique ID supplied in the XID command. If they match, the secondary 
					device masks the max(0,PL-2) right-most octets of its own unique ID with the 
					max(0,PL-2) right-most octets of the bit mask in the XID command and compares 
					the result with the max(0,PL-2) right-most octets of the unique ID supplied in
					the XID command.
					*/
					tPL = gHdlcParam.uniqueID[0];
					
					for ( i = 1 ; i <= MIN(tPL,2) ; i ++ )
					{
						bitMask[i] &= gHdlcParam.uniqueID[i];
						if ( bitMask[i] != uniqueID[i] )return DIS_RESPONSE;						
					}

					for ( i = MAX(0,tPL-2) ; i >= 1 ; i -- )
					{
						bitMask[i] &= gHdlcParam.uniqueID[i];											
						if ( bitMask[i] != uniqueID[i] )return DIS_RESPONSE;
					}					
					/*
					If they also match, the secondary station transmits an XID response message 
					with its own identification data in the fields PI=1 (complete unique ID), 
					PI=4 (device type) and PI=6 (vendor code).
					*/
					i = 3;
					layer2->member.info[i] = 0x01;					 		//PI = 1
					i++;
					layer2->member.info[i] = gHdlcParam.uniqueID[0]; 		//PL
					i++;		
					memcpy(&layer2->member.info[i],&gHdlcParam.uniqueID[1],gHdlcParam.uniqueID[0]);	//PV
					i+=gHdlcParam.uniqueID[0];

					layer2->member.info[i] = 0x04;					 		//PI = 4
					i++;
					layer2->member.info[i] = 0x01;					 		//PL
	 				i++;
					layer2->member.info[i] = gHdlcParam.devType;			//PV
					i++;

					layer2->member.info[i] = 0x06;					 		//PI = 6
					i++;
					layer2->member.info[i] = 0x02;					 		//PL
	 				i++;
					memcpy(&layer2->member.info[i],gHdlcParam.vndCode,2);	//PV					
					i+=2;
	
					layer2->member.info[2] = i-3;					 		//GL				
					//gHdlcParam.addr = addr;							 	//config the address
					
					return i;
				}
				else
				{//address assignment response
					/*
					If the secondary station still matches after these steps, the seconda-
					ry station sets its HDLC address to the address specified in PI=2 and 
					responds with an XID response which contains PI=1 and PI=4.
					*/									
					i = 3;
					layer2->member.info[i] = 0x01;					 		//PI = 1
					i++;
					layer2->member.info[i] = gHdlcParam.uniqueID[0]; 		//PL
					i++;		
					memcpy(&layer2->member.info[i],&gHdlcParam.uniqueID[1],gHdlcParam.uniqueID[0]);	//PV
					i+=gHdlcParam.uniqueID[0];

					layer2->member.info[i] = 0x04;					 		//PI = 4
					i++;
					layer2->member.info[i] = 0x01;					 		//PL
	 				i++;
					layer2->member.info[i] = gHdlcParam.devType;			//PV
					i++;
	
					layer2->member.info[2] = i-3;					 		//GL				
					gHdlcParam.addr = addr;							 		//config the address
					
					return i;
				}
			}
			else if ( tGI == GI_USER )
			{
				/*the aisg 2.0 extend the 3GPP 25462`s device type and the protocol version*/
				for ( i = 3 ; i < tGL+3 ; )
				{	
					tPI = layer2->member.info[i];i++;
					tPL = layer2->member.info[i];i++;
					pPV =&layer2->member.info[i];i+=tPL;
					
					switch (tPI)
					{
						case PI_AISG_V20:	
							*pPV = gHdlcParam.vesnAISG;
							break;
						case PI_AISG_V20_DEV_TYPE_SUB:	
							pPV[0] = gHdlcParam.devType;
							pPV[1] = 0;
							break;
						default:return DIS_RESPONSE;
					}
				}
				return tGL + 3;
			}
			else if ( tGI == GI_HDLC ) 
			{//HDLC parameter negotiation
				/*
				The SecondaryPayloadTransmitLength shall be 74 octets by default. It can be 
				increased via XID negotiation, but shall always be 74 octets or larger.
				The SecondaryPayloadReceiveLength shall be 74 octets by default. It can be 
				increased via XID negotiation, but shall always be 74 octets or larger.
				*/
				for ( i = 3 ; i < tGL+3 ; )
				{	
					tPI = layer2->member.info[i];i++;
					tPL = layer2->member.info[i];i++;
					pPV =&layer2->member.info[i];i+=tPL;
					
					switch (tPI)
					{
						case 5:	
							if ( (*(U32*)pPV)/8 <= UART_CPU_BUF_LEN - 6 ) 
							{
								gHdlcParam.bitLenTx = (*(U32*)pPV)/8;
							}
							else
							{
								*(U32*)pPV = (UART_CPU_BUF_LEN - 6)*8;
							}
							break;
						case 6:	
							if ( (*(U32*)pPV)/8 <= UART_CPU_BUF_LEN - 6 ) 
							{
								gHdlcParam.bitLenRx = (*(U32*)pPV)/8;
							}
							else
							{
								*(U32*)pPV = (UART_CPU_BUF_LEN - 6)*8;								
							}
							break;
						case 7:	
							if ( *pPV <= 7 || *pPV >= 1)
							{
								gHdlcParam.wndSizeTx = *pPV;
							}
							break;
						case 8:	
							if ( *pPV <= 7 || *pPV >= 1)
							{
								gHdlcParam.wndSizeRx = *pPV;
							}
							break;
					}
				}
				return tGL + 3;					
			}
			else
			{
				goto PROC_FRMR;
			}
		}
	}

	if ( gHdlcParam.addr != DEFAULT_ADDRESS ){stateMode = ADDRESS_ASSIGNED;}

	return DIS_RESPONSE;		    

PROC_FRMR:
	/*
	This response is used to indicate an error condition. The two most likely error conditions are:
	-	Invalid command;
	-	Sequence number problem.
	The latter is used when the primary station has requested retransmission of a sequence number 
	which it has already acknowledged.
	*/
	layer2->member.ctl = UNC_RES_FRMR;
	return WITHOUT_INFO_LEN;
}
/**
  * @brief  :Layer2 数据打包
  * @param  :port 端口
  * @param	:pktLayer2 Layer2 数据包e
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static void Layer2Pack(U8 port, U16 tLen)
{	
	U16 checksum,len;
	UHdlcFrame *layer2;

	if ( tLen == DIS_RESPONSE )return;
	//get the transmit buffer
	GetUartBufInfo(port,(U8**)&layer2,NULL);
	//add the address
	layer2->member.addr = gHdlcParam.addr;
	//calc the checksum
	checksum = CalcCRC16Value(&layer2->member.addr,tLen + 2);
	//add the fcs section
	layer2->buf[tLen + 3] = checksum&0xFF;
	layer2->buf[tLen + 4] = checksum>>8;
	//add the hdlc flag
	layer2->member.flag = HDLC_FRAME_FLAG;
	layer2->buf[tLen + 5] = HDLC_FRAME_FLAG;
	//convert
	len = HdlcConvertData(HDLC_FRAME_TX,(U8*)layer2,tLen + 6);
	//trigger the transmit
	UartTxOpen(port,len);		
}
/**
  * @brief  :Layer7 数据命令解析
  * @param  :port 端口
  * @param	:pktLayer2 Layer2 数据包
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static U16 
Layer7Handle
(U8 port, SPktLayer7 *const layer7)
{
	U8 i,procID,*data;
	U16 rLen,tLen;

	procID 	= layer7->info[0];
	rLen 	= *(U16*)&layer7->info[1];
	tLen 	= 0;
	data 	= &layer7->info[3];	
	
	if ( aisgVersion == AISG11_VERSION )
	{//AISG 1.1
		for ( i = 0; i < NumItemOfAISG11(); i ++ )
		{
			if ( gLayer7AISGV11[i].procID    == procID 		  &&
				 gLayer7AISGV11[i].issued    == PRIMARY_DEVICE&&
//				 gLayer7AISGV11[i].require   == MANDATORY	  &&
				 gLayer7AISGV11[i].procClass == PROC_CLASS1	  &&
				 gLayer7AISGV11[i].procMode  == MODE_NORMAL			
			)
			{
				if ( gLayer7AISGV11[i].proc != NULL )
				{
					tLen = gLayer7AISGV11[i].proc(gLayer7AISGV11[i].action,data,rLen);		
				}
			} 
		}
	}
	else if ( aisgVersion == AISG20_VERSION )
	{//AISG 2.0
		for ( i = 0; i < NumItemOfAISG20(); i ++ )
		{
			if ( gLayer7AISGV20[i].procID    == procID 		  &&
				 gLayer7AISGV20[i].issued    == PRIMARY_DEVICE&&
				 gLayer7AISGV20[i].require   == MANDATORY	  &&
				 gLayer7AISGV20[i].procClass == PROC_CLASS1	  &&
				 gLayer7AISGV20[i].procMode  == MODE_NORMAL			
			)
			{
				if ( gLayer7AISGV20[i].proc != NULL )
				{
					tLen = gLayer7AISGV20[i].proc(gLayer7AISGV20[i].action,data,rLen);		
				}
			} 
		}
	}

	return tLen + 3;	
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
//	/*在应用编程完成以后，MCU重启后向主机发送确认消息*/
//	if ( TRUE == CheckDownFlag() )
//	{
//	}
}
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static void AISG_PktHandle(U8 port)
{
	U16 tLen;
	SPktLayer7 layer7;
	
	layer7.flag = FALSE;	
	//handle the layer2	about the hdlc protocol
	tLen = Layer2Unpack(port,&layer7);
	//handle the layer7	about the aisg
	if ( layer7.flag == TRUE )
	{
		tLen = Layer7Handle(port,&layer7);
	}
	//handle the response
	Layer2Pack(port,tLen);			
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
	aisgVersion = AISG20_VERSION;
	InitUartBuf(UART1);
	ReturnUpgradeFlag(UART1);
}
/**
  * @brief  :protocol task routine
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
void TaskProtocol(void)
{	
	AISG_PktHandle(UART1);
}
/********************************END OF FILE***********************************/
