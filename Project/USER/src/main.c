/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 			MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   			https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"
float a = 0.1;
/*
 * ϵͳƵ�ʣ��ɲ鿴board.h�е� FOSC �궨���޸ġ�
 * board.h�ļ���FOSC��ֵ����Ϊ0,������Զ�����ϵͳƵ��Ϊ33.1776MHZ
 * ��board_init��,�Ѿ���P54��������Ϊ��λ
 * �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
 */



void BUZZ_Init(void)
{
    gpio_mode(P6_7, GPO_PP);
    BUZZPin = 0;		// �ȹرշ�����
}




unsigned char FlagEnter = 0, FlagCancel = 0, FlagUp = 0, FlagDown = 0;
unsigned char LastFlagEnter = 0, LastFlagCancel = 0, LastFlagUp = 0, LastFlagDown = 0;
void KeySystem(void);
GUIS guis[5][8];
unsigned char guiNumber[5] = {5, 8, 8, 8, 6};
unsigned char guiSelect = 0, guiScene = 0, guiEdit = 0;
unsigned char guiTop = 0;
void Gui_TEST_Init(void);


void All_Init(void)
{
    DisableGlobalIRQ();											// �ر����ж�
    board_init();														// ��ʼ���Ĵ���,��ɾ���˾���롣
    Encoder_Init();													// ��������ʼ��
    Motor_Init();														// �����ʼ��
    init_Steer_PWM();												// �����ʼ��
    BUZZ_Init();														// �������ߵ�ƽ����
    Analog_Digital_Converter_Init();				// ADC��ʼ��
    //Tof_Init();															// ���ģ���ʼ��
    //imu660ra_init();												// ��̬��������ʼ��
    oled_init();														// ��ʼ��oled��Ļ
    pit_timer_ms(TIM_1, 5);  								// ʹ�ö�ʱ���������жϣ�ʱ��5msһ��
    wireless_uart_init();										// ���ߴ��ڳ�ʼ��: UART4_TX_P03   UART4_RX_P02  115200  TIM2
    //////////////////////////////////////////////////////////////////
    Flag_Init();														// Fuse��־λ��ʼ������0��
    PID_Parameter_Init(&TurnPID);						// ת��PID�ṹ���ʼ��
    PID_Parameter_Init(&SpeedPID);					// �ٶ�PID�ṹ���ʼ��
    EnableGlobalIRQ();											// �������ж�
    wireless_uart_send_buff("Init OK!\n", 9); // ���ߴ��ڷ��ͳ�ʼ�������Ϣ
	//Tof_Init();
	Gui_TEST_Init();
	slinit();
}

