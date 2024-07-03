#include "MyIIC.h"

int ver = 13;
int iictmp;

void I2C_Delay1us(u8 DelayTime)	//@22.1184MHz
{
	do
	{
		unsigned long edata i;

		//_nop_();
		i = 7UL;
		while (i) i--;
		
	}while(DelayTime --);
}


/****************************/
u8 I2C_Start(void)               //start the I2C, SDA High-to-low when SCL is high
{
    SDA = 1;
    I2C_Delay1us(DELAY_TIME);
    SCL = 1;
    I2C_Delay1us(DELAY_TIME);
	if(!SDA)  { return 0;}//SDA线为低电平则总线忙,退出
    SDA = 0;
    I2C_Delay1us(DELAY_TIME);
	if(SDA) return 0;//SDA线为高电平则总线出错,退出
    SCL = 0;
    I2C_Delay1us(DELAY_TIME);
	//PrintString1("SDA == 1");
    return 1;
}       

void I2C_Stop(void)                 //STOP the I2C, SDA Low-to-high when SCL is high
{
    SDA = 0;
	SCL = 0;
    I2C_Delay1us(DELAY_TIME);
    SCL = 1;
    I2C_Delay1us(DELAY_TIME);
    SDA = 1;
    I2C_Delay1us(DELAY_TIME);
}

void I2C_SendACK(u8 i)            //Send ACK (LOW)
{
    if(1==i)
	  SDA=1;	             //准备好SDA电平状态，不应答
    else 
	  SDA=0;  						//准备好SDA电平状态，应答 	
	  SCL=1;
    I2C_Delay1us(DELAY_TIME);
    SCL=0; 
    I2C_Delay1us(DELAY_TIME);
}

        
u8 I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	u8 i=0;
	SDA=1;	        //释放SDA
	SCL=1;         //SCL拉高进行采样
	while(SDA)//等待SDA拉低
	{
		i++;      //等待计数
		if(i==2)//超时跳出循环
		break;
	}
	if(SDA)//再次判断SDA是否拉低
	{
		SCL=0; 
		return 0;//从机应答失败，返回0
	}
    I2C_Delay1us(DELAY_TIME);
    SCL = 0;
    I2C_Delay1us(DELAY_TIME);
	return 1;//从机应答成功

}

//向IIC总线发送一个字节数据
/*
一个字节8bit,当SCL低电平时，准备好SDA，SCL高电平时，从机采样SDA
*/
//**************************************
void I2C_SendByte(u8 dat)
{
  u8 i;
	SCL=0;//SCL拉低，给SDA准备
  for (i=0; i<8; i++)         //8位计数器
  {
	if(dat&0x80)//SDA准备
	 SDA=1;  
	else 
	 SDA=0;
    SCL=1;                //拉高时钟，给从机采样
    I2C_Delay1us(DELAY_TIME);       //延时保持IIC时钟频率，也是给从机采样有充足时间
    SCL=0;                //拉低时钟，给SDA准备
	I2C_Delay1us(DELAY_TIME); 		  //延时保持IIC时钟频率
	dat <<= 1;          //移出数据的最高位  
  }					 
}

//**************************************
//从IIC总线接收一个字节数据
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    SDA=1;//释放SDA，给从机使用
    I2C_Delay1us(10);          //延时给从机准备SDA时间            
    for (i=0; i<8; i++)         //8位计数器
    { 
		dat <<= 1;
			
        SCL=1;                //拉高时钟线，采样从机SDA
     
		  if(SDA) //读数据    
		   dat |=0x01;      
       I2C_Delay1us(DELAY_TIME);     //延时保持IIC时钟频率		
       SCL=0;           //拉低时钟线，处理接收到的数据
       I2C_Delay1us(DELAY_TIME);   //延时给从机准备SDA时间
    } 
    return dat;
}

////向IIC设备写入一个字节数据
////**************************************
//u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 datas)
//{
//	  if(I2C_Start()==0)  //起始信号
//		{I2C_Stop(); return 0;}           

