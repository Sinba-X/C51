#include <reg52.h>

sbit PWMOUT = P0^0;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char HighRH = 0;
unsigned char HighRL = 0;
unsigned char LowRH = 0;
unsigned char LowRL = 0;
void ConfigPWM(unsigned int fr, unsigned char dc);
void ClosePWM();

void main()
{
    unsigned int i;
    
    EA = 1;     //开总中断
    ENLED = 0;  //使能独立LED
    ADDR3 = 1;
    ADDR2 = 1;
    ADDR1 = 1;
    ADDR0 = 0;
    
    while (1)
    {
        ConfigPWM(100, 10);    //频率100Hz，占空比10%
        for (i=0; i<40000; i++);
        ClosePWM();
        ConfigPWM(100, 40);    //频率100Hz，占空比40%
        for (i=0; i<40000; i++);
        ClosePWM();
        ConfigPWM(100, 90);    //频率100Hz，占空比90%
        for (i=0; i<40000; i++);
        ClosePWM();            //关闭PWM，相当于占空比100%
        for (i=0; i<40000; i++);
    }
}
void ConfigPWM(unsigned int fr, unsigned char dc)
{
	unsigned long tmp;
	unsigned int high, low;

	tmp = 11059200/12/fr;
	high = (tmp*dc)/100;
	low = tmp - high;
	high = 65536 - high + 12;
	low = 65536 - low + 12;
	HighRH = (unsigned char)(high >>8);
	HighRL = (unsigned char)high;
	LowRH = (unsigned char)(low >> 8);
	LowRL = (unsigned char)low;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = HighRH;
	TL0 = HighRL;
	ET0 = 1;
	TR0 = 1;
	PWMOUT = 1;
	
}
void ClosePWM()
{
	TR0 = 0;
	ET0 = 0;
	PWMOUT = 1;
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