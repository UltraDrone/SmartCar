#include "Direction.h"

// ADC�ɼ�ֵ
// ��������0~4�ֱ�Ϊ
// ��������ֵ  �ҵ������ֵ  ��б�������ֵ  ��б�������ֵ  �м�������ֵ
uint16  adc_date[7];                						 // �����вɼ�ֵ ԭʼֵ 5�����

// ADC�޷�ֵ
uint16 adc_max[7] = {2000, 2000, 2200, 2200, 2200, 2200, 2000};      // ���ֵ
uint16 adc_min[7] = {0, 0, 100, 100, 100, 100, 100};               		 // ��Сֵ  (����һ����Ҫ)

int16  adc_deviation;           											 	 // ���ƫ��
int go_flag;
// ADC�ɼ�����ֵ
int16 Left_Adc = 0, Right_Adc = 0, Mid_Adc = 0, Left_Xie_Adc = 0, Right_Xie_Adc = 0, Left_Corner_Adc = 0, Right_Corner_Adc = 0;		//���ֵ

/****************ADC��ʼ��**************************
��  ����void ADC_Init(void)
��  �ܣ�ADCͨ�����ų�ʼ��
��  ������
˵  ����ADC_Init(ADC_P10,ADC_SYSclk_DIV_2);//��ʼ��P1.0ΪADC����,ADCʱ��Ƶ�ʣ�SYSclk/2
��  �ţ���ͷ�ļ�����
����ֵ����
***************************************************/
void Analog_Digital_Converter_Init(void)
{
    adc_init(Left_ADC_Pin, ADC_SYSclk_DIV_2); //��ʼ��ADC����
    adc_init(LeftXie_ADC_Pin, ADC_SYSclk_DIV_2); //��ʼ��ADC����
    adc_init(Mid_ADC_Pin, ADC_SYSclk_DIV_2); //��ʼ��ADC����
    adc_init(RightXie_ADC_Pin, ADC_SYSclk_DIV_2); //��ʼ��ADC����
    adc_init(Right_ADC_Pin, ADC_SYSclk_DIV_2); //��ʼ��ADC����
	adc_init(LeftCorner_ADC_Pin,ADC_SYSclk_DIV_2);//��ʼ��ADC����
	adc_init(RightCorner_ADC_Pin,ADC_SYSclk_DIV_2);//��ʼ��ADC����
}


/****************��вɼ�**************************
��  ����void ADC_MAX_Collect(void)
��  �ܣ�
��  ������
˵  �������ÿ⺯�����Լ�д�ģ�    adc_mean_filter (ADC_P10, ADC_12BIT, 10)
����ֵ����
**************************************************/
void ADC_MAX_Collect(void)
{
    int i, j;

    for(i = 600; i > 0; i--)
    {
        adc_date[0] = adc_mean_filter(Left_ADC_Pin, ADC_12BIT, 2);
        adc_date[1] = adc_mean_filter(Right_ADC_Pin, ADC_12BIT, 2);
        adc_date[2] = adc_mean_filter(LeftXie_ADC_Pin, ADC_12BIT, 2);
        adc_date[3] = adc_mean_filter(RightXie_ADC_Pin, ADC_12BIT, 2);
		adc_date[4] = adc_mean_filter(LeftCorner_ADC_Pin, ADC_12BIT, 2);
        adc_date[5] = adc_mean_filter(RightCorner_ADC_Pin, ADC_12BIT, 2);
        adc_date[6] = adc_mean_filter(Mid_ADC_Pin, ADC_12BIT, 2);

        for(j = 0; j < 7; j++)									// �����������޷�
        {
            if(adc_date[j] >= adc_max[j])							// ���޷�
                adc_max[j] = adc_date[j];

			if(adc_date[j] <= adc_min[j])							// ���޷�
				adc_min[j] = adc_date[j];
            delay_ms(2);//��ʱ�ɼ�
        }
    }
}

/****************��вɼ�**************************
��  ����void AD_Date_Fitier()
��  �ܣ��Ե�в�ֵ����ð�����򻬶��˲�
��  ������
˵  ����12bit  4096   ���ÿ⺯�����Լ�д�ģ�    adc_mean_filter(ADC_P10, ADC_12BIT, 10)
����ֵ����
**************************************************/
#define FILTER_N 5 //�˲����

