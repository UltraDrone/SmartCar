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
	if(!SDA)  { return 0;}//SDA��Ϊ�͵�ƽ������æ,�˳�
    SDA = 0;
    I2C_Delay1us(DELAY_TIME);
	if(SDA) return 0;//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
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
	  SDA=1;	             //׼����SDA��ƽ״̬����Ӧ��
    else 
	  SDA=0;  						//׼����SDA��ƽ״̬��Ӧ�� 	
	  SCL=1;
    I2C_Delay1us(DELAY_TIME);
    SCL=0; 
    I2C_Delay1us(DELAY_TIME);
}

        
u8 I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	u8 i=0;
	SDA=1;	        //�ͷ�SDA
	SCL=1;         //SCL���߽��в���
	while(SDA)//�ȴ�SDA����
	{
		i++;      //�ȴ�����
		if(i==2)//��ʱ����ѭ��
		break;
	}
	if(SDA)//�ٴ��ж�SDA�Ƿ�����
	{
		SCL=0; 
		return 0;//�ӻ�Ӧ��ʧ�ܣ�����0
	}
    I2C_Delay1us(DELAY_TIME);
    SCL = 0;
    I2C_Delay1us(DELAY_TIME);
	return 1;//�ӻ�Ӧ��ɹ�

}

//��IIC���߷���һ���ֽ�����
/*
һ���ֽ�8bit,��SCL�͵�ƽʱ��׼����SDA��SCL�ߵ�ƽʱ���ӻ�����SDA
*/
//**************************************
void I2C_SendByte(u8 dat)
{
  u8 i;
	SCL=0;//SCL���ͣ���SDA׼��
  for (i=0; i<8; i++)         //8λ������
  {
	if(dat&0x80)//SDA׼��
	 SDA=1;  
	else 
	 SDA=0;
    SCL=1;                //����ʱ�ӣ����ӻ�����
    I2C_Delay1us(DELAY_TIME);       //��ʱ����IICʱ��Ƶ�ʣ�Ҳ�Ǹ��ӻ������г���ʱ��
    SCL=0;                //����ʱ�ӣ���SDA׼��
	I2C_Delay1us(DELAY_TIME); 		  //��ʱ����IICʱ��Ƶ��
	dat <<= 1;          //�Ƴ����ݵ����λ  
  }					 
}

//**************************************
//��IIC���߽���һ���ֽ�����
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    SDA=1;//�ͷ�SDA�����ӻ�ʹ��
    I2C_Delay1us(10);          //��ʱ���ӻ�׼��SDAʱ��            
    for (i=0; i<8; i++)         //8λ������
    { 
		dat <<= 1;
			
        SCL=1;                //����ʱ���ߣ������ӻ�SDA
     
		  if(SDA) //������    
		   dat |=0x01;      
       I2C_Delay1us(DELAY_TIME);     //��ʱ����IICʱ��Ƶ��		
       SCL=0;           //����ʱ���ߣ�������յ�������
       I2C_Delay1us(DELAY_TIME);   //��ʱ���ӻ�׼��SDAʱ��
    } 
    return dat;
}

////��IIC�豸д��һ���ֽ�����
////**************************************
//u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 datas)
//{
//	  if(I2C_Start()==0)  //��ʼ�ź�
//		{I2C_Stop(); return 0;}           

//    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
// 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
//   
//		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
// 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
//   
//		I2C_SendByte(datas);       //�ڲ��Ĵ������ݣ�
//	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
//		
//		I2C_Stop();   //����ֹͣ�ź�
//		
//		return 1;
//}


////**************************************
////��IIC�豸��ȡһ���ֽ�����
////**************************************
//u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
//{
// if(I2C_Start()==0)  //��ʼ�ź�
//	{I2C_Stop(); return 0;}          
//	 
//	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
// 	if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
//	
//	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ
// 	if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
//	
//	if(I2C_Start()==0)  //��ʼ�ź�
//			{I2C_Stop(); return 0;}            

//			
//	I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
// 	if(!I2C_WaitAck()){I2C_Stop(); return 0;}
//	
//	while(length-1)
//	{
//		*REG_data++=I2C_RecvByte();       //�����Ĵ�������
//		I2C_SendACK(0);               //Ӧ��
//		length--;
//	}
//	*REG_data=I2C_RecvByte();  
//	I2C_SendACK(1);     //����ֹͣ�����ź�
//	I2C_Stop();                    //ֹͣ�ź�
//	return 1;
//}


