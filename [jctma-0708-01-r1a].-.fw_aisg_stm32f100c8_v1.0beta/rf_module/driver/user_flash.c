/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:user_flash.c
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 	IAP application:scatter irom 2 section
					0x8000000~0x8007000  (section1)user app
					0x8007000~0x8008000  (section2)bootloader -include flash lib
					and iwdg lib.
	define firmwware buffer:
					can define to other memory
				    buffer section = 0x8008000~0x800F000   -sizeof(section1)
	procedure	   :1.section1(load firmware to buffer)	   -section1(run)
					2.section1->section2				   -section1(run)
					3.section2(load buffer to section1)	   -section2(run)
					4.section2->section1				   -section2(run)
	firmware	   :1.(first load)all section include section1 & section2
					2.(iap upgrade)section1
	warning		   :this method with bootloader not include the data fetch way.
					so if the ungrade failed , user has to with SWD or ISP to load
					the whole firmware again.
 ******************************************************************************/
#ifndef __USER_FLASH_C__
#define __USER_FLASH_C__
#endif
/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CODE2FLASH_INIT		0xFFFE
#define CODE2FLASH_END		0xFFFF
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  :user can but uses this function in the bootloader section
  * @param  :None
  * @retval :BOOL
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL Bootloader(void) __attribute__((section("BOOTLOADER")));
BOOL Bootloader(void)
{	
	U32	i;
	U32 wrAddr,rdAddr;

	/* Close all interrupt */
	__disable_irq();
	/* Unlock the Flash Bank1 Program Erase controller */
	FLASH_Unlock();
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);		
	/* Erase the WORK FLASH pages 
	   note:if start this process, section1`s function is unavailability, so invoking those
	        while causes exception.*/
	for(i = 0; i < NUMBER_WORK_PAGE ; i++)
	{
		if ( FLASH_COMPLETE != FLASH_ErasePage(BANK_WORK_START_ADDR + (FLASH_PAGE_SIZE * i)))
		{
			goto BootFailure;
		}
		/* Reload IWDG counter 
		   if the iwdg is open , can close only when the mcu reset, so bootloader need to
		   feed the watchdog. */
    	IWDG_ReloadCounter();
	}	
	/* Program WORK Flash Bank */
	for(wrAddr = BANK_WORK_START_ADDR,rdAddr = BANK_BUF_START_ADDR;
	    rdAddr < BANK_BUF_END_ADDR; 
		wrAddr += 4,rdAddr += 4 )
	{
	 	if ( FLASH_COMPLETE != FLASH_ProgramWord(wrAddr, *(U32*)rdAddr) )
		{
			goto BootFailure;
		}
		/* verify the data write correct */
		if ( *(U32*)rdAddr != *(U32*)wrAddr)
		{
			goto BootFailure;
		}
		/* Reload IWDG counter 
		   if the iwdg is open , can close only when the mcu reset, so bootloader need to
		   feed the watchdog. */
    	IWDG_ReloadCounter();			
	}
	/* lock the Flash Bank1 Program Erase controller */
	FLASH_Lock();
	/* reset the mcu */	
	NVIC_SystemReset();//__INLINE
	return TRUE;
	/*bootloader failure*/