void AD_Date_Fitier(void)
{
    uint8 i;
    int16 filter_buf_L[FILTER_N];  //����д�������
    int16 filter_buf_LC[FILTER_N]; //��б��д�������
	int16 filter_buf_LX[FILTER_N]; //��б��д�������
    int16 filter_buf_M[FILTER_N];  //�к��д�������
    int16 filter_buf_R[FILTER_N];  //�Һ��д�������
    int16 filter_buf_RC[FILTER_N];  //��б��д�������
	int16 filter_buf_RX[FILTER_N]; //��б��д�������

    //--------�����˲�--------------
    for(i = 0; i < FILTER_N; i++)	//��ֵ
    {
        filter_buf_L[i]  = adc_mean_filter(Left_ADC_Pin, ADC_12BIT, 10); //���
        filter_buf_LX[i] = adc_mean_filter(LeftXie_ADC_Pin, ADC_12BIT, 5); //��б
		filter_buf_LC[i] = adc_mean_filter(LeftCorner_ADC_Pin, ADC_12BIT, 5); //��б
        filter_buf_M[i]  = adc_mean_filter(Mid_ADC_Pin, ADC_12BIT, 5); //�м�
        filter_buf_RC[i] = adc_mean_filter(RightCorner_ADC_Pin, ADC_12BIT, 5);  //��б
		filter_buf_RX[i] = adc_mean_filter(RightXie_ADC_Pin, ADC_12BIT, 5);//��б
        filter_buf_R[i]  = adc_mean_filter(Right_ADC_Pin, ADC_12BIT, 10); //�Һ�
    }
	//13 14 16 05 01 00
	//567321

    //--------ð������ȥ��ֵ��ƽ��---------
    adc_date[0] = I_Median_Average_Filter(filter_buf_L);  //��    3600
    adc_date[1] = I_Median_Average_Filter(filter_buf_R);  //��    3600
    adc_date[2] = I_Median_Average_Filter(filter_buf_LX); //��б  3000
    adc_date[3] = I_Median_Average_Filter(filter_buf_RX); //��б
	adc_date[4] = I_Median_Average_Filter(filter_buf_LC); //��б  3000
    adc_date[5] = I_Median_Average_Filter(filter_buf_RC); //��б
    adc_date[6] = I_Median_Average_Filter(filter_buf_M);  //�м�  4050

    Left_Adc = adc_date[0];							      //��������ֵ
    Right_Adc = adc_date[1];						      //�ҵ������ֵ
    Left_Xie_Adc = adc_date[2];						      //��б�������ֵ
    Right_Xie_Adc = adc_date[3];					      //��б�������ֵ
	Left_Corner_Adc = adc_date[4];
	Right_Corner_Adc = adc_date[5];
	Right_Corner_Adc = Right_Corner_Adc / 2;
    Mid_Adc = adc_date[6];							      //�м�������ֵ
}

void Protect_Anticollision(void)
{
    if(Left_Adc < 100 && Right_Adc < 100){
        //Flag.start_go = 0;
		go_flag = 0;
	}else
	if(Left_Adc > 150 || Right_Adc > 150){
		//Flag.start_go = 1;
		go_flag = 1;
	}
}


/*************************************
������void Electromagnetism_Control(void)
���ܣ���ſ���
��������
˵����
**************************************/
void Electromagnetism_Control(void)
{
    AD_Date_Fitier();                  // ��вɼ����� ð�����򻬶��˲�
    
    /// normalize_date();               // �ɼ���й�һ��  0--100 �����ã�δ�ҵ�ԭ�򣬲�Ҫ��һ����
    Protect_Anticollision();						// ����
}

/*****************�����������***********************
������void  annulus_analysis()
���ܣ��б𻷵�����
��������
˵����
����ֵ��
���ڣ�
ԭ���ǳ��򵥣���Բ�����жϵ����ֵ���������趨һ���̶����ٶȵ��٣�
      ͨ��������ʱ�ó�������ǰ��һ�����루��ֹ��·�磩��Ȼ��̶����ֱ�ӿ��ƶ����ǣ�
      Ȼ��Ҳ��ͨ��������ʱ����ֻҪ�ѳ���ս�ȥ�ˣ����Ǿͻָ�����ѭ��������Ҫ����������⣬����������ͨ�������ǻ��߱�����������ʱ�ѱ�־λ�����
      Ȼ���ֹ���Ӷ��������ǰ�ת��pid������С�ܶ࣬���ٶȻָ��������ɡ�
      �ڵ���ʱ����ֻ������Ǹ���ǵ���ʱ�ͼ������ߵ���ʱ�����������ù�

������룬�еı�����������൱��û�����ã��ɲο�
�����ο�������������������
****************************************************/
//uint8 annulus_sucess = 0;										// �뻷�ɹ�����
//uint8	Left_annulus = 0;											// �󻷱�־λ
uint8 Flag_Right_annulus = 0;									// �һ���־λ
uint8 PreFlag_Right_annulus = 0;             	// �һ�Ԥ�б�־λ
// ����Բ��
void Right_Annulus(void)
{
    if(Flag_Right_annulus == 0 && PreFlag_Right_annulus == 0 && Left_Xie_Adc > 3500 &&
            Left_Adc > 3500 && Mid_Adc > 3500)
        PreFlag_Right_annulus = 1;

//				LightOn;

    if(PreFlag_Right_annulus >= 1 && PreFlag_Right_annulus < 30 /*&& Right_Xie_Adc > 1900 && Right_Adc > 2500*/ && Flag_Right_annulus == 0)
    {
        BUZZOn;
        // LightOn;
        PreFlag_Right_annulus++;
		if(PreFlag_Right_annulus >= 30){
			BUZZOff;
			Flag_Right_annulus = 1;
		}
		Turn_PWM = 1000;
		//go_motor(0, 4000);
        //pwm_duty(Steer_Pin, 700);
        //delay_ms(300);
        
    }
}

