/*******************************Copyright (c)***********************************
 *
 *              Copyright (C), 1999-2011, Jointcom . Co., Ltd.
 *
 *------------------------------------------------------------------------------
 * @file	:rf_protocol.h
 * @author	:mashuai
 * @version	:v2.0 
 * @date	:2011.10.20
 * @brief	:
 *------------------------------------------------------------------------------
 * @attention
 ******************************************************************************/
#ifndef __RF_PROTOCOL_H__
#define __RF_PROTOCOL_H__

#ifndef __RF_PROTOCOL_C__
#define	__RF_PROTOCOL_EXT__ 	 extern
#else
#define	__RF_PROTOCOL_EXT__
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
//asb protocol struct
typedef struct __ASB_EXE_TABLE{
	U8	cmd;
	U8* data;
	U16	len;
	__callback void (*set)(U8*pt,U16 len); 	
	__callback void (*get)(U8*pt,U16 len);
}ASB_EXE_TABLE;
//jointcom internal protocol struct
typedef struct __JOINTCOM_EXE_TABLE {
    U16 cmd;
    U8  *data;
    U16 len;
    U8  type;
    S32 valMin;
    S32 valMax;
    S32 defaultVal;
    __callback void (*set)(U8*pt,U16 len);
    __callback void (*get)(U8*pt,U16 len);
} JOINTCOM_EXE_TABLE;
/* Public define -------------------------------------------------------------*/
//AISG 1.1
#define AISG_V11_GLOBAL_Reserved       				0x01 
#define AISG_V11_GLOBAL_GetDeviceType           	0x02 
#define AISG_V11_GLOBAL_Reset                   	0x03 
#define AISG_V11_GLOBAL_GetErrorStatus          	0x04 
#define AISG_V11_GLOBAL_GetInfo                 	0x05 
#define AISG_V11_GLOBAL_ClearAlarms             	0x06 
#define AISG_V11_GLOBAL_Alarm                   	0x07 
#define AISG_V11_GLOBAL_EnableDevice            	0x08 
#define AISG_V11_GLOBAL_DisableDevice 				0x09 
#define AISG_V11_GLOBAL_SelfTest                	0x0A 
#define AISG_V11_GLOBAL_GetSupportedBitRates    	0x0D 
#define AISG_V11_GLOBAL_SetDeviceData           	0x0E 
#define AISG_V11_GLOBAL_GetDeviceData           	0x0F 
#define AISG_V11_OPTIONAL_ReadMemory            	0x0B 
#define AISG_V11_OPTIONAL_WriteMemory           	0x0C 
#define AISG_V11_OPTIONAL_DownloadStart         	0x2E 
#define AISG_V11_OPTIONAL_StoreDataStart        	0x20 
#define AISG_V11_OPTIONAL_StoreDataBlockSeg    		0x21 
#define AISG_V11_OPTIONAL_DownloadEnd           	0x22 
#define AISG_V11_OPTIONAL_SetBitRate            	0x24 
                                                	
//TMA identification                            	
#define AISG_V11_TMA_SetMode  						0x40 
#define AISG_V11_TMA_GetMode  						0x41 
#define AISG_V11_TMA_SetGain  						0x42 
#define AISG_V11_TMA_GetGain  						0x43  
                                                	
//3GPP Common elementary procedures             	
#define ID_3GPP_Reserved	              			0x01	
#define ID_3GPP_ResetSoftware              			0x03	
#define ID_3GPP_GetAlarmStatus             			0x04
#define	ID_3GPP_GetInformation             			0x05
#define ID_3GPP_ClearActiveAlarms          			0x06
#define ID_3GPP_AlarmSubscribe             			0x12
#define ID_3GPP_ReadUserData               			0x10
#define ID_3GPP_WriteUserData              			0x11
#define ID_3GPP_SelfTest                   			0x0A
#define ID_3GPP_DownloadStart              			0x40
#define ID_3GPP_DownloadApplication        			0x41
#define ID_3GPP_DownloadEnd                			0x42
#define	ID_3GPP_Vendorspecificprocedure    			0x90
                                                	
