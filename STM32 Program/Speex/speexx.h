#define HAVE_CONFIG_H
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <speex/speex.h>
#include <stm32f30x.h>

#define MODIFY_SPEEX //����� � �������������� ������ ����� �������������� �������

//#define FRAME_SIZE 160
#define ENCODED_FRAME_SIZE 20 //������� � 8 ���
#define MAX_REC_FRAMES 100 //������������ ����� ������������ �������, ����� = MAX_REC_FRAMES*0,02���
#ifdef MODIFY_SPEEX
	#define M_ENCODED_FRAME_SIZE  (ENCODED_FRAME_SIZE + 1)
    extern uint8_t REC_DATA[MAX_REC_FRAMES*M_ENCODED_FRAME_SIZE]; //���� ����������� �������������� ������
#else
    extern uint8_t REC_DATA[MAX_REC_FRAMES*ENCODED_FRAME_SIZE]; //���� ����������� �������������� ������
#endif

//extern __IO uint16_t IN_Buffer[2][FRAME_SIZE];
//extern __IO uint8_t Start_Encoding;
//extern uint8_t REC_DATA[2][MAX_REC_FRAMES*ENCODED_FRAME_SIZE]; //���� ����������� �������������� ������

extern __IO uint16_t IN_Buffer[FRAME_SIZE];
extern __IO uint8_t Start_Encoding;


void Speex_Init(void);
void EncodingVoice(void);
