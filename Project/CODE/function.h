#ifndef _Function_h
#define _Function_h


#include "common.h"
#include "headfile.h"
#include "math.h"

//PID
//这几个类似于数组下标号，方便看
#define KP 0
#define KI 1
#define KD 2
#define KT 3   //积分限幅项

//typedef struct // PID賦凳极隅砱
//{
//    int SetValue; // ?咡硉 統蕉硉
//    int ActualValue; // 妗暱硉
//    float KPS; // 掀瞰秪赽
//	float KP2S;
//    float KIS; // 儅煦秪赽
//    float KDS; // 峚煦秪赽
//    int ek; // 掛撰昫船
//    int ek_1; // 奻珨棒
//    int ek_2; // 奻奻棒
//    int ek_sum; // 昫船濛儅

//    int Sum_max; // 昫船濛睿奻癹
//    int Sum_min; // 昫船濛睿狟癹
//    int PIDmax; // max limit
//    int PIDmin; // min limit
//	
//    int PIDout; // output
//}PIDT;
typedef struct PID   	// 用来PID参数计算变量
{
    float SumError;			// 误差累计
    int32 LastError;		// 上次误差
    int32 PrevError;		// 预测误差
    int32 LastData;			// 上次数据
} PID;

/* 外部声明 */
extern PID TurnPID;									// 转向PID储存（只是储存的中间量，无需关心）
extern PID SpeedPIDL;								// 速度PID储存（只是储存的中间量，无需关心）
extern PID SpeedPIDR;								// 速度PID储存（只是储存的中间量，无需关心）
extern float Speed_Pid[4];					// 速度环参数
extern float Turn_Pid[][5];					// 转向环参数数组
extern uint8 Turn_Suquence;					// 转向环参数选择
extern float ki_test;

int16 I_Median_Average_Filter(int16 *DATE);							// 去极值求平均
float Cha_BI_He_Sqrt(int16 date_1, int16 date_2, int16 x);
int16 PlacePID_Control(PID *sprt, float *PID, int32 NowPiont, int32 SetPoint);
void PID_Parameter_Init(PID *sptr);
int16 PID_Realize(PID *sptr, float *PID, int32 NowData, int32 Point);
int16 limit(int16 x, int y);
int16 range_protect(int32 duty, int32 min, int32 max);					// 电机限幅保护


#endif