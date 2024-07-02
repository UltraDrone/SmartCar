#include "fuse.h"
#include "math.h"

Body Flag;                  // 结构体定义各类关键标志位
int16 Turn_PWM = 0;  				// 最终转向PWM
float vtest = 0;
//float vtest_t = 0;
float RateLow = 0.7;
float RateUp = 0.5;
int SideRate = 900, CornerRate = 1000, XieRate = 100;
//PIDT testpid = {0}; 
/*******************PIT定时中断******************
函数：void Fuse_result(void)
功能：速度环、转向环控制
参数：无
说明：
返回值：无
 **********************************************/
unsigned char int_OK = 0;								// 初始化成功标志位
unsigned char Flag_OpenLoop = 0;				// 默认进行闭环控制
//int PID_Control_test(PIDT* pid, int flag)
//{
//    int inc;

//    pid->ek = pid->SetValue - pid->ActualValue;

//    inc = pid->KPS * (pid->ek - pid->ek_1) + pid->KIS * pid->ek
//          + pid->KDS * (pid->ek - 2 * pid->ek_1 + pid->ek_2);

//    pid->ek_2 = pid->ek_1; //湔揣昫船
//    pid->ek_1 = pid->ek; //湔揣昫船
//	
//    pid->PIDout += inc;
//	   if(flag == 1)
//    {
//        if(pid->PIDout > pid->PIDmax)
//            pid->PIDout = pid->PIDmax;
//        if(pid->PIDout < pid->PIDmin)
//            pid->PIDout = pid->PIDmin;
//    }
//    return pid->PIDout;
//}
void Fuse_result(void)
{
    if(int_OK)
    {
        if(!Flag.start_go){
            go_motor(-2000, -1000);	// 出界保护
			Flag.T_Inmost = 0;
			Flag.T_Turn = 0;
			Flag.T_Distance = 0;
			Flag.T_IMU = 0;
			Flag.T_Speed = 0;
			Electromagnetism_Control();                  // 电感采集处理 冒泡排序滑动滤波
		}
        else
        {
            // 5ms控制最内环:驱动轮子往前走
            if(1 == Flag.T_Inmost)
            {
                Flag.T_Inmost = 0;
                if(!Flag_Slope)														// 只要不是坡道
				{
                    Speed_PWM = OpenLoop_Speed;						// 则不需要添加速度闭环，直接将低速值赋给占空比
				}
				
				Speed_PWM_tmp = PID_Realize(&SpeedPID, Speed_Pid, real_speed, ((ClsLoop_Speed) - abs(Turn_PWM) * (vtest) * RateLow) * 3.0 / 4) - Speed_PWM;		// 速度位置式PID
				if(Speed_PWM_tmp < 0){
					//Speed_PWM = range_protect(Speed_PWM + Speed_PWM_tmp, 500, ClsLoop_Speed + 500);
					Speed_PWM = range_protect(Speed_PWM + Speed_PWM_tmp, 1500, ((ClsLoop_Speed) - abs(Turn_PWM) * vtest * RateLow + 500) > 1500 ? ((ClsLoop_Speed) - abs(Turn_PWM) * vtest * RateLow + 500) : 1500);// 注意正负号
				}else{
					//Speed_PWM = range_protect(Speed_PWM + Speed_PWM_tmp * RateUp, 500, ClsLoop_Speed + 500);
					Speed_PWM = range_protect(Speed_PWM + Speed_PWM_tmp * RateUp, 1500, ((ClsLoop_Speed) - abs(Turn_PWM) * vtest * RateLow + 500) > 1500 ? ((ClsLoop_Speed) - abs(Turn_PWM) * vtest * RateLow + 500) : 1500);// 注意正负号
				}
				/*Speed_PWM = PID_Realize(&SpeedPID, Speed_Pid, real_speed, ClsLoop_Speed * 3.0 / 4);
				Speed_PWM = range_protect(Speed_PWM, -ClsLoop_Speed - 500, ClsLoop_Speed + 500);*/
                // 否则则将速度环运算结果投入占空比
                All_PWM_left = Speed_PWM - (Turn_PWM) * (vtest/* * (real_speed / (ClsLoop_Speed * 3.0 / 4) + 1)*/);
                All_PWM_right = Speed_PWM + (Turn_PWM) * (vtest/* * (real_speed / (ClsLoop_Speed * 3.0 / 4) + 1)*/);
				All_PWM_left = range_protect(All_PWM_left, -ClsLoop_Speed - 2000, ClsLoop_Speed + 2000);
				All_PWM_right = range_protect(All_PWM_right, -ClsLoop_Speed - 2000, ClsLoop_Speed + 2000);
				/*testpid.KPS = 10;
				testpid.KIS = 0.1;
				testpid.KDS = 0.1;
				testpid.PIDmax = 3000;
				testpid.PIDmin = -3000;
				testpid.SetValue = 1500 - adc_deviation * 50; // 船厒
				testpid.ActualValue = left_speed;
				All_PWM_left = PID_Control_test(&testpid, 1);
				if(All_PWM_left == -3000 && adc_deviation < 0){
					All_PWM_left = 3000;
				}else
				if(All_PWM_left == 3000 && adc_deviation > 0){
					All_PWM_left = -3000;
				}
				testpid.SetValue = 1500 + adc_deviation * 50;
				testpid.ActualValue = right_speed;
				All_PWM_right = PID_Control_test(&testpid, 1);
				if(All_PWM_right == -3000 && adc_deviation > 0){
					All_PWM_right = 3000;
				}else
				if(All_PWM_right == 3000 && adc_deviation < 0){
					All_PWM_right = -3000;
				}
				All_PWM_left = range_protect(All_PWM_left, -3000, 3000);
				All_PWM_right = range_protect(All_PWM_right, -3000, 3000);*/
				go_motor(All_PWM_left/*  * maxPWM*/, All_PWM_right/* * maxPWM*/);
            }

            // 10ms控制：舵机转向环
            if(1 == Flag.T_Turn && 0 == Flag_OpenLoop)
            {
                Flag.T_Turn = 0;
                Electromagnetism_Control();						// 电磁采集所有
                adc_deviation = 0
					+ Cha_BI_He_Sqrt(Left_Adc, Right_Adc, SideRate) 
					+ Cha_BI_He_Sqrt(Left_Corner_Adc, Right_Corner_Adc, CornerRate) 
					+ Cha_BI_He_Sqrt(Left_Xie_Adc, Right_Xie_Adc, XieRate); //   9： 1
                Turn_PWM = PlacePID_Control(&TurnPID, Turn_Pid[Turn_Suquence], adc_deviation, 0); //转向动态PID
                Turn_PWM = abs(Turn_PWM);
				if(adc_deviation < 0)Turn_PWM *= -1;
				Turn_PWM = range_protect(Turn_PWM, -750, 750);
				Annulus_Analysis();								// 圆环识别处理
                //Steering_Control_Out(Turn_PWM);				// 舵机最终输出（函数内部已限幅）
            }

            // 30ms控制：TOF避障
            if(1 == Flag.T_Distance)
            {
                Flag.T_Distance = 0;

                if(!Flag_Tof_Finish)						// 还未完成过一次检测
                    Tof_Control();								// 障碍物检测控制
            }

            // 40ms控制：坡道检测(IMU660)
            if(1 == Flag.T_IMU)
            {
                Flag.T_IMU = 0;
//								if(!Flag_Slope_Finish)
//									IMU_Control();
            }

            // 50ms控制：速度闭环控制
            if(1 == Flag.T_Speed)
            {
                Flag.T_Speed = 0;
                //Speed_PWM = PID_Realize(&SpeedPID, Speed_Pid, real_speed, ClsLoop_Speed);		// 速度位置式PID
                //Speed_PWM = range_protect(Speed_PWM, -0, 4000);													// 注意正负号
            }
        }
    }
}

//核心关键的标志位结构体初始化
void Flag_Init(void)
{
    Flag.start_go = 0;
    Flag.T_Inmost = 0;
    Flag.T_Turn = 0;
    Flag.T_Distance = 0;
    Flag.T_Speed = 0;
    Flag.T_IMU = 0;
    Flag.STOP = 0;								// 停车结束
    Flag.OUT_Garage = 0;				  // 出库标志位
    Flag.Game = 0;
}
