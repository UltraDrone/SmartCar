#include "fuse.h"
#include "math.h"

Body Flag;                  // �ṹ�嶨�����ؼ���־λ
int16 Turn_PWM = 0;  				// ����ת��PWM
float vtest = 0;
//float vtest_t = 0;
float RateLow = 0.7;
float RateUp = 0.5;
float tmptest = 0;
int SideRate = 900, CornerRate = 1000, XieRate = 100;
int dis_test = 0;
int test_speed = 0;
//PIDT testpid = {0}; 
/*******************PIT��ʱ�ж�******************
������void Fuse_result(void)
���ܣ��ٶȻ���ת�򻷿���
��������
˵����
����ֵ����
 **********************************************/
unsigned char int_OK = 0;								// ��ʼ���ɹ���־λ
unsigned char Flag_OpenLoop = 0;				// Ĭ�Ͻ��бջ�����
//int PID_Control_test(PIDT* pid, int flag)
//{
//    int inc;

//    pid->ek = pid->SetValue - pid->ActualValue;

//    inc = pid->KPS * (pid->ek - pid->ek_1) + pid->KIS * pid->ek
//          + pid->KDS * (pid->ek - 2 * pid->ek_1 + pid->ek_2);

//    pid->ek_2 = pid->ek_1; //�մ��d��
//    pid->ek_1 = pid->ek; //�մ��d��
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
            go_motor(-2000, -1000);	// ���籣��
			Flag.T_Inmost = 0;
			Flag.T_Turn = 0;
			Flag.T_Distance = 0;
			Flag.T_IMU = 0;
			Flag.T_Speed = 0;
			Electromagnetism_Control();                  // ��вɼ����� ð�����򻬶��˲�
		}
        else
        {
            // 5ms�������ڻ�:����������ǰ��
            if(1 == Flag.T_Inmost)
            {
                Flag.T_Inmost = 0;
                if(!Flag_Slope)														// ֻҪ�����µ�
				{
                    Speed_PWM = OpenLoop_Speed;						// ����Ҫ����ٶȱջ���ֱ�ӽ�����ֵ����ռ�ձ�
				}
				
//				Speed_PWM_tmp = PID_Realize(&SpeedPID, Speed_Pid, real_speed, ((ClsLoop_Speed) - abs(Turn_PWM) * (vtest) * RateLow) * 3.0 / 4) - Speed_PWM;		// �ٶ�λ��ʽPID
//				if(Speed_PWM_tmp < 0){
//					//Speed_PWM = range_protect(Speed_PWM + Speed_PWM_tmp, 500, ClsLoop_Speed + 500);
//					Speed_PWM = range_protect(Speed_PWM + Speed_PWM_tmp, 1500, ((ClsLoop_Speed) - abs(Turn_PWM) * vtest * RateLow + 500) > 1500 ? ((ClsLoop_Speed) - abs(Turn_PWM) * vtest * RateLow + 500) : 1500);// ע��������
//				}else{
//					//Speed_PWM = range_protect(Speed_PWM + Speed_PWM_tmp * RateUp, 500, ClsLoop_Speed + 500);
//					Speed_PWM = range_protect(Speed_PWM + Speed_PWM_tmp * RateUp, 1500, ((ClsLoop_Speed) - abs(Turn_PWM) * vtest * RateLow + 500) > 1500 ? ((ClsLoop_Speed) - abs(Turn_PWM) * vtest * RateLow + 500) : 1500);// ע��������
//				}
//				/*Speed_PWM = PID_Realize(&SpeedPID, Speed_Pid, real_speed, ClsLoop_Speed * 3.0 / 4);
//				Speed_PWM = range_protect(Speed_PWM, -ClsLoop_Speed - 500, ClsLoop_Speed + 500);*/
//                // �������ٶȻ�������Ͷ��ռ�ձ�
//                All_PWM_left = Speed_PWM - (Turn_PWM) * (vtest/* * (real_speed / (ClsLoop_Speed * 3.0 / 4) + 1)*/);
//                All_PWM_right = Speed_PWM + (Turn_PWM) * (vtest/* * (real_speed / (ClsLoop_Speed * 3.0 / 4) + 1)*/);
				/*Electromagnetism_Control();						// ��Ųɼ�����
                adc_deviation = 0
					+ Cha_BI_He_Sqrt(Left_Adc + Left_Corner_Adc * 1.5, Right_Adc + Right_Corner_Adc * 1.5, SideRate) 
					//+ Cha_BI_He_Sqrt(Left_Corner_Adc, Right_Corner_Adc, CornerRate) 
					+ Cha_BI_He_Sqrt(Left_Xie_Adc, Right_Xie_Adc, XieRate); //   9�� 1
                Turn_PWM = PlacePID_Control(&TurnPID, Turn_Pid[Turn_Suquence], adc_deviation, 0); //ת��̬PID
                Turn_PWM = abs(Turn_PWM);
				if(adc_deviation < 0)Turn_PWM *= -1;
				Turn_PWM = range_protect(Turn_PWM, -750, 750);
				Annulus_Analysis();								// Բ��ʶ����*/
				//right_speed + left_speed
				speed_measure();
				if(Flag_OpenLoop != 1){
					//Turn_PWM = 0;
					
					tmptest = (real_speed * 20 - 1500.0) / (ClsLoop_Speed - 1500.0) * (vtest - 0.4 * vtest) + 0.4 * vtest;
					if(tmptest < 1){
						tmptest = 1;
					}
					//tmptest = vtest;
					//tmptest = 0;
					Speed_PWM = (ClsLoop_Speed - abs(Turn_PWM)/* * (tmptest)*/ * RateLow);
					/*if(test_speed < 1000){
						test_speed++;
					}else{
						test_speed++;
						if(test_speed > 2000){
							test_speed = 0;
						}
						Speed_PWM *= -1;
					}*/
					All_PWM_left = All_PWM_left + PID_Realize(
						&SpeedPIDL, Speed_Pid, left_speed * 20, 
						Speed_PWM - (Turn_PWM)/* * (tmptest)*/
					);
					All_PWM_right = All_PWM_right + PID_Realize(
						&SpeedPIDR, Speed_Pid, right_speed * 20, 
						Speed_PWM + (Turn_PWM)/* * (tmptest)*/
					);
					//Speed_PWM = (ClsLoop_Speed - abs(Turn_PWM) * (tmptest) * RateLow);
					//All_PWM_left = ((ClsLoop_Speed - (Turn_PWM) * (tmptest)) - abs(Turn_PWM) * (tmptest) * RateLow);
					//All_PWM_right = ((ClsLoop_Speed + (Turn_PWM) * (tmptest)) - abs(Turn_PWM) * (tmptest) * RateLow);
					All_PWM_left = range_protect(All_PWM_left, ClsLoop_Speed - 4000, ClsLoop_Speed + 4000);
					All_PWM_right = range_protect(All_PWM_right, ClsLoop_Speed - 4000, ClsLoop_Speed + 4000);
				}else
				if(Flag_OpenLoop){
					All_PWM_left = OpenLoop_Speed - (Turn_PWM) * (tmptest);
					All_PWM_right = OpenLoop_Speed + (Turn_PWM) * (tmptest);
				}
				
				
				/*if(All_PWM_left < 0 && All_PWM_left > -1500){
					All_PWM_left = -1500;
				}else
				if(All_PWM_left > 0 && All_PWM_left < 1500){
					All_PWM_left = 1500;
				}
				if(All_PWM_right < 0 && All_PWM_right > -1500){
					All_PWM_right = -1500;
				}else
				if(All_PWM_right > 0 && All_PWM_right < 1500){
					All_PWM_right = 1500;
				}*/
				/*testpid.KPS = 10;
				testpid.KIS = 0.1;
				testpid.KDS = 0.1;
				testpid.PIDmax = 3000;
				testpid.PIDmin = -3000;
				testpid.SetValue = 1500 - adc_deviation * 50; // ����
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

            // 10ms���ƣ����ת��
            if(1 == Flag.T_Turn && Flag_OpenLoop != 1)
            {
                Flag.T_Turn = 0;
                Electromagnetism_Control();						// ��Ųɼ�����
				//Left_Corner_Adc = Left_Corner_Adc > 300 ? Left_Corner_Adc - 300 : 0;
				adc_deviation = 0
						+ Cha_BI_He_Sqrt(Left_Adc + Left_Corner_Adc, Right_Adc + Right_Corner_Adc, SideRate) 
						//+ Cha_BI_He_Sqrt(Left_Corner_Adc, Right_Corner_Adc, CornerRate) 
						+ Cha_BI_He_Sqrt(Left_Xie_Adc, Right_Xie_Adc, XieRate); //   9�� 1
				Turn_PWM = PID_Realize(&TurnPID, Turn_Pid[Turn_Suquence], adc_deviation, 0); //ת��̬PID
				Turn_PWM = -Turn_PWM;
				Turn_PWM = range_protect(Turn_PWM, -1250, 1250);
				Annulus_Analysis();								// Բ��ʶ����
				
                //Steering_Control_Out(Turn_PWM);				// �����������������ڲ����޷���
            }

            // 30ms���ƣ�TOF����
            if(1 == Flag.T_Distance)
            {
                Flag.T_Distance = 0;

                if(!Flag_Tof_Finish){
					//dis_test = GY_GetMeasureResult();
					//GY_GetMeasureResultTemp(dis_tmp_tmp);
					//GY_WriteStartMeasure();
					//Tof_Control();
					//dl1b_get_distance();
				}

				// ��δ��ɹ�һ�μ��
                    //Tof_Control();								// �ϰ��������
				
            }

            // 40ms���ƣ��µ����(IMU660)
            if(1 == Flag.T_IMU)
            {
                Flag.T_IMU = 0;
//								if(!Flag_Slope_Finish)
//									IMU_Control();
            }

            // 50ms���ƣ��ٶȱջ�����
            if(1 == Flag.T_Speed)
            {
                Flag.T_Speed = 0;
                //Speed_PWM = PID_Realize(&SpeedPID, Speed_Pid, real_speed, ClsLoop_Speed);		// �ٶ�λ��ʽPID
                //Speed_PWM = range_protect(Speed_PWM, -0, 4000);													// ע��������
            }
        }
    }
}

//���Ĺؼ��ı�־λ�ṹ���ʼ��
void Flag_Init(void)
{
    Flag.start_go = 0;
    Flag.T_Inmost = 0;
    Flag.T_Turn = 0;
    Flag.T_Distance = 0;
    Flag.T_Speed = 0;
    Flag.T_IMU = 0;
    Flag.STOP = 0;								// ͣ������
    Flag.OUT_Garage = 0;				  // �����־λ
    Flag.Game = 0;
}
