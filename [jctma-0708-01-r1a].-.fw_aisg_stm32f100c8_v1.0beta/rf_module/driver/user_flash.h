/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:user_flash.h
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __USER_FLASH_H__
#define __USER_FLASH_H__
#endif

#ifndef __USER_FLASH_C__
#define	__USER_FLASH_EXT__ 	 extern
#else
#define	__USER_FLASH_EXT__
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE     ((U16)0x800)
#else
  #define FLASH_PAGE_SIZE     ((U16)0x400)
#endif

#define SIZEOF_UPGRADE		  ((U32)0x7000)

#define BANK_WORK_START_ADDR  ((U32)0x08000000)
#define BANK_WORK_END_ADDR    ((U32)(BANK_WORK_START_ADDR + SIZEOF_UPGRADE))

#define BANK_BUF_START_ADDR   ((U32)0x08008000)
#define BANK_BUF_END_ADDR     ((U32)(BANK_BUF_START_ADDR  + SIZEOF_UPGRADE))

#define NUMBER_WORK_PAGE 	  (BANK_WORK_END_ADDR - BANK_WORK_START_ADDR) / FLASH_PAGE_SIZE
#define NUMBER_BUF_PAGE		  (BANK_BUF_END_ADDR  - BANK_BUF_START_ADDR ) / FLASH_PAGE_SIZE
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/
__USER_FLASH_EXT__ BOOL Code2Flash(U8*buf,U16 len);
__USER_FLASH_EXT__ BOOL EndCode2Flash(void);
__USER_FLASH_EXT__ BOOL InitCode2Flash(void);
/********************************END OF FILE***********************************/