//    I2C_SendByte(Slave_Address);   //发送设备地址+写信号
// 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
//   
//		I2C_SendByte(REG_Address);    //内部寄存器地址，
// 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
//   
//		I2C_SendByte(datas);       //内部寄存器数据，
//	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
//		
//		I2C_Stop();   //发送停止信号
//		
//		return 1;
//}


////**************************************
////从IIC设备读取一个字节数据
////**************************************
//u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
//{
// if(I2C_Start()==0)  //起始信号
//	{I2C_Stop(); return 0;}          
//	 
//	I2C_SendByte(Slave_Address);    //发送设备地址+写信号
// 	if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
//	
//	I2C_SendByte(REG_Address);     //发送存储单元地址
// 	if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
//	
//	if(I2C_Start()==0)  //起始信号
//			{I2C_Stop(); return 0;}            

//			
//	I2C_SendByte(Slave_Address+1);  //发送设备地址+读信号
// 	if(!I2C_WaitAck()){I2C_Stop(); return 0;}
//	
//	while(length-1)
//	{
//		*REG_data++=I2C_RecvByte();       //读出寄存器数据
//		I2C_SendACK(0);               //应答
//		length--;
//	}
//	*REG_data=I2C_RecvByte();  
//	I2C_SendACK(1);     //发送停止传输信号
//	I2C_Stop();                    //停止信号
//	return 1;
//}


u8 takeRangeReading(u8 Slave_Address)
{
	I2C_Delay1us(40); 
  if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return 0;}           

    I2C_SendByte(Slave_Address);   //发送设备地址+写信号
 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_Delay1us(40);  
		I2C_SendByte(0x51);    //内部寄存器地址，
 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
   
		
		I2C_Stop();   	
		return 1;
}

u8 requestRange(u8 Slave_Address,u8 *distance)
{
	
	u8 REG_data[2]={0,0};
 
	if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return 0;}          
	 
	I2C_SendByte(Slave_Address);    //发送设备地址+写信号
 	if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_Delay1us(DELAY_TIME);  


	REG_data[0]=I2C_RecvByte();       //读出寄存器数据

	I2C_SendACK(0);               //应答

	REG_data[1]=I2C_RecvByte();       //读出寄存器数据
	
	I2C_SendACK(1);     //发送停止传输信号
	
	I2C_Stop();                    //停止信号
	//distance[0]=REG_data[0]<<8|REG_data[1];
	distance[0]=REG_data[0];
	distance[1]=REG_data[1];
	return 1;
}

u8 GY_WriteStartMeasure(void)
{

	return takeRangeReading(0xE0);

}

u8 GY_GetMeasureResultTemp(u8 *distance)
{

	 return requestRange(0xE1, distance) ;

}

int GY_GetMeasureResult(u8 *distance)
{

	 if(!GY_GetMeasureResultTemp(distance)) {return -1;}
	 return distance[0] << 8 | distance[1];

}

/*******************************************************************************
* 函数名         : void At24c02Write(unsigned char addr,unsigned char dat)
* 函数功能		   : 往24c02的一个地址写入一个数据
* 输入           : 无
* 输出         	 : 1 通信正常 ； 0 通信失败
*******************************************************************************/

u8 At24c02_Write_u8(u8 addr,u8 dat)
{
	if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return 0;}  
	I2C_SendByte(0xa0);//发送写器件地址
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(addr);//发送要写入内存地址
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(dat);	//发送数据
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_Stop(); 
		
	return 1;
}

u8 At24c02_Write_int(u8 addr,int dat)
{
	 u8 temp[INT_BYTE_NUM] = {0};
	 temp[0] = dat >> 8;
	 temp[1] = dat;
	 return At24c02_PageWrite(addr, temp, INT_BYTE_NUM);
		
}

u8 At24c02_Write_float(u8 addr,float dat)
{
	 storFloatData temp;
	 temp.value = dat;
	 return At24c02_PageWrite(addr, temp.byte, FLOAT_BYTE_NUM);
		
}