void main()
{
    //char txt[20];											// sprintf����
    //char mess[30];										// wireless����
    /*----���Ժ���(�ڲ���������ѭ��)----*/
    //	Test_Motor(1);			// 1:��ת  0:��ת
    // �ٶȲ���
    ClsLoop_Set_Speed  = 2700;						// �ջ��ٶȣ�����֮��
    ClsLoop_Speed = ClsLoop_Set_Speed;
    OpenLoop_Set_Speed = 2700;						// �����ٶȣ�����֮ǰ��
    OpenLoop_Speed = OpenLoop_Set_Speed;
    // ת�򻷲���
    Turn_Suquence = 0;										// ת��PID�±�
	vtest = 5;
    // ��������0���������  1�������ҳ���
    Default_Dir = 0;											// ��������⡢���Ϸ���һ��

    All_Init();												// ȫ��Ӳ����ʼ��
	/*while(1){
		KeySystem();
		delay_ms(500);
	}*/
	while(P60 == 0 || P64 == 0){
		go_motor(-2000, -2000);
		delay_ms(20);
		go_motor(2000, 2000);
		delay_ms(20);
	}
    while (1) {
		KeySystem();
		Flag.start_go = go_flag;
        /**********��ʾ5�����ֵ************/
//        if(!showing){
//			sprintf(txt, "Mid_Adc= %05d", adc_date[6]);
//			oled_p6x8str(1, 1, txt);   // ��ʾ
//			sprintf(txt, "Left_Adc= %05d", adc_date[0]);
//			oled_p6x8str(1, 4, txt);   // ��ʾ
//			sprintf(txt, "Right_Adc= %05d", adc_date[1]);
//			oled_p6x8str(1, 5, txt);   // ��ʾ
//			sprintf(txt, "Left_Xie= %05d", adc_date[2]);
//			oled_p6x8str(1, 6, txt);   // ��ʾ
//			sprintf(txt, "Right_Xie= %05d", adc_date[3]);
//			oled_p6x8str(1, 7, txt); 	 // ��ʾ
//			sprintf(txt, "Left_Corner= %05d", adc_date[4]);
//			oled_p6x8str(1, 2, txt);   // ��ʾ
//			sprintf(txt, "Right_Corner= %05d", adc_date[5]);
//			oled_p6x8str(1, 3, txt);   // ��ʾ
//		}else{
//			sprintf(txt, "adc_deviation = %05d", adc_deviation);
//			oled_p6x8str(1, 1, txt);   // ��ʾ
//			sprintf(txt, "Speed_PWM = %05d", Speed_PWM);
//			oled_p6x8str(1, 2, txt);   // ��ʾ
//			sprintf(txt, "Turn_PWM = %05d", Turn_PWM);
//			oled_p6x8str(1, 3, txt);   // ��ʾ
//			sprintf(txt, "LEFT_PWM = %05d", All_PWM_left);
//			oled_p6x8str(1, 4, txt);   // ��ʾ
//			sprintf(txt, "RIGHT_PWM = %05d", All_PWM_right);
//			oled_p6x8str(1, 5, txt);   // ��ʾ
//		}
		
        //			sprintf(txt,"adc_deviation= %05d",adc_deviation);
        //			oled_p6x8str(1, 8, txt); // ��ʾ

        /* ���Ա����� */
        //			sprintf(mess,"%d,%d,%d\n",right_speed,left_speed,real_speed);			// ������

        /* �����ٶȱջ� */
        //			sprintf(mess,"%d,%d\n",real_speed,Speed_PWM);
        //			wireless_uart_send_buff(mess,30);				// �����ٶȷ���

        /* ����IMU660 */
        //				sprintf(mess,"%f\n",Slope_gyro);
        //				wireless_uart_send_buff(mess,30);

        /* ���� */
		//go_motor(2000, 2000);
		
        if (Flag.Game == 0) {
            LightOn;
            delay_ms(500);
            //OutInGarage(Default_Dir, 2000);	// ����
            LightOff;
            Flag.Game = 1;									// ��ֹ�ٴν���
            int_OK = 1;
            Flag_Slope = 1  ;
            go_flag = 1;							// ִ��Fuseȫ�ֿ���
        }

        // �Ȳ����뷽����ԣ���Ҫ����ʱ��0 -> 1
        #if 0
        /* ͣ�� */
        else if (Flag.Game == 1) {
            STOP_Analyse();    // ͣ����� + ���
        }

        #endif
    }
}

