#include <reg52.h>

extern void InitLcd1602();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
extern void I2CStart();
extern void I2CStop();
extern unsigned char I2CReadNAK();
extern bit I2CWrite(unsigned char dat);
unsigned char E2ReadByte(unsigned char addr);
void E2WriteByte(unsigned char addr, unsigned char dat);

void main()
{
    unsigned char dat;
    unsigned char str[10];

    InitLcd1602();   //初始化液晶
    dat = E2ReadByte(0x02);    //读取指定地址上的一个字节
    str[0] = (dat/100) + '0';  //转换为十进制字符串格式
    str[1] = (dat/10%10) + '0';
    str[2] = (dat%10) + '0';
    str[3] = '\0';
    LcdShowStr(0, 0, str);     //显示在液晶上
    dat++;                     //将其数值+1
    E2WriteByte(0x02, dat);    //再写回到对应的地址上
    
    while (1);
}
unsigned char E2ReadByte(unsigned char addr)
{
	unsigned char dat;

	I2CStart();
	I2CWrite(0x50<<1);
	I2CWrite(addr);
	I2CStart();	
	I2CWrite((0x50<<1) |0x01);
	dat = I2CReadNAK();
	I2CStop();

	return dat;
}
void E2WriteByte(unsigned char addr, unsigned char dat)
{
	I2CStart();
	I2CWrite(0x50<<1);
	I2CWrite(addr);
	I2CWrite(dat);
	I2CStop();
}