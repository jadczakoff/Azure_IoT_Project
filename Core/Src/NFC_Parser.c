/*
 * NFC_Parser.c
 *
 *  Created on: Jun 3, 2022
 *      Author: domin
 */

#include "NFC_Parser.h"
#include "stm32l4s5i_iot01.h"
#include "stm32l4s5i_iot01_accelero.h"
#include "stm32l4s5i_iot01_gyro.h"
#include "stm32l4s5i_iot01_hsensor.h"
#include "stm32l4s5i_iot01_magneto.h"
#include "stm32l4s5i_iot01_psensor.h"
#include "stm32l4s5i_iot01_tsensor.h"
#include "stm32l4s5i_iot01_nfctag.h"
#include "lib_NDEF.h"
#include "lib_NDEF_Text.h"
#include <stdio.h>
#include <string.h>

NFC_Parser nfc_pars;
CircleBuffer CircTempBuff;
CircleBuffer CircHumidityBuff;
CircleBuffer CircPressureBuff;
CircleBuffer CircMagnetoXBuff;
CircleBuffer CircMagnetoYBuff;
CircleBuffer CircMagnetoZBuff;
CircleBuffer CircAccelXBuff;
CircleBuffer CircAccelYBuff;
CircleBuffer CircAccelZBuff;
CircleBuffer CircGyroXBuff;
CircleBuffer CircGyroYBuff;
CircleBuffer CircGyroZBuff;

extern sRecordInfo_t recordStruct;
extern NDEF_Text_info_t TextCont;

uint8_t NFCBuffer_Plot[2048];

uint8_t Choose = 20;


void NFC_Main_Task(){


	RefreshSensorData();
	Choose = NFC_Choose_Plot();
	HAL_Delay(500);
	NFC_Send_Packet(Choose);


}