//Specification of Elementary Procedures        	
#define	AISG_V20_TMASetMode               			0x70
#define	AISG_V20_TMAGetMode                     	0x71
#define	AISG_V20_TMAGetSupportedFunctions       	0x7A
#define	AISG_V20_TMASetGain                     	0x72
#define	AISG_V20_TMAGetGain                			0x73
#define	AISG_V20_TMASetDeviceData          			0x74
#define	AISG_V20_TMAGetDeviceData          			0x75
#define	AISG_V20_TMAAlarmIndication        			0x76
#define	AISG_V20_TMAClearActiveAlarms      			0x77
#define	AISG_V20_TMAGetAlarmStatus         			0x78
#define	AISG_V20_TMAGetNumberOfSubunits    			0x79
 
 
//ASSIGNED RETURN CODES AISG1.1
#define RETURN_AISG11_OK  							0x00	//Normal response 
#define RETURN_AISG11_ActuatorDetectionFail  		0x01	//Signals from the actuator are detected but are abnormal, for example due to failed calibration.
#define RETURN_AISG11_ActuatorJamPermanent  		0x02	//Actuator cannot be moved 
#define RETURN_AISG11_ActuatorJamTemporary  		0x03	//Actuator jam has been detected. No movement was detected in response to the normal stimulus. 
#define RETURN_AISG11_BlockNumberSequenceError  	0x04	//Used in combination with software download; block number sequence is wrong. 
#define RETURN_AISG11_Busy  						0x05	//The device is busy and cannot respond until an activity is complete. 
#define RETURN_AISG11_ChecksumError  				0x06	//Used in combination with software download; checksum incorrect. 
#define RETURN_AISG11_CommandSequenceError  		0x07	//Used in combination with software download; command sequence is not permitted, eg a SetTilt command is received during software update sequence. 
#define RETURN_AISG11_DataError  					0x08	//Layer 7 data fault, eg length of data is inconsistent with length fields. 
#define RETURN_AISG11_DeviceDisabled  				0x09	//Device is in logical Disabled state and cannot execute Set commands. 
#define RETURN_AISG11_EEPROMError                   0x0A	//EEPROM error detected 
#define RETURN_AISG11_Fail  						0x0B	//Abnormal response. Indicates that a command has not been executed. 
#define RETURN_AISG11_FlashEraseError  				0x0C	//Used in combination with software download. indicates error when erasing flash memory. 
#define RETURN_AISG11_FlashError   					0x0D	//Used in combination with software download. indicates error when writing to flash memory. 
#define RETURN_AISG11_NotCalibrated  				0X0E	//The device has not completed a calibration operation, or calibration has been lost. 
#define RETURN_AISG11_NotScaled  					0x0F	//No setup table has been stored in the device. 
#define RETURN_AISG11_OtherHardwareError  			0x11	//Any hardware error which cannot be classified
#define RETURN_AISG11_OtherSoftwareError  			0x12	//Any software error which cannot be classified.
#define RETURN_AISG11_OutofRange  					0x13	//Value specified by a Set Tilt command is not supported by the device. 
#define RETURN_AISG11_PositionLost  				0x14	//RET controller is unable to return a correct position value, for example there was a powerfailure while a SetTilt command was being executed. 
#define RETURN_AISG11_RAMError   					0x15	//An error was detected in reading data to/from RAM 
#define RETURN_AISG11_SegmentNumberSequenceError 	0x16	//Used in combination with software download; block sequence number is wrong. 
#define RETURN_AISG11_UARTError 					0x17	//Hardware specific. This error may be sent after recovery from a temporary error which has prevented the sending or receiving ofdata. 
#define RETURN_AISG11_UnknownCommand   				0x19	//Received command is not defined in the version of AISG1 transmitted in the frame header, or the device has received a vendor-specific command with a vendor ID different from its own. 
#define RETURN_AISG11_TMAAlarmMinor  				0x1A	//An event has taken place that effects the TMAperformance, the TMA continues to function and bypass is not implemented. (The actual performance degradation criteria must be vendor specified.) 
#define RETURN_AISG11_TMAAlarmMajor  				0x1B	//An event has taken place that renders TMA performance unacceptable. If bypass is fitted the TMA will switch into bypass. 
#define RETURN_AISG11_GainoutofRange  				0x1C	//A SetGain command was received which specified a gain out side the range supported.
#define RETURN_AISG11_ReadOnly  					0x1D	//Used in combination with SetDeviceData command; the device parameter cannot be changed. 
#define RETURN_AISG11_UnknownParameter  			0x1E	//Specified parameter is not supported for this command. Mainly used as a response to SetDeviceData if an attempt is made to set anunsupported field. 

