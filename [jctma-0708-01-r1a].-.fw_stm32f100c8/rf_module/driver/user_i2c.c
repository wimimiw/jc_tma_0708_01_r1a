/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:user_i2c.c
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 	����I2Cͨ�ŵ��Ե�һЩ�뷨��
	I2Cͨ��Э��Ƚϼ򵥣�����һ��ͨ��ģ��һ�����Ƚ�ע��ʵʱ�ԡ��ر���MCU��Ӳ����
	��I2C�������ͨ�Ź�����ʹ�öϵ㲶׽ͨ��״̬����Ȼ���������ǿ��еģ���������
	MCU Jtag�ڶ��ڲ�I2Cģ��Ŀ����������ޣ��ϵ�۲�I2Cͨ��״̬��������I2Cģ��Ĵ�
	������Ϣ������������Ƽ������й�ͨ�Ź��̵ķ���Ӧ����״̬��׽����ʹ�ô�ӡ��
	Ϣ��������ʱ����������ֶλ�ȡͨ�Ź����е�״̬��Ϣ���������Ա���ͨ�ŵ�ʱЧ�ԣ�
	�����ڴ���ķ������Ų�Ĳο���ֵ�Ǻܸߵġ�
 ******************************************************************************/
#ifndef __USER_I2C_C__
#define __USER_I2C_C__
#endif
/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_RESET_NUM	3
#define TIMER_OVER_MAX	1000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_SLAVE_HANDLE i2c_slave_handle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  :debug driver
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
//#define __DEBUG_THIS
#ifdef __DEBUG_THIS 
int main(void)
{	
	static U8  tmp[10],buf[10] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};
	static U16 addr = 0x0000;

	InitTarget();	

	I2cWriteDevice(I2C1,0xA0,(U8*)&addr,2,buf,10);
	I2cReadDevice (I2C1,0xA0,(U8*)&addr,2,tmp,10);
		
	for(;;);
}
#endif
/**
  * @brief  :This function handles I2C1 global interrupt request.
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
//#define DEBUG_I2C_SLAVE_READ
#ifdef DEBUG_I2C_SLAVE_READ
U32 errorCnt,event;
U8  test[UART_CPU_BUF_LEN];
#endif
void I2C1_EV_IRQHandler(void)
{	
	static U16 rxCnt,txCnt,txSum;
	static U8  *txBuf,rxBuf[UART_CPU_BUF_LEN];
	volatile U32 lastEvent;

	switch( lastEvent = I2C_GetLastEvent(I2C1) )
	{	/* Slave RECEIVER mode --------------------------*/
		case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED://EV1
			rxCnt = 0;
			break;
		case I2C_EVENT_SLAVE_BYTE_RECEIVED://EV2
			rxBuf[rxCnt++%UART_CPU_BUF_LEN] = I2C_ReceiveData(I2C1);
			break;
		case I2C_EVENT_SLAVE_STOP_DETECTED://EV4
			I2C_GetITStatus(I2C1,I2C_IT_STOPF);
			I2C_GenerateSTOP(I2C1,DISABLE);	
			if ( NULL != i2c_slave_handle )
			{
				i2c_slave_handle(rxBuf,rxCnt,&txBuf,&txSum);
			}
			break;
		/* Slave TRANSMITTER mode -----------------------*/
		case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED://EV1,EV3_1
			/* warning:
			   relevance datasheet recommend under this state shoud 
			   invoking the I2C_SendData, in fact it is needed, otherwise
			   the master will received the first byte it`s '0xFF', I 
			   have no idea of this case, so remove the key of break */
			txCnt = 0;
		case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:
		case I2C_EVENT_SLAVE_BYTE_TRANSMITTING://EV3	
			/* note: on account of the I2C protocol ,the length of the 
			   read maked up master`s mind */
			if( NULL != txBuf )
			{
				I2C_SendData(I2C1,txBuf[txCnt++]);
				/* this delay is needed and the position is not permitted to alter */
				usdelay(10);
#ifdef DEBUG_I2C_SLAVE_READ
				test[txCnt-1] = txBuf[txCnt-1];
#endif
			}
			break;		
		case I2C_EVENT_SLAVE_ACK_FAILURE://EV3_2
			I2C_ClearITPendingBit(I2C1,I2C_IT_AF);
			break;
		default:
			/* while the error happen, the data queue must be initialized */ 
			txCnt = rxCnt = 0;
			I2C_GetITStatus(I2C1,I2C_IT_STOPF);
			I2C_GenerateSTOP(I2C1,DISABLE);
			I2C_ClearITPendingBit(I2C1,I2C_IT_AF);
			I2C_ClearITPendingBit(I2C1,I2C_IT_BERR);
			I2C_ClearITPendingBit(I2C1,I2C_IT_OVR);			
			I2C_ClearITPendingBit(I2C1,I2C_IT_PECERR);	
