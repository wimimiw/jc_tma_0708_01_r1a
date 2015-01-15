/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:main.c
 * @author	:mashuai
 * @version	:v1.0
  -----------------------------------------------------------------------------
 |	about the version manager,												   |
 |			 v1  	.	  0													   |
 |			  |     	  |													   |
 |		  hardware		 EVEN  update the user application & bootloader		   |
 |		  				 ODD   update the user application					   |
  -----------------------------------------------------------------------------
 * @date	:2012.4.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
	1.Mdk421 simulator is not perfect,need watch the ARM register to be refreshed.
	2.Mdk421 with compatibility of stm32f100c8 is not perfect.Device please select
	  the stm32f100c8, and IRAM1 size set up as 0x2000. 
	3.The crystal oscillator of mcu uses qualification of 11.0592MHz,because the l-
	  ib recommend qualification of 8MHz, is needed to modify.
	4.When the compiler is optimization, mdk421 variable obtained is error when d-
	  ebugging.
	5.About the method of stm32 debugging in irom.
	  exception: some xxxx.o not setting the breakpoint(MDK F9 represent unactive).
	  it may be caused by the JLINK V8, the solution is erasing the irom again.
 ******************************************************************************/
#ifndef __MAIN_C__
#define __MAIN_C__
#endif
/* Includes ------------------------------------------------------------------*/
#include "config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void InitWatchdog(void);
static void TaskWatchdog(void);
static void TaskDebug(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
#define __DEBUG_THIS
#ifdef __DEBUG_THIS
int main(void)
{		
	InitTarget();	   //Initialize the target
    InitWatchdog();
	InitParam();	
//	InitControl();
//  InitProtocol();
	/* warining��
	   the amount of task is limited, the default value is 10, define by TASK_MAX_NUM */	
//	AddTaskTimer( TaskSample,USER_TIMER_1MS(5) );		//5ms
//	AddTaskTimer( TaskControl,USER_TIMER_1S(1) );		//1s
	AddTaskTimer( TaskParameter,USER_TIMER_1S(2) );		//2s
	AddTaskTimer( TaskDebug,USER_TIMER_NO_DELAY );
//	AddTaskTimer( TaskProtocol,USER_TIMER_NO_DELAY );	//poll
	AddTaskTimer( TaskWatchdog,USER_TIMER_NO_DELAY );	//poll
	InitTaskTimer();   //Initialize the timer of task
	TaskTimerRoutine();//the task running
}
#endif
/**
  * @brief  :
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v1.0
  * @date	:2012.4.20
  */
static void TaskDebug(void)
{
	static BOOL flag;

	if ( flag++ == 0 )
	{		
		WritePe4302(&attBus[0],0x00);	
		WritePe4302(&attBus[1],0x00);	
		WritePe4302(&attBus[2],0x00);	
		WritePe4302(&attBus[3],0x00);
		
		GPIORF_SW5V1(HIGH);
		GPIORF_SW5V2(HIGH);
		GPIORF_SW9V1(HIGH);
		GPIORF_SW9V2(HIGH);
	}

	return;	
}
/**
  * @brief  :watchdog init
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static void InitWatchdog(void)
{
#ifdef	WATCHDOG_EN
	/* Check if the system has resumed from IWDG reset */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{	
		/* Clear reset flags */
		RCC_ClearFlag();
	}
	/* Enable the LSI OSC */
	RCC_LSICmd(ENABLE);	
	/* Wait till LSI is ready */
  	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}
	/* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
	 dispersion) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	
	/* IWDG counter clock: LSI/32 */
	IWDG_SetPrescaler(IWDG_Prescaler_32);	
	/* Set counter reload value to obtain 250ms IWDG TimeOut.
	 Counter Reload Value = 250ms/IWDG counter clock period
	                      = 250ms / (LSI/32)
	                      = 0.25s / (LsiFreq/32)
	                      = LsiFreq/(32 * 4)
	                      = LsiFreq/128
	*/
	IWDG_SetReload(40000/128);	  //~~300ms
	/* Reload IWDG counter */
	IWDG_ReloadCounter();	
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
#endif
} 
/**
  * @brief  :Watchdog Task routine
  * @param  :None
  * @retval :None
  * @author	:mashuai
  * @version:v2.0
  * @date	:2011.10.20
  */
static void TaskWatchdog(void)
{
#ifdef	WATCHDOG_EN
    /* Reload IWDG counter */
    IWDG_ReloadCounter(); 		
#endif
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while (1)
  {}
}
#endif

/********************************END OF FILE***********************************/