//Return Codes for Secondary Devices
#define RETURN_3GPP_OK								0x00	//Normal response
#define RETURN_3GPP_MotorJam 						0x02	//Motor cannot move
#define RETURN_3GPP_ActuatorJam						0x03	//Actuator jam has been detected. No movement of the actuator, but movement of the motor was detected
#define RETURN_3GPP_Busy							0x05	//The device is busy and cannot respond until an ongoing activity is completed
#define RETURN_3GPP_ChecksumError					0x06	//Checksum incorrect for otherwise valid data..
#define RETURN_3GPP_FAIL							0x0B	//Abnormal response. Indicates that a procedure has not been executed successfully
#define RETURN_3GPP_NotCalibrated					0x0E	//The device has not completed a calibration operation, or calibration has been lost
#define RETURN_3GPP_NotConfigured					0x0F	//Actuator configuration data is missing
#define RETURN_3GPP_HardwareError					0x11	//Any hardware error which cannot be classified. May not be reported as an alarm until the fault is likely to be persistent
#define RETURN_3GPP_OutOfRange						0x13	//A parameter given by an operator (e.g. tilt value or memory offset) is out of range
#define RETURN_3GPP_UnknownProcedure 				0x19	//Received procedure code is not defined
#define RETURN_3GPP_ReadOnly						0x1D	//Invalid device data parameter usage
#define RETURN_3GPP_UnknownParameter				0x1E	//Specified parameter is not supported for the used procedure
#define RETURN_3GPP_WorkingSoftwareMissing			0x21	//The unit is inDownloadMode state. Returned upon unsupported procedure when in DownloadMode state
#define RETURN_3GPP_InvalidFileContent				0x22	//The data being downloaded is detected to be of wrong format or size
#define RETURN_3GPP_FormatError						0x24	//Procedure message is inconsistent or if an addressed field or antenna is invalid or the data parameter field length is inconsistent with the corresponding field length parameter
#define RETURN_3GPP_UnsupportedProcedure			0x25	//The procedure is optional and not supported or the procedure does not apply to this device type
#define RETURN_3GPP_InvalidProcedureSequence		0x26	//Procedure sequence as described in annex C is expected but not experienced by the secondary device
#define RETURN_3GPP_ActuatorInterference			0x27	//An actuator movement outside the control of the RET unit has been detected. Probable cause is manual interference

//Table B.1: Additional Return Codes for Secondary AISG ALDs    
#define RETURN_AISG20_MinorTMAFault   				0x1A	//A fault in the TMA subunit is detected which reduces the gain performance but maintains its function.                                                      
#define RETURN_AISG20_MajorTMAFault   				0x1B	//A fault in the TMA subunit is detected. The fault prevents the function of the TMA subunit.                                                           
#define RETURN_AISG20_UnsupportedValue  			0x1C	//The requested value is not supported.      
#define RETURN_AISG20_BypassMode  					0x1F	//The TMA subunit is in bypass mode and cannot report a correct gain value.                                

//Issued by                                     	
#define PRIMARY_DEVICE								0x00
#define SECONDARY_DEVICE							0x01
//Requirement                                   	
#define MANDATORY									0x00
#define OPTIONAL									0x01						
//Procedure class                               	        		
#define PROC_CLASS1									0x00
#define PROC_CLASS2									0x01
//DownloadMode state                            	           		
#define MODE_DOWNLOAD								0x00
#define MODE_NORMAL									0x01
//Power mode                                    	
#define POWER_HIGH									0x00
#define POWER_LOW									0x01
//length is variable
#define LENGTH_FIX									0x00
#define LENGTH_VAR									0x01
//NA                                                    	
#define NA											0xFF
//action
#define ACTION_SET									0x00
#define ACTION_GET									0x01
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/
__RF_PROTOCOL_EXT__ int TaskProtocol(int*argv[],int argc);
__RF_PROTOCOL_EXT__ void InitProtocol(void);
__RF_PROTOCOL_EXT__ U16	CalcCRC16Value(U8 *ptr,U16 len);

#endif/*__RF_PROTOCOL_H*/
/********************************END OF FILE***********************************/
