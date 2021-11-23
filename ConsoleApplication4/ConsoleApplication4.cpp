#include <stdio.h>
#include <string.h>
#include <iostream>
#define ENDFAME 0xff
#define ERRCHECKSUM  0x00
#define HardCheck    0x7C
enum State { Correct_Data = 1, DeCorrect_Data = 0 };
uint8_t Rbuff[100] = { 0 };
uint8_t Tbuff[11]   = { 'W', 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF };



// ID0 - PHẦN NGUYÊN
uint16_t x1;
uint16_t x2;
uint16_t x3;

// ID1 - PHẦN NGUYÊN
uint16_t yy1; 
uint16_t yy2;

// ID2 - PHẦN NGUYÊN
uint16_t z1;
uint16_t z2;
uint16_t z3;


// ID0 - PHẦN THẬP PHÂN
uint8_t k1;
uint8_t k2;
uint8_t k3;


// ID1 - PHẦN THẬP PHÂN
uint8_t k4;
//uint8_t k5;


// ID2 - PHẦN THẬP PHÂN
uint8_t k6;
uint8_t k7;
uint8_t k8;

uint32_t kq1;
uint32_t kq2;
uint32_t kq3;

uint32_t kq4;
uint32_t kq5;

uint32_t kq6;
uint32_t kq7;
uint32_t kq8;



// ARRAY MAPPING 
uint8_t Arr_Map[] = {0, 3, 5, 8};
//typedef void(*Re_Caculator)(const uint16_t*, const uint8_t *);
// STRUCT ADDRESS
typedef struct
{
	uint16_t *InterData;
	uint8_t  *DeportData;
	//uint32_t *ValueData;
	//Re_Caculator Caculator
}Refer_Data;
Refer_Data mapping[] = 
{
	{&x1, &k1},
	{&x2, &k2},
	{&x3, &k3},
	{&yy1,&k4},
	{&yy2,NULL},
	{&z1, &k6},
	{&z2, &k7},
	{&z3, &k8}
};



uint8_t Length(uint8_t *Rbuff)
{
	uint8_t length = 0;
	while (*(Rbuff + length) != 255)
	{
		length++;
	}
	return length + 1;
}
uint8_t CaCulCheckSum(uint8_t *buff, uint8_t Rlength)
{
	Rlength = 0;
	uint8_t Cchecksum = 0;
	for (uint8_t i = 0; i < Rlength - 2; i++)
	{
		Cchecksum += buff[i];
	}
	return (uint8_t)(Cchecksum & HardCheck);
}

State StateChecksum(uint8_t *Rbuff, uint8_t Rlength)
{
	if (CaCulCheckSum(Rbuff, Rlength) != Rbuff[Rlength - 1])
	{
		return DeCorrect_Data;
	}
	return Correct_Data;
}


void Handle_Data(uint8_t *buff, uint8_t  *Tbuff, uint8_t Rlength)
{
	State  state = StateChecksum(buff, Rlength);
	if (state == Correct_Data)
	{
		uint16_t id = (uint16_t) ((buff[1] << 8) | buff[2]);
		uint8_t  number = (uint8_t)buff[3];
		uint16_t inter[20];
		uint8_t  deport[20];
		uint8_t  i_index = 0, d_index = 0;
		uint32_t U32Count1 = 0;
		if (buff[0] == 'W')
		{
			for (uint8_t i = 6; i < (4 + number * 3); i++)
			{
				if (i % 3 == 0)
				{
					deport[d_index] = (uint8_t)buff[i];
					d_index++;
					inter[i_index] = (buff[i - 2] << 8) | buff[i - 1];
					i_index++;
				}
			}
			for (uint32_t U32Count = Arr_Map[id], U32Count1 = 0; U32Count < Arr_Map[id + 1], U32Count1 < number; U32Count++, U32Count1++)
			{
				*(mapping[U32Count].InterData) = inter[U32Count1];
				if (mapping[U32Count].DeportData != NULL)
				{
					*(mapping[U32Count].DeportData) = deport[U32Count1];
				}
			}
		}
		else  // when reading
		{
			

		}
	}
	else   // Data Erros
	{
		Tbuff[1] = buff[1];
		Tbuff[2] = buff[2];
		Tbuff[3] = ERRCHECKSUM;
		Tbuff[8] = CaCulCheckSum(Tbuff,10);
		// Transfer ERROS BUFF


	}
}





void receive(uint8_t * buff)
{
	Rbuff[0] =  'W';
	Rbuff[1] =  0x01;   //354
	Rbuff[2] =  0x02;
	Rbuff[3] =  0x03; 

	Rbuff[4] =  0xC3;   //2F7
	Rbuff[5] =  0x5E;
	Rbuff[6] =  0x02;
	Rbuff[7] = 0x75;
	Rbuff[8] = 0x3F;
	Rbuff[9] = 0x00;
	Rbuff[10] = 0x9C;
	Rbuff[11] = 0x82;
	Rbuff[12] = 0x02;

	Rbuff[13] = 0x23;
	Rbuff[14] = 0xff;
}
void transfer(uint8_t *buff);
int main()
{
  //receive(Rbuff);
  //printf("%d ", Length(Rbuff));
	return 0;
}