void Gui_TEST_Init(void){
	float *PID;
	guis[0][0].type = GUI_TYPE_SCENE_CHANGE;
	guis[0][1].type = GUI_TYPE_SCENE_CHANGE;
	guis[0][2].type = GUI_TYPE_SCENE_CHANGE;
	guis[0][3].type = GUI_TYPE_SCENE_CHANGE;
	guis[0][4].type = GUI_TYPE_EDIT_INT32_VALUE;
	strcpy(guis[0][0].names, "ADC");
	strcpy(guis[0][1].names, "SP");
	strcpy(guis[0][2].names, "PID");
	strcpy(guis[0][3].names, "RAT");
	strcpy(guis[0][4].names, "GO");
	guis[0][0].sceneGoTo = 1;
	guis[0][1].sceneGoTo = 2;
	guis[0][2].sceneGoTo = 3;
	guis[0][3].sceneGoTo = 4;
	guis[0][4].intval = &go_flag;
	guis[0][4].p1 = 1; guis[0][4].p2 = 1;
	
	guis[1][0].type = GUI_TYPE_SCENE_CHANGE;
	guis[1][1].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[1][2].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[1][3].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[1][4].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[1][5].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[1][6].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[1][7].type = GUI_TYPE_SHOW_INT32_VALUE;
	strcpy(guis[1][0].names, "BK");
	strcpy(guis[1][1].names, "M");
	strcpy(guis[1][2].names, "L");
	strcpy(guis[1][3].names, "R");
	strcpy(guis[1][4].names, "LX");
	strcpy(guis[1][5].names, "RX");
	strcpy(guis[1][6].names, "LC");
	strcpy(guis[1][7].names, "RC");
	guis[1][0].sceneGoTo = 0;
	guis[1][1].intval = (int*)(&adc_date[6]);
	guis[1][2].intval = (int*)(&adc_date[0]);
	guis[1][3].intval = (int*)(&adc_date[1]);
	guis[1][4].intval = (int*)(&adc_date[2]);
	guis[1][5].intval = (int*)(&adc_date[3]);
	guis[1][6].intval = (int*)(&adc_date[4]);
	guis[1][7].intval = (int*)(&adc_date[5]);
	
	guis[2][0].type = GUI_TYPE_SCENE_CHANGE;
	guis[2][1].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[2][2].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[2][3].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[2][4].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[2][5].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[2][6].type = GUI_TYPE_SHOW_INT32_VALUE;
	guis[2][7].type = GUI_TYPE_SHOW_INT32_VALUE;
	strcpy(guis[2][0].names, "BK");
	strcpy(guis[2][1].names, "ADO");
	strcpy(guis[2][2].names, "TPM");
	strcpy(guis[2][3].names, "SPM");
	strcpy(guis[2][4].names, "LPM");
	strcpy(guis[2][5].names, "RPM");
	strcpy(guis[2][6].names, "lre");
	strcpy(guis[2][7].names, "rre");
	guis[2][0].sceneGoTo = 0;
	guis[2][1].intval = &adc_deviation;
	guis[2][2].intval = &Turn_PWM;
	guis[2][3].intval = &Speed_PWM;
	guis[2][4].intval = &All_PWM_left;
	guis[2][5].intval = &All_PWM_right;
	guis[2][6].intval = &left_speed;
	guis[2][7].intval = &right_speed;
	
	guis[3][0].type = GUI_TYPE_SCENE_CHANGE;
	guis[3][1].type = GUI_TYPE_EDIT_FLOAT_VALUE;
	guis[3][2].type = GUI_TYPE_EDIT_FLOAT_VALUE;
	guis[3][3].type = GUI_TYPE_EDIT_FLOAT_VALUE;
	guis[3][4].type = GUI_TYPE_EDIT_FLOAT_VALUE;
	guis[3][5].type = GUI_TYPE_EDIT_FLOAT_VALUE;
	guis[3][6].type = GUI_TYPE_EDIT_FLOAT_VALUE;
	guis[3][7].type = GUI_TYPE_EDIT_INT32_VALUE;
	strcpy(guis[3][0].names, "BK");
	strcpy(guis[3][1].names, "KP");	//ʵ���ϲ���ȫ��kp
	strcpy(guis[3][2].names, "KP2");	//???
	strcpy(guis[3][3].names, "KI");
	strcpy(guis[3][4].names, "KD");
	strcpy(guis[3][5].names, "KT");
	strcpy(guis[3][6].names, "KIS");
	strcpy(guis[3][7].names, "SP");
	guis[3][0].sceneGoTo = 0;
	guis[3][1].floatval = &vtest;
	PID = Turn_Pid[Turn_Suquence];
	guis[3][2].floatval = &(PID[KP]);
	guis[3][3].floatval = &(PID[KI]);
	guis[3][4].floatval = &(PID[KD]);
	guis[3][5].floatval = &(PID[KT]);
	guis[3][6].floatval = &ki_test;
	guis[3][7].intval = &(ClsLoop_Speed);
	guis[3][1].p1 = 1; guis[3][1].p2 = 1;
	guis[3][2].p1 = 1; guis[3][2].p2 = 10;
	guis[3][3].p1 = 1; guis[3][3].p2 = 10;
	guis[3][4].p1 = 10; guis[3][4].p2 = 1;
	guis[3][5].p1 = 10; guis[3][5].p2 = 1;
	guis[3][6].p1 = 1; guis[3][6].p2 = 10;
	guis[3][7].p1 = 100; guis[3][7].p2 = 1;
	
	guis[4][0].type = GUI_TYPE_SCENE_CHANGE;
	guis[4][1].type = GUI_TYPE_EDIT_INT32_VALUE;
	guis[4][2].type = GUI_TYPE_EDIT_INT32_VALUE;
	guis[4][3].type = GUI_TYPE_EDIT_INT32_VALUE;
	guis[4][4].type = GUI_TYPE_EDIT_FLOAT_VALUE;
	guis[4][5].type = GUI_TYPE_EDIT_FLOAT_VALUE;
	strcpy(guis[4][0].names, "BK");
	strcpy(guis[4][1].names, "Sid");
	strcpy(guis[4][2].names, "Cor");
	strcpy(guis[4][3].names, "Xie");
	strcpy(guis[4][4].names, "RL");
	strcpy(guis[4][5].names, "RH");
	guis[4][0].sceneGoTo = 0;
	guis[4][1].intval = &SideRate;
	guis[4][2].intval = &CornerRate;
	guis[4][3].intval = &XieRate;
	guis[4][4].floatval = &RateLow;
	guis[4][5].floatval = &RateUp;
	guis[4][1].p1 = 50; guis[4][1].p2 = 1;
	guis[4][2].p1 = 50; guis[4][2].p2 = 1;
	guis[4][3].p1 = 50; guis[4][3].p2 = 1;
	guis[4][4].p1 = 1; guis[4][4].p2 = 10;
	guis[4][5].p1 = 1; guis[4][5].p2 = 10;
	return;
}