u8 takeRangeReading(u8 Slave_Address)
{
	I2C_Delay1us(40); 
  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return 0;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_Delay1us(40);  
		I2C_SendByte(0x51);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
   
		
		I2C_Stop();   	
		return 1;
}

u8 requestRange(u8 Slave_Address,u8 *distance)
{
	
	u8 REG_data[2]={0,0};
 
	if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return 0;}          
	 
	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_Delay1us(DELAY_TIME);  


	REG_data[0]=I2C_RecvByte();       //�����Ĵ�������

	I2C_SendACK(0);               //Ӧ��

	REG_data[1]=I2C_RecvByte();       //�����Ĵ�������
	
	I2C_SendACK(1);     //����ֹͣ�����ź�
	
	I2C_Stop();                    //ֹͣ�ź�
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

int GY_GetMeasureResult()
{
	 u8 tempdistance[2] = {0};
	 if(!GY_GetMeasureResultTemp(tempdistance)) {return -1;}
	 return (((int)tempdistance[0]) << 8) | tempdistance[1];

}

/*******************************************************************************
* ������         : void At24c02Write(unsigned char addr,unsigned char dat)
* ��������		   : ��24c02��һ����ַд��һ������
* ����           : ��
* ���         	 : 1 ͨ������ �� 0 ͨ��ʧ��
*******************************************************************************/

u8 At24c02_Write_u8(u8 addr,u8 dat)
{
	if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return 0;}  
	I2C_SendByte(0xa0);//����д������ַ
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(addr);//����Ҫд���ڴ��ַ
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(dat);	//��������
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
	I2C_SendByte(0xa0); //����д������ַ
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(addr); //����Ҫ��ȡ�ĵ�ַ
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_Start();
	I2C_SendByte(0xa1); //���Ͷ�������ַ
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	
	Data = I2C_RecvByte(); //��ȡ����
	I2C_SendACK(1);     //����ֹͣ�����ź�
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




//������ȹ������ᴥ����ʱ
u8 At24c02_PageWrite(u8 addr, u8 *dat, u8 len)					//����д��
{
	I2C_Start();
	I2C_SendByte(0xa0); //����д������ַ
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(addr);
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
		
	while(len--){ 		//�������鳤�ȼ���ѭ��������������ַ��8��������,���з�ҳ
		I2C_SendByte(*dat);
			if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
		dat  ++;
		addr ++;
	
		if(addr % (unsigned char)8 ==0){	//����д���ַΪ8������������ҳ
			I2C_Stop();
			delay_ms(7);
			
			I2C_Start();
			I2C_SendByte(0xa0); //����д������ַ
				if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
			I2C_SendByte(addr);	
				if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
		}
			
	}

	I2C_Stop();	
	return 1;
}

u8 At24c02_Sequential_Read(u8 addr, u8 *dat, u8 len)				//������ȡ
{
	I2C_Start();
	I2C_SendByte(0xa0); //����д������ַ
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_SendByte(addr);
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	I2C_Start();
	I2C_SendByte(0xa1); //����д������ַ
		if(!I2C_WaitAck()){I2C_Stop(); return 0;} 
	while(len--)
	{
		*dat = I2C_RecvByte();										//��ȡ����ÿһ��Ԫ��
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
//	I2CStart();//IIC��ʼ�ź�
//	I2CSendByte(0xE0);//д���ַ�Ͷ�д����
//	I2CWaitAck();//�ȴ�Ӧ���ź�
//	I2CSendByte(0x51);//д���ַ�Ͷ�д����
//	I2CWaitAck();//�ȴ�Ӧ���ź�
//	I2CStop();//ֹͣ�ź�
//}

//u16 GetMeasure(void)
//{
//	u8 Hdata, Ldata;
//	I2CStart();//IIC��ʼ�ź�
//	I2CSendByte(0xE1);//д���ַ�Ͷ�д����
//	I2CWaitAck();//�ȴ�Ӧ���ź�
//	
//	Hdata=I2CReceiveByte();//������8λ����
//	I2CSendAck(1);//����Ӧ���ź�
//	Ldata=I2CReceiveByte();//������8λ����
//	I2CSendAck(0);//����Ӧ���ź�

//	I2CStop();//ֹͣ�ź�
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