// ����Բ��
void Left_Annulus(void)
{

}
uint8 Flag_circleLand = 0;
void circleLand(void){
	if(Left_Xie_Adc > 3200 && Left_Adc > 3200 && Left_Corner_Adc > 3200
		&& Mid_Adc > 3200 
		&& Right_Xie_Adc > 3200 && Right_Adc > 3200 && Right_Corner_Adc > 3200 && Flag_circleLand == 0){
		Flag_circleLand = 1;
	}
	if(Flag_circleLand > 1 && Flag_circleLand < 15){
		BUZZOn;
        Flag_circleLand++;
		if(Flag_circleLand >= 15){
			Flag_circleLand = 0;
			BUZZOff;
		}
		Turn_PWM = 0;
	}
}

void Annulus_Analysis(void)
{
    Right_Annulus();
    Left_Annulus();
	circleLand();
}


/***********************************�����ʼ��*****************************************
������void init_Steer_PWM(void)
��������
˵������ĸ10000��ʹ�ã������޸������޸Ķ�Ӧ�궨�弴��
       pwm_init(PWM0_P00, 100, 5000);     //��ʼ��PWM0  ʹ������P0.0  ���PWMƵ��100HZ   ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//		 PWM_DUTY_MAX��zf_pwm.h�ļ��� 				Ĭ��Ϊ10000
*
*ע�⣬�ȵ��ڶ����������ΪSD05����Ƶ��Ϊ200hz ,������ΪS3010,Ƶ����Ϊ50hz
*Ƶ��ȷ�����Ȱ�ռ�ձȷ�ĸ����PWM_DUTY_MAXȷ����һ�������޸���
*Ȼ��Ϳ�ʼ���ڶ���ˣ���ռ�ձȵķ��ӣ������õĺ���������Ǹ����������ݾ�����һ�£������1/20��ռ�ձȣ�Ȼ����������������
*���㹫ʽ����ֵռ�ձȴ����7.5% ����Ƶ�ʾ��ȶ��й�ϵ�� 20ms(1.5ms�ߵ�ƽ)
����ֵ����
**************************************************************************************/
void init_Steer_PWM(void)
{
    pwm_init(Steer_Pin, 50, Steer_Duty_Midle);   //��ʼ�����  ���PWMƵ��50HZ����������ֵ
}


/*******************���ת��������*************************
������void Steering_Control_Out(int16 duty)
���ܣ�
��������
˵�������ת�����    ע����ö����ֵ�����Ҽ���Ҳ��������Ҫ�޸�����ĺ궨��
����ֵ��
************************************************************/
void Steering_Control_Out(int16 duty)
{
    duty = Steer_Duty_Midle + duty;													// �ڶ����ֵ�Ļ����Ͻ���ƫ��

    if(duty >= Steer_Duty_Max) 															// ��ǹ���ͼ���
    {
        duty = Steer_Duty_Max;
        ClsLoop_Speed  -=  100;																// �����ǿ��ջ�״̬������
        OpenLoop_Speed -=  100;
    }
    else		// ������ԭ�����ٶ���ʻ
    {
        OpenLoop_Speed = OpenLoop_Set_Speed;
        ClsLoop_Speed = ClsLoop_Set_Speed;
    }

    if(duty <= Steer_Duty_Min)																// ��ǹ���ͼ���
    {
        duty = Steer_Duty_Min;
        ClsLoop_Speed  -=  100;																// �����ǿ��ջ�״̬������
        OpenLoop_Speed -=  100;
    }
    else	// ������ԭ�����ٶ���ʻ
    {
        OpenLoop_Speed = OpenLoop_Set_Speed;
        ClsLoop_Speed = ClsLoop_Set_Speed;
    }

    pwm_duty(Steer_Pin, duty);
}
