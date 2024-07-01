#ifndef __MYIIC_H__
#define __MYIIC_H__

#include "STC32Gxx.h"
#include "stdio.h"
#include "intrins.h"
#include "zf_delay.h"
//#include "string.h"
#define 	FLOAT_BYTE_NUM 	4
#define  	INT_BYTE_NUM	2
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

#define GUI_TYPE_SCENE_CHANGE			0
#define GUI_TYPE_SHOW_FLOAT_VALUE		1
#define GUI_TYPE_SHOW_INT32_VALUE		2
#define GUI_TYPE_EDIT_FLOAT_VALUE		3
#define GUI_TYPE_EDIT_INT32_VALUE		4

//浮点数存储共用体
typedef union
{
    float value;
    u8    byte[FLOAT_BYTE_NUM];
} storFloatData;
typedef struct GUIS{
	unsigned char names[3];
	float* floatval;
	int* intval;
	unsigned char p1, p2;
	unsigned char sceneGoTo;
	unsigned char type;
}GUIS;
extern int ver, iictmp;
extern GUIS guis[5][8];
//void WriteStartMeasure(void);
//u16  GetMeasureResult(void);

sbit    SDA = P4^1; //定义SDA
sbit    SCL = P4^0; //定义SCL

//sbit sda=P4^1;
//sbit scl=P4^0;

//u8 I2C_Start(void);
//u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length);
//u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 datas);

u8   GY_WriteStartMeasure(void);
u8   GY_GetMeasureResultTemp(u8 *distance);
int  GY_GetMeasureResult(u8 *distance);

u8 	  At24c02_Write_u8(u8 addr,u8 dat);
u8 	  At24c02_Write_int(u8 addr,int dat);
u8 	  At24c02_Write_float(u8 addr,float dat);


u8	  At24c02_Read_u8(u8 addr);
int   At24c02_Read_int(u8 addr);
float At24c02_Read_float(u8 addr);


u8 At24c02_Sequential_Read(u8 addr, u8 *dat, u8 len);
u8 At24c02_PageWrite(u8 addr, u8 *dat, u8 len);		//如果长度过长，会触发延时

void saveConfig(void);
void loadConfig(void);
void slinit(void);
//void MeasureStart(void);
//u16 GetMeasure(void);

/////UART
//extern void PrintString1(u8 *puts);

#define DELAY_TIME	10

#endif