BootFailure:
	for(;;);
}
/**
  * @brief  :flash loader function
  * @param  :buf - data poiter 
  * @param  :len - data length
  * @retval :BOOL
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL Code2Flash(U8*buf,U16 len)
{	
	U32 i,wrAddr,tmpIdx,tmpCnt;
	static struct{
		U8  pageIdx;
		U32 byteCnt;
	    U8  buf[FLASH_PAGE_SIZE];
	}bufQueue = {0,0,0,{0}};	

	/* InitCode2Flash */
	if ( CODE2FLASH_INIT == len)
	{//clear buf information
		bufQueue.pageIdx = 0;
		bufQueue.byteCnt = 0;		
		return TRUE;		
	}	
	/* process the end of code2flash */
	if ( CODE2FLASH_END == len)
	{//load the rest of data to the buffer flash
		/* Unlock the Flash Bank1 Program Erase controller */
		FLASH_Unlock();		
		/* Clear All pending flags */
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);		
		/* Erase the FLASH pages */
		if ( FLASH_COMPLETE == FLASH_ErasePage(BANK_BUF_START_ADDR + FLASH_PAGE_SIZE * bufQueue.pageIdx))
		{
			/* Program Flash Bank */
			for ( i = 0 , wrAddr = BANK_BUF_START_ADDR + FLASH_PAGE_SIZE * bufQueue.pageIdx; 
			      i < bufQueue.byteCnt % FLASH_PAGE_SIZE ;
				  i+= 4 , wrAddr += 4)
			{
				if ( FLASH_COMPLETE != FLASH_ProgramWord(wrAddr, *(U32*)&bufQueue.buf[i]))
				{
					goto CODE2FLASH_FAILURE;
				}
				/* verify the data write correct */
				if ( *(U32*)&bufQueue.buf[i] != *(U32*)wrAddr)
				{
					goto CODE2FLASH_FAILURE;
				}										
			}				
			/*between user & bootloader filled 0x00 */									 
			for ( i = bufQueue.byteCnt + BANK_BUF_START_ADDR ; i < BANK_BUF_END_ADDR; i += 4 )
			{
				if ( FLASH_COMPLETE != FLASH_ProgramWord(wrAddr, 0))
				{
					goto CODE2FLASH_FAILURE;
				}
				/* verify the data write correct */
				if ( *(U32*)&bufQueue.buf[i] != *(U32*)wrAddr)
				{
					goto CODE2FLASH_FAILURE;
				}						
			}
		}
		else
		{
			goto CODE2FLASH_FAILURE;
		}
		/* lock the Flash Bank1 Program Erase controller */
		FLASH_Lock();
						
		bufQueue.pageIdx = 0;
		bufQueue.byteCnt = 0;

		SetDownFlag();				
		return Bootloader();							
	}
	/* check the error */
	if ( NULL == buf)return FALSE;
	/* enter the queue */
	tmpCnt = bufQueue.byteCnt;
	for ( i = 0,tmpIdx = len ; i < len && bufQueue.byteCnt <= SIZEOF_UPGRADE ; i++ )
	{
		bufQueue.buf[bufQueue.byteCnt++ % FLASH_PAGE_SIZE]	= buf[i];		
		if (( 0 == bufQueue.byteCnt % FLASH_PAGE_SIZE ))
		{
			tmpIdx = ++i;
			break;
		}	
	}
	/* operater the flash */
	if (( 0 == bufQueue.byteCnt % FLASH_PAGE_SIZE ))
	{//write ready!
		/* Unlock the Flash Bank1 Program Erase controller */
		FLASH_Unlock();		
		/* Clear All pending flags */
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);		
		/* Erase the FLASH pages */
		if ( FLASH_COMPLETE == FLASH_ErasePage(BANK_BUF_START_ADDR + FLASH_PAGE_SIZE * bufQueue.pageIdx))
		{	 
			/* Program Flash Bank1 */
			for ( i = 0 , wrAddr = BANK_BUF_START_ADDR + FLASH_PAGE_SIZE * bufQueue.pageIdx; 
			      i < FLASH_PAGE_SIZE ;
				  i+= 4 , wrAddr += 4)
			{
				if ( FLASH_COMPLETE != FLASH_ProgramWord(wrAddr, *(U32*)&bufQueue.buf[i]))
				{
					bufQueue.byteCnt = tmpCnt;
					goto CODE2FLASH_FAILURE;
				}
				/* verify the data write correct */
				if ( *(U32*)&bufQueue.buf[i] != *(U32*)wrAddr)
				{
					bufQueue.byteCnt = tmpCnt;
					goto CODE2FLASH_FAILURE;
				}			
			}						
		}
		else
		{
			bufQueue.byteCnt = tmpCnt;
			goto CODE2FLASH_FAILURE;
		}
		/* lock the Flash Bank1 Program Erase controller */
		FLASH_Lock();
		/* other data put into the queue */
		bufQueue.pageIdx ++;
		for ( i = tmpIdx ; i < len ; i ++ )
		{
			bufQueue.buf[bufQueue.byteCnt++ % FLASH_PAGE_SIZE]	= buf[i];				
		}
	}
	
	return TRUE;

CODE2FLASH_FAILURE:
	
	FLASH_Lock();
	return FALSE;	
}
/**
  * @brief  :declare the data transmission is over
  * @param  :None
  * @retval :BOOL
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL EndCode2Flash(void)
{
	return Code2Flash(NULL,CODE2FLASH_END);
}
/**
  * @brief  :initialize the data to flash
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
BOOL InitCode2Flash(void)
{
	return Code2Flash(NULL,CODE2FLASH_INIT);
}
/********************************END OF FILE***********************************/
