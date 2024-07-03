/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 			MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   			https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"
float a = 0.1;
/*
 * 系统频率，可查看board.h中的 FOSC 宏定义修改。
 * board.h文件中FOSC的值设置为0,则程序自动设置系统频率为33.1776MHZ
 * 在board_init中,已经将P54引脚设置为复位
 * 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */



void BUZZ_Init(void)
{
    gpio_mode(P6_7, GPO_PP);
    BUZZPin = 0;		// 先关闭蜂鸣器
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
    DisableGlobalIRQ();											// 关闭总中断
    board_init();														// 初始化寄存器,勿删除此句代码。
    Encoder_Init();													// 编码器初始化
    Motor_Init();														// 电机初始化
    init_Steer_PWM();												// 舵机初始化
    BUZZ_Init();														// 蜂鸣器高电平点亮
    Analog_Digital_Converter_Init();				// ADC初始化
    //Tof_Init();															// 测距模块初始化
    //imu660ra_init();												// 姿态传感器初始化
    oled_init();														// 初始化oled屏幕
    pit_timer_ms(TIM_1, 5);  								// 使用定时器做周期中断，时间5ms一次
    wireless_uart_init();										// 无线串口初始化: UART4_TX_P03   UART4_RX_P02  115200  TIM2
    //////////////////////////////////////////////////////////////////
    Flag_Init();														// Fuse标志位初始化（置0）
    PID_Parameter_Init(&TurnPID);						// 转向PID结构体初始化
    PID_Parameter_Init(&SpeedPID);					// 速度PID结构体初始化
    EnableGlobalIRQ();											// 开启总中断
    wireless_uart_send_buff("Init OK!\n", 9); // 无线串口发送初始化完成信息
	//Tof_Init();
	Gui_TEST_Init();
	slinit();
}

void main()
{
    //char txt[20];											// sprintf容器
    //char mess[30];										// wireless容器
    /*----测试函数(内部本身有死循环)----*/
    //	Test_Motor(1);			// 1:正转  0:反转
    // 速度参数
    ClsLoop_Set_Speed  = 2700;						// 闭环速度（避障之后）
    ClsLoop_Speed = ClsLoop_Set_Speed;
    OpenLoop_Set_Speed = 2700;						// 开环速度（避障之前）
    OpenLoop_Speed = OpenLoop_Set_Speed;
    // 转向环参数
    Turn_Suquence = 0;										// 转向PID下标
	vtest = 5;
    // 发车方向（0：左入左出  1：右入右出）
    Default_Dir = 0;											// 发车、入库、避障方向一致

    All_Init();												// 全体硬件初始化
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
        /**********显示5个电感值************/
//        if(!showing){
//			sprintf(txt, "Mid_Adc= %05d", adc_date[6]);
//			oled_p6x8str(1, 1, txt);   // 显示
//			sprintf(txt, "Left_Adc= %05d", adc_date[0]);
//			oled_p6x8str(1, 4, txt);   // 显示
//			sprintf(txt, "Right_Adc= %05d", adc_date[1]);
//			oled_p6x8str(1, 5, txt);   // 显示
//			sprintf(txt, "Left_Xie= %05d", adc_date[2]);
//			oled_p6x8str(1, 6, txt);   // 显示
//			sprintf(txt, "Right_Xie= %05d", adc_date[3]);
//			oled_p6x8str(1, 7, txt); 	 // 显示
//			sprintf(txt, "Left_Corner= %05d", adc_date[4]);
//			oled_p6x8str(1, 2, txt);   // 显示
//			sprintf(txt, "Right_Corner= %05d", adc_date[5]);
//			oled_p6x8str(1, 3, txt);   // 显示
//		}else{
//			sprintf(txt, "adc_deviation = %05d", adc_deviation);
//			oled_p6x8str(1, 1, txt);   // 显示
//			sprintf(txt, "Speed_PWM = %05d", Speed_PWM);
//			oled_p6x8str(1, 2, txt);   // 显示
//			sprintf(txt, "Turn_PWM = %05d", Turn_PWM);
//			oled_p6x8str(1, 3, txt);   // 显示
//			sprintf(txt, "LEFT_PWM = %05d", All_PWM_left);
//			oled_p6x8str(1, 4, txt);   // 显示
//			sprintf(txt, "RIGHT_PWM = %05d", All_PWM_right);
//			oled_p6x8str(1, 5, txt);   // 显示
//		}
		
        //			sprintf(txt,"adc_deviation= %05d",adc_deviation);
        //			oled_p6x8str(1, 8, txt); // 显示

        /* 调试编码器 */
        //			sprintf(mess,"%d,%d,%d\n",right_speed,left_speed,real_speed);			// 编码器

        /* 调试速度闭环 */
        //			sprintf(mess,"%d,%d\n",real_speed,Speed_PWM);
        //			wireless_uart_send_buff(mess,30);				// 右轮速度发送

        /* 调试IMU660 */
        //				sprintf(mess,"%f\n",Slope_gyro);
        //				wireless_uart_send_buff(mess,30);

        /* 发车 */
		//go_motor(2000, 2000);
		
        if (Flag.Game == 0) {
            LightOn;
            delay_ms(500);
            //OutInGarage(Default_Dir, 2000);	// 出库
            LightOff;
            Flag.Game = 1;									// 防止再次进入
            int_OK = 1;
            Flag_Slope = 1  ;
            go_flag = 1;							// 执行Fuse全局控制
        }

        // 先不编译方便调试，需要编译时将0 -> 1
        #if 0
        /* 停车 */
        else if (Flag.Game == 1) {
            STOP_Analyse();    // 停车检测 + 入库
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
	strcpy(guis[3][1].names, "KP");	//实际上不完全是kp
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