void NFC_Send_Packet(uint8_t NumberOfPacket){

	switch(NumberOfPacket){
	case SEND_Temp:
		memcpy(&NFCBuffer_Plot[0], Prefix_Temp, strlen(Prefix_Temp));
		//memset(&NFCBuffer_Plot[(strlen(Prefix_Temp)+1)], 0 , 1);
		memcpy(&NFCBuffer_Plot[strlen(Prefix_Temp)], CircTempBuff.Buffer, sizeof(CircTempBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_Humidity:
		memcpy(&NFCBuffer_Plot[0], Prefix_Humidity, strlen(Prefix_Humidity));
		//memset(&NFCBuffer_Plot[(strlen(Prefix_Humidity)+1)], 0 , 1);
		memcpy(&NFCBuffer_Plot[strlen(Prefix_Humidity)], CircHumidityBuff.Buffer, sizeof(CircHumidityBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_Pressure:
		memcpy(&NFCBuffer_Plot[0], Prefix_Pressure, strlen(Prefix_Pressure));
		//memset(&NFCBuffer_Plot[(strlen(Prefix_Pressure)+1)], 0 , 1);
		memcpy(&NFCBuffer_Plot[strlen(Prefix_Pressure)], CircPressureBuff.Buffer, sizeof(CircPressureBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_MagnetoX:
		memcpy(&NFCBuffer_Plot[0], Prefix_MagnetoX, strlen(Prefix_MagnetoX));
		memcpy(&NFCBuffer_Plot[strlen(Prefix_MagnetoX)], CircMagnetoXBuff.Buffer, sizeof(CircMagnetoXBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_MagnetoY:
		memcpy(&NFCBuffer_Plot[0], Prefix_MagnetoY, strlen(Prefix_MagnetoY));
		memcpy(&NFCBuffer_Plot[strlen(Prefix_MagnetoY)], CircMagnetoYBuff.Buffer, sizeof(CircMagnetoYBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_MagnetoZ:
		memcpy(&NFCBuffer_Plot[0], Prefix_MagnetoZ, strlen(Prefix_MagnetoZ));
		memcpy(&NFCBuffer_Plot[strlen(Prefix_MagnetoZ)], CircMagnetoZBuff.Buffer, sizeof(CircMagnetoZBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_AccelX:
		memcpy(&NFCBuffer_Plot[0], Prefix_AccelX, strlen(Prefix_AccelX));
		memcpy(&NFCBuffer_Plot[strlen(Prefix_AccelX)], CircAccelXBuff.Buffer, sizeof(CircAccelXBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_AccelY:
		memcpy(&NFCBuffer_Plot[0], Prefix_AccelY, strlen(Prefix_AccelY));
		memcpy(&NFCBuffer_Plot[strlen(Prefix_AccelY)], CircAccelYBuff.Buffer, sizeof(CircAccelYBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_AccelZ:
		memcpy(&NFCBuffer_Plot[0], Prefix_AccelZ, strlen(Prefix_AccelZ));
		memcpy(&NFCBuffer_Plot[strlen(Prefix_AccelZ)], CircAccelZBuff.Buffer, sizeof(CircAccelZBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_GyroX:
		memcpy(&NFCBuffer_Plot[0], Prefix_GyroX, strlen(Prefix_GyroX));
		memcpy(&NFCBuffer_Plot[strlen(Prefix_GyroX)], CircGyroXBuff.Buffer, sizeof(CircGyroXBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_GyroY:
		memcpy(&NFCBuffer_Plot[0], Prefix_GyroY, strlen(Prefix_GyroY));
		memcpy(&NFCBuffer_Plot[strlen(Prefix_GyroY)], CircGyroYBuff.Buffer, sizeof(CircGyroYBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	case SEND_GyroZ:
		memcpy(&NFCBuffer_Plot[0], Prefix_GyroZ, strlen(Prefix_GyroZ));
		memcpy(&NFCBuffer_Plot[strlen(Prefix_GyroZ)], CircGyroZBuff.Buffer, sizeof(CircGyroZBuff.Buffer));
		NDEF_WriteText((char *)NFCBuffer_Plot);
		HAL_Delay(5000);
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
		break;

	default:



		break;

	}


}

uint8_t NFC_Choose_Plot(){

	NDEF_IdentifyNDEF(&recordStruct, NFCBuffer_Plot);

    if(strstr(NFCBuffer_Plot, Key_word_Temp) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_Temp;
    }

    if(strstr(NFCBuffer_Plot, Key_word_Humidity) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_Humidity;
    }

    if(strstr(NFCBuffer_Plot, Key_word_Pressure) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_Pressure;
    }


    if(strstr(NFCBuffer_Plot, Key_word_MagnetoX) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_MagnetoX;
    }

    if(strstr(NFCBuffer_Plot, Key_word_MagnetoY) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_MagnetoY;
    }

    if(strstr(NFCBuffer_Plot, Key_word_MagnetoZ) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_MagnetoZ;
    }


    if(strstr(NFCBuffer_Plot, Key_word_AccelX) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_AccelX;
    }

    if(strstr(NFCBuffer_Plot, Key_word_AccelY) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_AccelY;
    }

    if(strstr(NFCBuffer_Plot, Key_word_AccelZ) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_AccelZ;
    }

    if(strstr(NFCBuffer_Plot, Key_word_GyroX) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_GyroX;
    }

    if(strstr(NFCBuffer_Plot, Key_word_GyroY) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_GyroY;
    }

    if(strstr(NFCBuffer_Plot, Key_word_GyroZ) != NULL){
    	memset(&NFCBuffer_Plot[0], 0, sizeof(NFCBuffer_Plot));
    	NDEF_ClearNDEF();
    	return SEND_GyroZ;
    }

}


void RefreshSensorData(){

	nfc_pars.Temp = BSP_TSENSOR_ReadTemp();
	nfc_pars.TempLength = sprintf(nfc_pars.Temp8, "%.2f", nfc_pars.Temp);
	Add_Elements(&CircTempBuff, nfc_pars.Temp8, nfc_pars.TempLength);

	nfc_pars.Humidity = BSP_HSENSOR_ReadHumidity();
	nfc_pars.HumidityLength = sprintf(nfc_pars.Humidity8, "%.2f", nfc_pars.Humidity);
	Add_Elements(&CircHumidityBuff, nfc_pars.Humidity8, nfc_pars.HumidityLength);

	nfc_pars.Pressure = BSP_PSENSOR_ReadPressure();
	nfc_pars.PressureLength = sprintf(nfc_pars.Pressure8, "%.2f", nfc_pars.Pressure);
	Add_Elements(&CircPressureBuff, nfc_pars.Pressure8, nfc_pars.PressureLength);

	BSP_MAGNETO_GetXYZ(nfc_pars.Magnetometer);

	nfc_pars.MagnetoXLength = sprintf(nfc_pars.MagnetoX8, "%d", nfc_pars.Magnetometer[0]);
	Add_Elements(&CircMagnetoXBuff, nfc_pars.MagnetoX8, nfc_pars.MagnetoXLength);

	nfc_pars.MagnetoYLength = sprintf(nfc_pars.MagnetoY8, "%d", nfc_pars.Magnetometer[1]);
	Add_Elements(&CircMagnetoYBuff, nfc_pars.MagnetoY8, nfc_pars.MagnetoYLength);

	nfc_pars.MagnetoZLength = sprintf(nfc_pars.MagnetoZ8, "%d", nfc_pars.Magnetometer[2]);
	Add_Elements(&CircMagnetoZBuff, nfc_pars.MagnetoZ8, nfc_pars.MagnetoZLength);

    BSP_ACCELERO_AccGetXYZ(nfc_pars.Accelerometr);

    nfc_pars.AccelXLength = sprintf(nfc_pars.AccelX8, "%d", nfc_pars.Accelerometr[0]);
	Add_Elements(&CircAccelXBuff, nfc_pars.AccelX8, nfc_pars.AccelXLength);

	nfc_pars.AccelYLength = sprintf(nfc_pars.AccelY8, "%d", nfc_pars.Accelerometr[1]);
	Add_Elements(&CircAccelYBuff, nfc_pars.AccelY8, nfc_pars.AccelYLength);

	nfc_pars.AccelZLength = sprintf(nfc_pars.AccelZ8, "%d", nfc_pars.Accelerometr[2]);
	Add_Elements(&CircAccelZBuff, nfc_pars.AccelZ8, nfc_pars.AccelZLength);

    BSP_GYRO_GetXYZ(nfc_pars.Gyroscope);

    nfc_pars.GyroXLength = sprintf(nfc_pars.GyroX8, "%.2f", nfc_pars.Gyroscope[0]);
	Add_Elements(&CircGyroXBuff, nfc_pars.GyroX8, nfc_pars.GyroXLength);

	nfc_pars.GyroYLength = sprintf(nfc_pars.GyroY8, "%.2f", nfc_pars.Gyroscope[1]);
	Add_Elements(&CircGyroYBuff, nfc_pars.GyroY8, nfc_pars.GyroYLength);

	nfc_pars.GyroZLength = sprintf(nfc_pars.GyroZ8, "%.2f", nfc_pars.Gyroscope[2]);
	Add_Elements(&CircGyroZBuff, nfc_pars.GyroZ8, nfc_pars.GyroZLength);


}


void InitCirclePrefix(){
	memcpy(&CircTempBuff.Buffer[0], Prefix_Temp, sizeof(Prefix_Temp));
	memcpy(&CircHumidityBuff.Buffer[0], Prefix_Humidity, sizeof(Prefix_Humidity));
	memcpy(&CircPressureBuff.Buffer[0], Prefix_Pressure, sizeof(Prefix_Pressure));
	memcpy(&CircMagnetoXBuff.Buffer[0], Prefix_MagnetoX, sizeof(Prefix_MagnetoX));
	memcpy(&CircMagnetoYBuff.Buffer[0], Prefix_MagnetoY, sizeof(Prefix_MagnetoY));
	memcpy(&CircMagnetoZBuff.Buffer[0], Prefix_MagnetoZ, sizeof(Prefix_MagnetoZ));
	memcpy(&CircAccelXBuff.Buffer[0], Prefix_AccelX, sizeof(Prefix_AccelX));
	memcpy(&CircAccelYBuff.Buffer[0], Prefix_AccelY, sizeof(Prefix_AccelY));
	memcpy(&CircAccelZBuff.Buffer[0], Prefix_AccelZ, sizeof(Prefix_AccelZ));
	memcpy(&CircGyroXBuff.Buffer[0], Prefix_GyroX, sizeof(Prefix_GyroX));
	memcpy(&CircGyroYBuff.Buffer[0], Prefix_GyroY, sizeof(Prefix_GyroY));
	memcpy(&CircGyroZBuff.Buffer[0], Prefix_GyroZ, sizeof(Prefix_GyroZ));


}

void SearchingPrefix(CircleBuffer *StructPointer){
	uint8_t *TmpPointer;
	uint8_t cnt = 0;
	TmpPointer = &StructPointer->Buffer[0];
	while(*TmpPointer != ':'){
		TmpPointer++;
		cnt++;
	}
	StructPointer->PrefixConst = cnt+1;
	StructPointer->Tail = cnt+1;
	StructPointer->Head = StructPointer->Tail;
}

void Add_Elements(CircleBuffer *StructPointer, uint8_t *InData, uint16_t Length){

	uint8_t *TmpPointer;
	TmpPointer = InData;

	if(StructPointer->Head == (sizeof(StructPointer->Buffer)-1)){
		StructPointer->Head = StructPointer->PrefixConst;
	}

	StructPointer->Size = Length;

	for (uint16_t i = 0; i < Length; i++){

		StructPointer->Buffer[StructPointer->Head] = *TmpPointer;

		TmpPointer++;
		StructPointer->Head++;

	}

	StructPointer->Buffer[StructPointer->Head] = ';';

	StructPointer->Head++;
}

void Take_Elements(CircleBuffer *StructPointer, uint8_t *OutData){

	uint8_t *TmpPointer;
	TmpPointer = OutData;

	if(StructPointer->Tail == StructPointer->Head){
		return;
	}

	if(StructPointer->Tail == (sizeof(StructPointer->Buffer)-1)){
		StructPointer->Tail = StructPointer->PrefixConst;
	}

	for (uint16_t i = StructPointer->Tail; i < (StructPointer->Tail + StructPointer->Size); i++){

		*TmpPointer = StructPointer->Buffer[StructPointer->Tail];

		TmpPointer++;
		StructPointer->Tail++;

	}

	*OutData = StructPointer->Buffer[StructPointer->Tail];
	StructPointer->Tail++;

}
