#include <reg52.h>

sbit  ADDR3 = P1^3;
sbit  ENLED = P1^4;

unsigned char code LedChar[] = {  //数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};
unsigned char LedBuff[7] = {  //数码管+独立LED显示缓冲区
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
bit flag1s = 1;
unsigned char T0RH = 0;
unsigned char T0RL = 0;

void ConfigTimer0(unsigned int ms);
void TrafficLight();
void main()
{
    EA = 1;      //开总中断
    ENLED = 0;   //使能数码管和LED
    ADDR3 = 1;
	ConfigTimer0(1);  //配置T0定时1ms
    
    while (1)
    {
        if (flag1s)  //每秒执行一次交通灯刷新
        {
            flag1s = 0;
            TrafficLight();
        }
    }
}

void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //临时变量
    
    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 18;           //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8);  //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}
void TrafficLight()
{
	static unsigned char color = 2;
	static unsigned char timer = 0;

	if(timer == 0)
	{
		switch(color)
		{
			case 0:
			color = 1;
			timer = 2;
			LedBuff[6] = 0xE7;
			break;

			case 1:
			color = 2;
			timer = 29;
			LedBuff[6] = 0xFC;
			break;

			case 2:
			color = 0;
			timer = 39;
			LedBuff[6] = 0x3F;
			break;

			default:
			break;
		}
	}
	else
	{
		timer--;
	}
	LedBuff[0] = LedChar[timer%10];
	LedBuff[1] = LedChar[timer/10];	
}
void LedScan()
{
    static unsigned char i = 0;  //动态扫描索引
    
    P0 = 0xFF;             //关闭所有段选位，显示消隐
    P1 = (P1 & 0xF8) | i;  //位选索引值赋值到P1口低3位
    P0 = LedBuff[i];       //缓冲区中索引位置的数据送到P0口
    if (i < 6)             //索引递增循环，遍历整个缓冲区
        i++;
    else
        i = 0;
}

void InterruptTimer0() interrupt 1
{
    static unsigned int tmr1s = 0;  //1秒定时器

    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    LedScan();   //LED扫描显示
    tmr1s++;     //1秒定时的处理
    if (tmr1s >= 1000)
    {
        tmr1s = 0;
        flag1s = 1;  //设置秒定时标志
    }
}