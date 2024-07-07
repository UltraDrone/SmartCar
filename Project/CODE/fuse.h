#ifndef __FUSE_H__
#define __FUSE_H__
#include "headfile.h"


// ���Ĺؼ��ı�־λ�ṹ�嶨��
typedef struct
{
    unsigned char start_go;         //��ʼ������
    unsigned char T_Inmost;         //���ڻ���������
    unsigned char T_Turn ;          //ת���������
    unsigned char T_Distance ;			//TOF����ϰ���
    unsigned char T_IMU;						//IMU660����µ�
    unsigned char T_Speed ;         //�ٶȿ�������
    unsigned char STOP ;            //ͣ������
    unsigned char OUT_Garage;     	//�����־λ
    unsigned char Game;
} Body;
// �ⲿ����
extern Body Flag;
extern int16 Turn_PWM;
extern unsigned char int_OK;
extern unsigned char Flag_OpenLoop;
extern int SideRate, CornerRate, XieRate;
extern float vtest;
extern float tmptest;
extern float RateLow, RateUp;
extern int dis_test;
extern int test_speed;
/*********��������**************/
void Fuse_result(void);
//���Ĺؼ��ı�־λ�ṹ���ʼ��
void Flag_Init(void);

#endif