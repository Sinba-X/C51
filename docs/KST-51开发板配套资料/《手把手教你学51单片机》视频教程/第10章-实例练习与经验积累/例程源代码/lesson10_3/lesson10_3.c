#include <reg52.h>

sbit PWMOUT = P0^0;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char HighRH = 0;  //高电平重载值的高字节
unsigned char HighRL = 0;  //高电平重载值的低字节
unsigned char LowRH  = 0;  //低电平重载值的高字节
unsigned char LowRL  = 0;  //低电平重载值的低字节
unsigned char T1RH = 0;
unsigned char T1RL = 0;
unsigned long PeriodCnt = 0;

void ConfigTimer1(unsigned int ms);
void ConfigPWM(unsigned int fr, unsigned char dc);

void main()
{
    EA = 1;     //开总中断
    ENLED = 0;  //使能独立LED
    ADDR3 = 1;
    ADDR2 = 1;
    ADDR1 = 1;
    ADDR0 = 0;
    
    ConfigPWM(100, 10);  //配置并启动PWM
    ConfigTimer1(50);    //用T1定时调整占空比
    while (1);
}
void ConfigTimer1(unsigned int ms)
{
    unsigned long tmp;  //临时变量
    
    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 12;           //补偿中断响应延时造成的误差
    T1RH = (unsigned char)(tmp>>8);  //定时器重载值拆分为高低字节
    T1RL = (unsigned char)tmp;
    TMOD &= 0x0F;   //清零T1的控制位
    TMOD |= 0x10;   //配置T1为模式1
    TH1 = T1RH;     //加载T1重载值
    TL1 = T1RL;
    ET1 = 1;        //使能T1中断
    TR1 = 1;        //启动T1
}
void ConfigPWM(unsigned int fr, unsigned char dc)
{
    unsigned int high, low;
    
    PeriodCnt = (11059200/12) / fr; //计算一个周期所需的计数值
    high = (PeriodCnt*dc) / 100;    //计算高电平所需的计数值
    low  = PeriodCnt - high;        //计算低电平所需的计数值
    high = 65536 - high + 12;       //计算高电平的定时器重载值并补偿中断延时
    low  = 65536 - low  + 12;       //计算低电平的定时器重载值并补偿中断延时
    HighRH = (unsigned char)(high>>8); //高电平重载值拆分为高低字节
    HighRL = (unsigned char)high;
    LowRH  = (unsigned char)(low>>8);  //低电平重载值拆分为高低字节
    LowRL  = (unsigned char)low;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = HighRH;   //加载T0重载值
    TL0 = HighRL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
    PWMOUT = 1;     //输出高电平
}
void AdjustDutyCycle(unsigned char dc)
{
	unsigned int high, low;

	high = (PeriodCnt*dc)/100;
	low = PeriodCnt - high;
	high = 65536 - high + 12;
	low = 65536 - low + 12;
	HighRH = (unsigned char)(high >> 8);
	HighRL = (unsigned char)high;
	LowRH = (unsigned char)(low >> 8);
	LowRL = (unsigned char)low;
}
void InterruptTimer0() interrupt 1
{
    if (PWMOUT == 1)  //当前输出为高电平时，装载低电平值并输出低电平
    {
        TH0 = LowRH;
        TL0 = LowRL;
        PWMOUT = 0;
    }
    else              //当前输出为低电平时，装载高电平值并输出高电平
    {
        TH0 = HighRH;
        TL0 = HighRL;
        PWMOUT = 1;
    }
}

void InterruptTimer1() interrupt 3
{
	unsigned char code table[13] = {
	5, 18, 30, 41, 51, 60, 68, 75, 81, 86, 90, 93, 95
	};
	static bit dir = 0;
	static unsigned char index = 0;

	TH1 = T1RH;
	TL1 = T1RL;

	AdjustDutyCycle(table[index]);
	if(dir == 0)
	{
		index++;
		if(index >= 12)
		{
			dir = 1;
		}
	}
	else
	{
		index--;
		if(index == 0)
		{
			dir = 0;
		}
	}
}