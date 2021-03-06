/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:rf_task.h
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __TASK_H__
#define __TASK_H__
#endif

#ifndef __TASK_C__
#define	__TASK_EXT__ 	 extern
#else
#define	__TASK_EXT__
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
#define TASK_MAX_NUM        10                        /* 最大任务数           */
#define TASK_TIM_HANDLE		TIM2
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/
__TASK_EXT__ void AddTaskTimer( void (*handler)(), S32 interval );
__TASK_EXT__ void DelTaskTimer( void (*handler)() );
__TASK_EXT__ void InitTaskTimer( void );
__TASK_EXT__ void TaskTimerRoutine( void );
/********************************END OF FILE***********************************/
