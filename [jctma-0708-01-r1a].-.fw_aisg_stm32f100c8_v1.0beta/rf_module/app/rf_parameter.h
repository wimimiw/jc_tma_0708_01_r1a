/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:rf_parameter.h
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __RF_PARAMETER_H__
#define __RF_PARAMETER_H__
#endif

#ifndef __RF_PARAMETER_C__
#define	__RF_PARAMETER_EXT__ 	 extern
#else
#define	__RF_PARAMETER_EXT__
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
// 功率校准结构体
typedef struct _PA_ADJ_VAL_ {
	S16 dbmVal;
	U16 adVal;
}PA_ADJ_VAL, *PPA_ADJ_VAL;

typedef struct _FCT_PARAM_{
   	U8  gGainOffset1;													//0x0002		//1字节，1#衰减器调整值
   	U8  gGainOffset2;													//0x0102		//1字节，2#衰减器调整值 
   	U8  gAtt1;		    												//0x0001		//1字节，1#衰减器设定值
   	U8  gAtt2;		    												//0x0101		//1字节，2#衰减器设定值

   	U8  LNA1_gAttTrue1;													//0x0003	    //1字节，1#衰减器实际值
   	U8  LNA1_gAttTrue2;	    											//0x0004     	//1字节，2#衰减器实际值
   	U8  LNA2_gAttTrue1;													//0x0103	    //1字节，1#衰减器实际值
   	U8  LNA2_gAttTrue2;	    										    //0x0104     	//1字节，2#衰减器实际值

   	U8  SW1_30dB;			  											//开关1
   	U8  SW2_30dB;			  											//开关2

   	U8  gTempgain;	    												//1字节，温补基准值设定

   	S16 gcurRfTemp;														//当前温度值
   	S16 gpreRfTemp;														//上一次温度值
    S8  gBaseTemp;														//温度补偿基准温度值			 1
   	S16 gainCo;															//和基准温度的差值
   	S16 gtestRfTemp;													//测试温度值					 5
   	U8  gtestflag;		        										//调试命令标志，0为正常状态，1为调试状态		   6
   	U8  gGainNumer;		        										//0x00A2		//1字节，增益温补系数分子		   3
   	U8  gGainDenomi;	            									//0x00A3		//1字节，增益温补系数分母		   4
   	U8  gAtttempval;													//温度补偿衰减值				 2					

   	U16 gLnaCurHlim;	    											//0x009A		//2字节，LNA电流上限
   	U16 gLnaCurLlim;	    											//0x009C		//2字节，LNA电流下限
   	S16 gCurCo;															//0x0080		//2字节，lna电流斜率
   	U16 gCurOffset;														//0x0082		//2字节，lna1电流检测调整值
   	U16 gCurCoAll;														//0x0080	    //2字节，lna总电流斜率
   	U16 gCurOffsetAll;													//0x0082	    //2字节，lna1总电流检测调整值

    U16 LNACurrentAD;					       							//低噪放电流，总电流
    U16 LNA1CurrentAD;				           							//1路LNA电流  ------------------------->
    U16 LNA2CurrentAD;			    	       							//2路LNA电流 -------------------------->
    U16 curPwrOutAD;                            						//输出功率AD值
    U16 curPwrReAD;                             						//反射功率AD值
    S16 curPwrOut;                               						//输出功率dbm值，100倍
    S16 curPwrRe;                                						//反射功率dbm值，100倍

    S16 gPwrOutOffset;													//输出功率定标值，2字节
    U16 gPwrOutCo;														//输出功率检测斜率，2字节
    S16 gPwrReOffset;													//反射功率定标值，2字节
   	U16 gPwrReCo;														//反射功率检测斜率，2字节
    U16 gValueA;														//功率系数A，2字节
    U16 gValueB;														//功率系数B，2字节

   	U8  gRF_No;															//0x00C0		//1字节，射频模块编号
   	U8  gRFSW1;															//0x00C2		 //----射频开关1---------------->
   	U8  gRFSW2;															//0x00C3		 //----射频开关2---------------->
    U8  gRFSW_RX2;						 								//RX2选择开关 0: Select LNA2  / 1: Select LNA1

    U16 all_checksum;													//程序校验和
	U8  AlarmLNA1;
	U8  AlarmLNA2;	
/*==============================================================================================*/
	U8  sn[10];															// 模块序列号 10字节	
	U16	version;
	U8	gGC1;
	U8 	gGC2;
	U8	gGC3;
	BOOL upgrade;
} FCT_PARAM, *PFCT_PARAM;

// asb parameter struct
typedef struct _ASB_PARAM_ {
	U8 	addr;
	U8  lna1;
	U8  lna2;
	S16	temp;
	U8	version;
	U16	vswr;
	U16 power;
}ASB_PARAM;
/* Public define -------------------------------------------------------------*/
/****功率校准宏定义******************************************/    //by  2010.8.29
/* 参数类型定义         */
#define RF_PARAM_RO	            0x00	 //只读数据
#define RF_PARAM_SAMPLE         0x01	 //采样数据
#define RF_PARAM_WR		        0x02	 //读写数据
#define RF_PARAM_EEPROM         0x02	 //存储数据

/* 软件升级状态         */
#define SOFT_DOWN_RESET         0x02
#define SOFT_DOWN_DOING         0x01
#define SOFT_DOWN_UNDO          0x00

/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
extern const JOINTCOM_EXE_TABLE jointExeTab[];
extern const ASB_EXE_TABLE asbExeTab[];
__RF_PARAMETER_EXT__ FCT_PARAM gFctParam;
__RF_PARAMETER_EXT__ ASB_PARAM gAsbParam;
/* Public function prototypes ------------------------------------------------*/
__RF_PARAMETER_EXT__ BOOL SaveParam(U16 iaddr, U8 *pbuf, U16 plen);
__RF_PARAMETER_EXT__ BOOL FetchParam(U16 iaddr, U8 *pbuf, U16 plen);
__RF_PARAMETER_EXT__ void TaskParameter(void);
__RF_PARAMETER_EXT__ void InitParam(void);
__RF_PARAMETER_EXT__ BOOL CheckDownFlag(void);
__RF_PARAMETER_EXT__ void SetDownFlag(void);
/********************************END OF FILE***********************************/