u8 At24c02_Read_u8(u8 addr)
{
	u8 Data;
	I2C_Start();
	I2C_SendByte(0xa0); //发送写器件地址
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(addr); //发送要读取的地址
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_Start();
	I2C_SendByte(0xa1); //发送读器件地址
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	
	Data = I2C_RecvByte(); //读取数据
	I2C_SendACK(1);     //发送停止传输信号
	I2C_Stop(); 
	
	return Data;	
}


int At24c02_Read_int(u8 addr)
{
	u8 temp[INT_BYTE_NUM] = {0};
	At24c02_Sequential_Read(addr, temp, INT_BYTE_NUM);
	return (int)(temp[0] << 8 | temp[1]);
}

float At24c02_Read_float(u8 addr)
{
	storFloatData read_data;
	At24c02_Sequential_Read(addr, read_data.byte, FLOAT_BYTE_NUM);
	return read_data.value;
}




//如果长度过长，会触发延时
u8 At24c02_PageWrite(u8 addr, u8 *dat, u8 len)					//连续写入
{
	I2C_Start();
	I2C_SendByte(0xa0); //发送写器件地址
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(addr);
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
		
	while(len--){ 		//根据数组长度计算循环次数，遇到地址的8的整数倍,进行翻页
		I2C_SendByte(*dat);
			if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
		dat  ++;
		addr ++;
	
		if(addr % (unsigned char)8 ==0){	//遇到写入地址为8的整数倍，翻页
			I2C_Stop();
			delay_ms(7);
			
			I2C_Start();
			I2C_SendByte(0xa0); //发送写器件地址
				if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
			I2C_SendByte(addr);	
				if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
		}
			
	}

	I2C_Stop();	
	return 1;
}

u8 At24c02_Sequential_Read(u8 addr, u8 *dat, u8 len)				//连续读取
{
	I2C_Start();
	I2C_SendByte(0xa0); //发送写器件地址
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(addr);
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_Start();
	I2C_SendByte(0xa1); //发送写器件地址
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	while(len--)
	{
		*dat = I2C_RecvByte();										//读取数组每一个元素
		if(len > 0)
		{
			I2C_SendACK(0);
		}
		
		dat++;	
	}
	
	I2C_SendACK(1);
	I2C_Stop();	
	return 1;
}

//void WriteStartMeasure()
//{
//		I2C_Start();
//		I2C_WriteAbyte(0xE0);
//		I2C_Check_ACK();
//		I2C_WriteAbyte(0x51);
//		I2C_Check_ACK();
//		I2C_Stop();
//}

//u16  GetMeasureResult()
//{
//		u8 HData, LData;
//		I2C_Start();
//		I2C_WriteAbyte(0xE1);
//		I2C_Check_ACK();
//		HData = I2C_ReadAbyte();
//		S_ACK();
//		LData = I2C_ReadAbyte();
//		S_NoACK();
//		I2C_Stop();
//		
//		return (HData << 8) | LData;
//}




////
//static void I2C_Delay(unsigned char n)
//{
//    do
//    {
//        _nop_();_nop_();_nop_();_nop_();_nop_();
//        _nop_();_nop_();_nop_();_nop_();_nop_();
//        _nop_();_nop_();_nop_();_nop_();_nop_();		
//    }
//    while(n--);      	
//}

////
//void I2CStart(void)
//{
//    sda = 1;
//    scl = 1;
//	I2C_Delay(DELAY_TIME);
//    sda = 0;
//	I2C_Delay(DELAY_TIME);
//    scl = 0;    
//}

////
//void I2CStop(void)
//{
//    sda = 0;
//    scl = 1;
//	I2C_Delay(DELAY_TIME);
//    sda = 1;
//	I2C_Delay(DELAY_TIME);
//}

