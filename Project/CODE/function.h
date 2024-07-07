#ifndef _Function_h
#define _Function_h


#include "common.h"
#include "headfile.h"
#include "math.h"

//PID
//�⼸�������������±�ţ����㿴
#define KP 0
#define KI 1
#define KD 2
#define KT 3   //�����޷���

//typedef struct // PID�x�ʼ���g
//{
//    int SetValue; // ?���t �y���t
//    int ActualValue; // 桕��t
//    float KPS; // ��~�b
//	float KP2S;
//    float KIS; // �}��~�b
//    float KDS; // �a��~�b
//    int ek; // ��׫�d��
//    int ek_1; // �f����
//    int ek_2; // �f�f��
//    int ek_sum; // �d�����}

//    int Sum_max; // �d����f�j
//    int Sum_min; // �d����B�j
//    int PIDmax; // max limit
//    int PIDmin; // min limit
//	
//    int PIDout; // output
//}PIDT;
typedef struct PID   	// ����PID�����������
{
    float SumError;			// ����ۼ�
    int32 LastError;		// �ϴ����
    int32 PrevError;		// Ԥ�����
    int32 LastData;			// �ϴ�����
} PID;

/* �ⲿ���� */
extern PID TurnPID;									// ת��PID���棨ֻ�Ǵ�����м�����������ģ�
extern PID SpeedPIDL;								// �ٶ�PID���棨ֻ�Ǵ�����м�����������ģ�
extern PID SpeedPIDR;								// �ٶ�PID���棨ֻ�Ǵ�����м�����������ģ�
extern float Speed_Pid[4];					// �ٶȻ�����
extern float Turn_Pid[][5];					// ת�򻷲�������
extern uint8 Turn_Suquence;					// ת�򻷲���ѡ��
extern float ki_test;

int16 I_Median_Average_Filter(int16 *DATE);							// ȥ��ֵ��ƽ��
float Cha_BI_He_Sqrt(int16 date_1, int16 date_2, int16 x);
int16 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint);
void PID_Parameter_Init(PID *sptr);
int16 PID_Realize(PID *sptr, float *PID, int32 NowData, int32 Point);
int16 limit(int16 x, int y);
int16 range_protect(int32 duty, int32 min, int32 max);					// ����޷�����


#endif