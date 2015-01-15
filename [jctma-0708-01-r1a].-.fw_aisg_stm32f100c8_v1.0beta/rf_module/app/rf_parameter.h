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
// ����У׼�ṹ��
typedef struct _PA_ADJ_VAL_ {
	S16 dbmVal;
	U16 adVal;
}PA_ADJ_VAL, *PPA_ADJ_VAL;

typedef struct _FCT_PARAM_{
   	U8  gGainOffset1;													//0x0002		//1�ֽڣ�1#˥��������ֵ
   	U8  gGainOffset2;													//0x0102		//1�ֽڣ�2#˥��������ֵ 
   	U8  gAtt1;		    												//0x0001		//1�ֽڣ�1#˥�����趨ֵ
   	U8  gAtt2;		    												//0x0101		//1�ֽڣ�2#˥�����趨ֵ

   	U8  LNA1_gAttTrue1;													//0x0003	    //1�ֽڣ�1#˥����ʵ��ֵ
   	U8  LNA1_gAttTrue2;	    											//0x0004     	//1�ֽڣ�2#˥����ʵ��ֵ
   	U8  LNA2_gAttTrue1;													//0x0103	    //1�ֽڣ�1#˥����ʵ��ֵ
   	U8  LNA2_gAttTrue2;	    										    //0x0104     	//1�ֽڣ�2#˥����ʵ��ֵ

   	U8  SW1_30dB;			  											//����1
   	U8  SW2_30dB;			  											//����2

   	U8  gTempgain;	    												//1�ֽڣ��²���׼ֵ�趨

   	S16 gcurRfTemp;														//��ǰ�¶�ֵ
   	S16 gpreRfTemp;														//��һ���¶�ֵ
    S8  gBaseTemp;														//�¶Ȳ�����׼�¶�ֵ			 1
   	S16 gainCo;															//�ͻ�׼�¶ȵĲ�ֵ
   	S16 gtestRfTemp;													//�����¶�ֵ					 5
   	U8  gtestflag;		        										//���������־��0Ϊ����״̬��1Ϊ����״̬		   6
   	U8  gGainNumer;		        										//0x00A2		//1�ֽڣ������²�ϵ������		   3
   	U8  gGainDenomi;	            									//0x00A3		//1�ֽڣ������²�ϵ����ĸ		   4
   	U8  gAtttempval;													//�¶Ȳ���˥��ֵ				 2					

   	U16 gLnaCurHlim;	    											//0x009A		//2�ֽڣ�LNA��������
   	U16 gLnaCurLlim;	    											//0x009C		//2�ֽڣ�LNA��������
   	S16 gCurCo;															//0x0080		//2�ֽڣ�lna����б��
   	U16 gCurOffset;														//0x0082		//2�ֽڣ�lna1����������ֵ
   	U16 gCurCoAll;														//0x0080	    //2�ֽڣ�lna�ܵ���б��
   	U16 gCurOffsetAll;													//0x0082	    //2�ֽڣ�lna1�ܵ���������ֵ

    U16 LNACurrentAD;					       							//����ŵ������ܵ���
    U16 LNA1CurrentAD;				           							//1·LNA����  ------------------------->
    U16 LNA2CurrentAD;			    	       							//2·LNA���� -------------------------->
    U16 curPwrOutAD;                            						//�������ADֵ
    U16 curPwrReAD;                             						//���书��ADֵ
    S16 curPwrOut;                               						//�������dbmֵ��100��
    S16 curPwrRe;                                						//���书��dbmֵ��100��

    S16 gPwrOutOffset;													//������ʶ���ֵ��2�ֽ�
    U16 gPwrOutCo;														//������ʼ��б�ʣ�2�ֽ�
    S16 gPwrReOffset;													//���书�ʶ���ֵ��2�ֽ�
   	U16 gPwrReCo;														//���书�ʼ��б�ʣ�2�ֽ�
    U16 gValueA;														//����ϵ��A��2�ֽ�
    U16 gValueB;														//����ϵ��B��2�ֽ�

   	U8  gRF_No;															//0x00C0		//1�ֽڣ���Ƶģ����
   	U8  gRFSW1;															//0x00C2		 //----��Ƶ����1---------------->
   	U8  gRFSW2;															//0x00C3		 //----��Ƶ����2---------------->
    U8  gRFSW_RX2;						 								//RX2ѡ�񿪹� 0: Select LNA2  / 1: Select LNA1

    U16 all_checksum;													//����У���
	U8  AlarmLNA1;
	U8  AlarmLNA2;	
/*==============================================================================================*/
	U8  sn[10];															// ģ�����к� 10�ֽ�	
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
/****����У׼�궨��******************************************/    //by  2010.8.29
/* �������Ͷ���         */
#define RF_PARAM_RO	            0x00	 //ֻ������
#define RF_PARAM_SAMPLE         0x01	 //��������
#define RF_PARAM_WR		        0x02	 //��д����
#define RF_PARAM_EEPROM         0x02	 //�洢����

/* �������״̬         */
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