#ifdef DEBUG_I2C_SLAVE_READ
			event = lastEvent;
			errorCnt = errorCnt++;
#endif
			break;
	}	
}
/**
  * @brief  :This function handles I2C2 global interrupt request.
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
void I2C2_EV_IRQHandler(void)
{
}
/**
  * @brief  :temp
  * @param  :
  * @retval :
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static BOOL CheckEventTimeOut(I2C_TypeDef*I2Cx,U32 event)
{
	U16	timerOverCnt;

	for(timerOverCnt=0;
		timerOverCnt < TIMER_OVER_MAX && I2C_CheckEvent(I2Cx,event)==RESET;
		timerOverCnt++);

	if(timerOverCnt == TIMER_OVER_MAX)
		return FALSE;
	else
		return TRUE;		
}
/**
  * @brief  :I2C2 master communication process polling
  * @param  :I2Cx 
  * @param	:addr				������ַ
  * @param	:iaddr				�����ڲ���ַ
  * @param	:iaddrLen			�����ڲ���ַ����  
  * @param	:pbuf				���������׵�ַ
  * @param	:plen				�������ݳ���
  * @retval :BOOL
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static BOOL I2cMasterWrite(I2C_TypeDef*I2Cx, U8 addr, U8*iaddr, U8 iaddrLen, U8 *pbuf, U8 plen)
{
	BOOL result = FALSE;
	U8 	i,j;

	/*�������г���ʱ�����õ�������������¼Ĵ����Ķ�ȡ���ܳ��ִ���*/
	for(i = 0;i < I2C_RESET_NUM;i ++)
	{
		//START transmitter
		I2C_GenerateSTART(I2Cx,ENABLE);
		//wait EV5
		if(!CheckEventTimeOut(I2Cx,I2C_EVENT_MASTER_MODE_SELECT))continue;
		//address transmitter
		I2C_Send7bitAddress(I2Cx,addr,I2C_Direction_Transmitter);
		//wait EV6 EV8_1
		if(!CheckEventTimeOut(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))continue;	 
		//internal address transmitter
		for(j = 0;j < iaddrLen;j ++)
		{//wait EV8 		
			I2C_SendData(I2Cx,iaddr[j]);							
			if(!CheckEventTimeOut(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING))break;
		}
		if(j != iaddrLen)continue;		
		//data transmitter
		for(j = 0;j < plen;j ++)
		{//wait EV8
			I2C_SendData(I2Cx,pbuf[j]);
			if(!CheckEventTimeOut(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTING))break;							
		}				 
		if(j != plen)continue;
		//wait EV8_2
		if(!CheckEventTimeOut(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))continue;
		//stop transmitter
		I2C_GenerateSTOP(I2Cx,ENABLE);
		//return result
		result = TRUE;break;
	}
	return result;
}
/**
  * @brief  :I2C2 slave communication process polling
  * @param  :I2Cx 
  * @param	:addr				������ַ
  * @param	:iaddr				�����ڲ���ַ
  * @param	:iaddrLen			�����ڲ���ַ����
  * @param	:pbuf				���������׵�ַ
  * @param	:plen				�������ݳ���
  * @retval :BOOL
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static BOOL I2cMasterRead(I2C_TypeDef* I2Cx,U8 addr, U8 *pbuf, U8 plen)
{
	BOOL result = FALSE;
	U8 	i,j;

	/*�������г���ʱ�����õ�������������¼Ĵ����Ķ�ȡ���ܳ��ִ���*/
	for(i = 0;i < I2C_RESET_NUM;i ++)
	{
		//START transmitter
		I2C_GenerateSTART(I2Cx,ENABLE);
		//wait EV5
		if(!CheckEventTimeOut(I2Cx,I2C_EVENT_MASTER_MODE_SELECT))continue;
		//address receiver
		I2C_Send7bitAddress(I2Cx,addr,I2C_Direction_Receiver);
		//wait EV6
		if(!CheckEventTimeOut(I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))continue;				
		//data transmitter
		for(j = 0;j < plen;j ++)
		{//method 1
			if(1 == plen || j == plen - 1)
			{//EV6_1,EV7_1
				I2C_AcknowledgeConfig(I2Cx,DISABLE);
				I2C_GenerateSTOP(I2Cx,ENABLE);				
			}
			//wait EV7
			if(!CheckEventTimeOut(I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED))break;
			pbuf[j] = I2C_ReceiveData(I2Cx);																				
		}
		if( j != plen )continue;		
		//return result
		result = TRUE;break;
	}
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
	return result;
}
/**
  * @brief  :�����ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
  * @param	:I2Cx            I2C����
  * @param	:addr            �����ӵ�ַ
  * @param	:iaddr       	 �����ӵ�ַ
  * @param	:iaddrLen        �����ӵ�ַ����
  * @param	:pbuf            ���ݽ��ջ�����ָ��  
  * @param	:plen            ��ȡ�ĸ���
  * @retval :����ֵΪFALSEʱ��ʾ����ΪTRUEʱ��ʾ������ȷ
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL I2cReadDevice(I2C_TypeDef* I2Cx, U8 addr, U8*iaddr, U8 iaddrLen, U8*pbuf, U8 plen)
{
	/* write the device address */
	if ( TRUE == I2cMasterWrite(I2Cx,addr,iaddr,iaddrLen,NULL,0) )
	{	/* read the data from the device */
		if ( TRUE == I2cMasterRead(I2Cx,addr,pbuf,plen) )
		{
			return TRUE;
		}
	}		
	return FALSE;
}
/**
  * @brief  :�����ӵ�ַ����д��N�ֽ�����
  * @param	:I2Cx            I2C����
  * @param	:addr            �����ӵ�ַ
  * @param	:iaddr       	 �����ӵ�ַ
  * @param	:iaddrLen        �����ӵ�ַ����
  * @param	:pbuf            ���ݽ��ջ�����ָ��  
  * @param	:plen            ��ȡ�ĸ���
  * @retval :����ֵΪFALSEʱ��ʾ����ΪTRUEʱ��ʾ������ȷ
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL I2cWriteDevice(I2C_TypeDef* I2Cx, U8 addr, U8*iaddr, U8 iaddrLen, U8*pbuf, U8 plen)
{	
	return I2cMasterWrite(I2Cx,addr,iaddr,iaddrLen,pbuf,plen);
}
/**
  * @brief  :I2C Configuration
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
void InitI2c(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	//i2c master mode
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100*1000; 	
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}
/**
  * @brief  :Init the I2C slave communication
  * @param  :const I2C_CALLBACK*i2cSlaveCom
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
void InitI2cProc(I2C_TypeDef* I2Cx, I2C_SLAVE_HANDLE handle)
{
	i2c_slave_handle = handle;	 
}
/********************************END OF FILE***********************************/