void keyStateChange(unsigned char* FlagButtom, unsigned char ButtomPin){
	if(*FlagButtom == 0){
		if(ButtomPin == 0){
			delay_ms(10);
			if(ButtomPin == 0){
				*FlagButtom = 1;
			}
		}
	}else{
		if(ButtomPin == 1){
			delay_ms(10);
			if(ButtomPin == 1){
				*FlagButtom = 0;
			}
		}
	}
	return;
}

char keyPressed(unsigned char FlagButtom, unsigned char LastFlagButtom){
	return (FlagButtom == 1 && FlagButtom != LastFlagButtom);
}

void moveGuiSelection(char dir){
	if(!dir){
		if(guiSelect == 0){
			return;
		}
		guiSelect--;
		if(guiSelect < guiTop){
			guiTop = guiSelect;
		}
	}else{
		if(guiSelect == guiNumber[guiScene] - 1){
			return;
		}
		guiSelect++;
		if(guiSelect > guiTop + 6){
			guiTop = guiSelect - 6;
		}
	}
	return;
}

void KeySystem(void){
	int i = 0;
	char txt[20];
	keyStateChange(&FlagEnter, (BUTTOM_ENTER == 0) ? 0 : 1);
	keyStateChange(&FlagCancel, (BUTTOM_CANCEL == 0) ? 0 : 1);
	keyStateChange(&FlagUp, (BUTTOM_UP == 0) ? 0 : 1);
	keyStateChange(&FlagDown, (BUTTOM_DOWN == 0) ? 0 : 1);
	/*if(FlagEnter == 1 && FlagEnter != LastFlagEnter){
		oled_fill(0x00);
		showing = !showing;
	}*/
	if(!guiEdit){
		if(keyPressed(FlagUp, LastFlagUp)){
			oled_fill(0x00);
			moveGuiSelection(0);
		}else
		if(keyPressed(FlagDown, LastFlagDown)){
			oled_fill(0x00);
			moveGuiSelection(1);
		}
	}
	for(i = guiTop; i < guiNumber[guiScene], i < guiTop + 7; i++){
		if(guiSelect == i){
			if(guis[guiScene][i].type == GUI_TYPE_SCENE_CHANGE){
				if(keyPressed(FlagEnter, LastFlagEnter)){
					oled_fill(0x00);
					guiScene = guis[guiScene][i].sceneGoTo;
					guiTop = 0;
					guiSelect = 0;
				}
				sprintf(txt, "*%s", guis[guiScene][i].names);
			}else
			if(guis[guiScene][i].type == GUI_TYPE_SHOW_FLOAT_VALUE){
				sprintf(txt, "*%-8s %.2f ", guis[guiScene][i].names, *(guis[guiScene][i].floatval));
			}else
			if(guis[guiScene][i].type == GUI_TYPE_SHOW_INT32_VALUE){
				sprintf(txt, "*%-8s %05d ", guis[guiScene][i].names, *(guis[guiScene][i].intval));
			}else
			if(guis[guiScene][i].type == GUI_TYPE_EDIT_FLOAT_VALUE){
				if(guiEdit == 0){
					if(keyPressed(FlagEnter, LastFlagEnter)){
						oled_fill(0x00);
						guiEdit = 1;
					}
					sprintf(txt, "*%-8s %.2f ", guis[guiScene][i].names, *(guis[guiScene][i].floatval));
				}else{
					if(keyPressed(FlagEnter, LastFlagEnter)){
						oled_fill(0x00);
						guiEdit = 0;
						saveConfig();
					}else
					if(keyPressed(FlagUp, LastFlagUp)){
						oled_fill(0x00);
						*(guis[guiScene][i].floatval) -= (float)guis[guiScene][i].p1 / (float)guis[guiScene][i].p2;
					}else
					if(keyPressed(FlagDown, LastFlagDown)){
						oled_fill(0x00);
						*(guis[guiScene][i].floatval) += (float)guis[guiScene][i].p1 / (float)guis[guiScene][i].p2;
					}
					sprintf(txt, "*%-8s<%.2f>", guis[guiScene][i].names, *(guis[guiScene][i].floatval));
				}
			}else
			if(guis[guiScene][i].type == GUI_TYPE_EDIT_INT32_VALUE){
				if(guiEdit == 0){
					if(keyPressed(FlagEnter, LastFlagEnter)){
						oled_fill(0x00);
						guiEdit = 1;
					}
					sprintf(txt, "*%-8s %05d ", guis[guiScene][i].names, *(guis[guiScene][i].intval));
				}else{
					if(keyPressed(FlagEnter, LastFlagEnter)){
						oled_fill(0x00);
						guiEdit = 0;
						saveConfig();
					}else
					if(keyPressed(FlagUp, LastFlagUp)){
						oled_fill(0x00);
						*(guis[guiScene][i].intval) -= (float)guis[guiScene][i].p1 / (float)guis[guiScene][i].p2;
					}else
					if(keyPressed(FlagDown, LastFlagDown)){
						oled_fill(0x00);
						*(guis[guiScene][i].intval) += (float)guis[guiScene][i].p1 / (float)guis[guiScene][i].p2;
					}
					sprintf(txt, "*%-8s<%05d>", guis[guiScene][i].names, *(guis[guiScene][i].intval));
				}
			}
			oled_p6x8str(1, (uint8)(i - guiTop + 1), txt);
		}else{
			if(guis[guiScene][i].type == GUI_TYPE_SCENE_CHANGE){
				sprintf(txt, " %-s", guis[guiScene][i].names);
			}else
			if(guis[guiScene][i].type == GUI_TYPE_SHOW_FLOAT_VALUE){
				sprintf(txt, " %-8s %.2f ", guis[guiScene][i].names, *(guis[guiScene][i].floatval));
			}else
			if(guis[guiScene][i].type == GUI_TYPE_SHOW_INT32_VALUE){
				sprintf(txt, " %-8s %05d ", guis[guiScene][i].names, *(guis[guiScene][i].intval));
			}else
			if(guis[guiScene][i].type == GUI_TYPE_EDIT_FLOAT_VALUE){
				if(guiEdit == 0){
					sprintf(txt, " %-8s %.2f ", guis[guiScene][i].names, *(guis[guiScene][i].floatval));
				}else{
					sprintf(txt, " %-8s<%.2f>", guis[guiScene][i].names, *(guis[guiScene][i].floatval));
				}
			}else
			if(guis[guiScene][i].type == GUI_TYPE_EDIT_INT32_VALUE){
				if(guiEdit == 0){
					sprintf(txt, " %-8s %05d ", guis[guiScene][i].names, *(guis[guiScene][i].intval));
				}else{
					sprintf(txt, " %-8s<%05d>", guis[guiScene][i].names, *(guis[guiScene][i].intval));
				}
			}
			oled_p6x8str(1, (uint8)(i - guiTop + 1), txt);
		}
	}
	/*At24c02_Write_float(0x04, a++);
	a = At24c02_Read_float(0x04);*/
	if(guiScene == 0){
		sprintf(txt, "%d, %d", (P60 == 1) ? 1 : 0, (P64 == 1) ? 1 : 0);
		oled_p6x8str(1, 7, txt);
	}
	LastFlagEnter = FlagEnter;
	LastFlagCancel = FlagCancel;
	LastFlagUp = FlagUp;
	LastFlagDown = FlagDown;
}