////
//void I2CSendByte(unsigned char byt)
//{
//    unsigned char i;
//	
//    for(i=0; i<8; i++){
//        scl = 0;
//		I2C_Delay(DELAY_TIME);
//        if(byt & 0x80){
//            sda = 1;
//        }
//        else{
//            sda = 0;
//        }
//		I2C_Delay(DELAY_TIME);
//        scl = 1;
//        byt <<= 1;
//		I2C_Delay(DELAY_TIME);
//    }
//	
//    scl = 0;  
//}

////
//unsigned char I2CReceiveByte(void)
//{
//	unsigned char da;
//	unsigned char i;
//	for(i=0;i<8;i++){   
//		scl = 1;
//		I2C_Delay(DELAY_TIME);
//		da <<= 1;
//		if(sda) 
//			da |= 0x01;
//		scl = 0;
//		I2C_Delay(DELAY_TIME);
//	}
//	return da;    
//}

////
//unsigned char I2CWaitAck(void)
//{
//	unsigned char ackbit;
//	
//    scl = 1;
//	I2C_Delay(DELAY_TIME);
//    ackbit = sda; 
//    scl = 0;
//	I2C_Delay(DELAY_TIME);
//	
//	return ackbit;
//}


//void I2CSendAck(unsigned char ackbit)
//{
//    scl = 0;
//    sda = ackbit; 
//	I2C_Delay(DELAY_TIME);
//    scl = 1;
//	I2C_Delay(DELAY_TIME);
//    scl = 0; 
//	sda = 1;
//	I2C_Delay(DELAY_TIME);
//}



//void MeasureStart(void)
//{
//	I2CStart();//IIC起始信号
//	I2CSendByte(0xE0);//写入地址和读写方向
//	I2CWaitAck();//等待应答信号
//	I2CSendByte(0x51);//写入地址和读写方向
//	I2CWaitAck();//等待应答信号
//	I2CStop();//停止信号
//}

//u16 GetMeasure(void)
//{
//	u8 Hdata, Ldata;
//	I2CStart();//IIC起始信号
//	I2CSendByte(0xE1);//写入地址和读写方向
//	I2CWaitAck();//等待应答信号
//	
//	Hdata=I2CReceiveByte();//读出高8位数据
//	I2CSendAck(1);//送入应答信号
//	Ldata=I2CReceiveByte();//读出高8位数据
//	I2CSendAck(0);//送入应答信号

//	I2CStop();//停止信号
//}

void saveConfig(void){
	uint8 i, j;
	iictmp = 0x04;
	At24c02_Write_int(0x00, ver);
	for(i = 0; i < 5; i++){
		for(j = 0; j < 8; j++){
			if(guis[i][j].type == GUI_TYPE_EDIT_FLOAT_VALUE){
				At24c02_Write_float((u8)iictmp, *(guis[i][j].floatval));
				iictmp += 4;
				delay_ms(2);
			}else
			if(guis[i][j].type == GUI_TYPE_EDIT_INT32_VALUE){
				At24c02_Write_int((u8)iictmp, *(guis[i][j].intval));
				iictmp += 4;
				delay_ms(2);
			}
		}
	}
}
void loadConfig(void){
	uint8 i, j;
	iictmp = 0x04;
	for(i = 0; i < 5; i++){
		for(j = 0; j < 8; j++){
			if(guis[i][j].type == GUI_TYPE_EDIT_FLOAT_VALUE){
				*(guis[i][j].floatval) = At24c02_Read_float((u8)iictmp);
				iictmp += 4;
				delay_ms(2);
			}else
			if(guis[i][j].type == GUI_TYPE_EDIT_INT32_VALUE){
				*(guis[i][j].intval) = At24c02_Read_int((u8)iictmp);
				iictmp += 4;
				delay_ms(2);
			}
		}
	}
}
void slinit(void){
	iictmp =  At24c02_Read_int(0x00);
	if(iictmp != ver){
		iictmp = -1;
	}else{
		loadConfig();
		iictmp = 0;
	}
	return;
}
