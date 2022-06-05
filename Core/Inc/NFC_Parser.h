/*
 * NFC_Parser.h
 *
 *  Created on: Jun 3, 2022
 *      Author: domin
 */

#ifndef INC_NFC_PARSER_H_
#define INC_NFC_PARSER_H_

#include "main.h"

#define Prefix 10
#define SIZE_ARRAY 1024
//#define AMOUNT_SAMPLES_EACH (90 + Prefix)
#define AMOUNT_SAMPLES_EACH (190 + Prefix)
#define SAMPLE_TIME 1000

const static char Prefix_Temp[] = "Temp:";
const static char Prefix_Humidity[] = "Humidity:";
const static char Prefix_Pressure[] = "Pressure:";
const static char Prefix_MagnetoX[] = "MagnetoX:";
const static char Prefix_MagnetoY[] = "MagnetoY:";
const static char Prefix_MagnetoZ[] = "MagnetoZ:";
const static char Prefix_AccelX[] = "AccelX:";
const static char Prefix_AccelY[] = "AccelY:";
const static char Prefix_AccelZ[] = "AccelZ:";
const static char Prefix_GyroX[] = "GyroX:";
const static char Prefix_GyroY[] = "GyroY:";
const static char Prefix_GyroZ[] = "GyroZ:";


const static char Key_word_Temp[] = "Plot_Temp";
const static char Key_word_Humidity[] = "Plot_Humidity";
const static char Key_word_Pressure[] = "Plot_Pressure";
const static char Key_word_MagnetoX[] = "Plot_MagnetoX";
const static char Key_word_MagnetoY[] = "Plot_MagnetoY";
const static char Key_word_MagnetoZ[] = "Plot_MagnetoZ";
const static char Key_word_AccelX[] = "Plot_AccelX";
const static char Key_word_AccelY[] = "Plot_AccelY";
const static char Key_word_AccelZ[] = "Plot_AccelZ";
const static char Key_word_GyroX[] = "Plot_GyroX";
const static char Key_word_GyroY[] = "Plot_GyroY";
const static char Key_word_GyroZ[] = "Plot_GyroZ";




enum Sending_States{
	SEND_Temp = 1,
	SEND_Humidity,
	SEND_Pressure,
	SEND_MagnetoX,
	SEND_MagnetoY,
	SEND_MagnetoZ,
	SEND_AccelX,
	SEND_AccelY,
	SEND_AccelZ,
	SEND_GyroX,
	SEND_GyroY,
	SEND_GyroZ
};

typedef struct{

	float Humidity;
	float Temp;
	float Pressure;
	int16_t Magnetometer[3];
	int16_t Accelerometr[3];
	float Gyroscope[3];

	uint8_t Temp8[5];
	uint8_t TempLength;
	uint8_t Humidity8[8];
	uint8_t HumidityLength;
	uint8_t Pressure8[8];
	uint8_t PressureLength;
	uint8_t MagnetoX8[8];
	uint8_t MagnetoXLength;
	uint8_t MagnetoY8[8];
	uint8_t MagnetoYLength;
	uint8_t MagnetoZ8[8];
	uint8_t MagnetoZLength;
	uint8_t AccelX8[8];
	uint8_t AccelXLength;
	uint8_t AccelY8[8];
	uint8_t AccelYLength;
	uint8_t AccelZ8[8];
	uint8_t AccelZLength;
	float GyroX8[8];
	uint8_t GyroXLength;
	float GyroY8[8];
	uint8_t GyroYLength;
	float GyroZ8[8];
	uint8_t GyroZLength;

}NFC_Parser;

typedef struct{

	uint8_t Tail;
	uint8_t Head;
	uint8_t Buffer[AMOUNT_SAMPLES_EACH];
	uint8_t PrefixConst;
	uint8_t Size;

}CircleBuffer;


void NFC_Main_Task();
void InitCirclePrefix();
void SearchingPrefix(CircleBuffer *StructPointer);
void Add_Elements(CircleBuffer *StructPointer, uint8_t *InData, uint16_t Length);
void Take_Elements(CircleBuffer *StructPointer, uint8_t *OutData);
void RefreshSensorData();
uint8_t NFC_Choose_Plot();
void NFC_Send_Packet(uint8_t NumberOfPacket);


#endif /* INC_NFC_PARSER_H_ */
