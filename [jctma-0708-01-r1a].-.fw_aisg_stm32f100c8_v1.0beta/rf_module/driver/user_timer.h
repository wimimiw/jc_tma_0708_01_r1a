/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:dev_timer.h
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __USER_TIMER_H__
#define __USER_TIMER_H__
#endif

#ifndef __USER_TIMER_C__
#define	__USER_TIMER_EXT__ 	 extern
#else
#define	__USER_TIMER_EXT__
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
typedef struct{
	U8	cflag;
	U32 cnt;
	U32 record;
}UserTimerDef;
/* Public define -------------------------------------------------------------*/
#define TIM_CNT_FREQ		38400						  /*��Ϊ����Ϊ11.0592MHz
														  	������38400Ϊ����Ƶ��
															����õ�������ʱ*/
#define USER_TIMER_NO_DELAY	(0)							  //����ʱ

#define USER_TIMER_1S(t)	((t)*TIM_CNT_FREQ		)	  //��ȷ
#define USER_TIMER_10MS(t)	((t)*TIM_CNT_FREQ/100	)	  //��ȷ
#define USER_TIMER_1MS(t)	((t)*TIM_CNT_FREQ/1000	)	  //�ֲ�
#define USER_TIMER_100US(t)	((t)*TIM_CNT_FREQ/10000	)	  //�ֲ�
/* Public macro --------------------------------------------------------------*/
__inline void usdelay(U16 delayTimer)
{
	U16 i;
	for(i = 0;i < delayTimer*NUM_NOP_IN_US ;i ++);
}
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/
__USER_TIMER_EXT__ void InitTimer(void);
__USER_TIMER_EXT__ void UserTimerReset(TIM_TypeDef* TIMx,UserTimerDef * timer);
__USER_TIMER_EXT__ BOOL UserTimerOver(TIM_TypeDef* TIMx,UserTimerDef * timer,S32 limit);
/********************************END OF FILE***********************************/
