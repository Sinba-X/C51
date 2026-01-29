#include <reg52.h>

sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit KEY1 = P2^4;
sbit KEY2 = P2^5;
sbit KEY3 = P2^6;
sbit KEY4 = P2^7;

unsigned char code LedChar[] = {  //数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};
unsigned char LedBuff[6] = {  //数码管显示缓冲区
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
unsigned char KeySta[4] = {  //按键当前状态
    1, 1, 1, 1
};
unsigned char T0RH = 0;
unsigned char T0RL = 0;
bit  StopwatchRefresh = 1;
bit  StopwatchRunning = 0;
unsigned char DecimalPart = 0;
unsigned int IntegerPart = 0;

void ConfigTimer0(unsigned int ms);
void StopwatchDisplay();
void KeyDriver();
void main()
{
	EA = 1;
	ENLED = 0;
	ADDR3 = 1;
	P2 = 0xFE;
	ConfigTimer0(2);
  	while(1)
	{
		if(StopwatchRefresh)
		{
			StopwatchRefresh = 0;
			StopwatchDisplay();	
		}
		KeyDriver();	
	}
}

void ConfigTimer0(unsigned int ms)
{
	unsigned long tmp;

	tmp = 11059200/12;
	tmp = (tmp * ms)/1000;
	tmp = 65536 - tmp;
	tmp = tmp + 18;

	T0RH = (unsigned char)(tmp >> 8);
	T0RL = (unsigned char)tmp;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 =1;
	TR0 = 1;
}
void StopwatchDisplay()
{
	signed char i;
	unsigned char buf[4];
	
	LedBuff[0] = LedChar[DecimalPart%10];
	LedBuff[1] = LedChar[DecimalPart/10];
	buf[0] = IntegerPart%10;
	buf[1] = IntegerPart/10%10;
	buf[2] = IntegerPart/100%10;
	buf[3] = IntegerPart/1000%10;

	for(i=3; i>=1; i--)
	{
		if(buf[i] == 0)
			LedBuff[i+2] = 0xFF;
		else
			break;
	}
	for(; i>=0; i--)
	{
		LedBuff[i+2] = LedChar[buf[i]];
	}
	LedBuff[2] &= 0x7F;
		
}
void StopwatchAction()
{
	if(StopwatchRunning)
		StopwatchRunning = 0;
	else
		StopwatchRunning = 1;
}
void StopwatchReset()
{
	StopwatchRunning = 0;
	DecimalPart = 0;
	IntegerPart = 0;
	StopwatchRefresh = 1;	
}
void KeyDriver()
{
    unsigned char i;
    static unsigned char backup[4] = {1,1,1,1};

    for (i=0; i<4; i++)  //循环检测4个按键
    {
        if (backup[i] != KeySta[i])  //检测按键动作
        {
            if (backup[i] != 0)      //按键按下时执行动作
            {
                if (i == 1)          //Esc键复位秒表
                    StopwatchReset();
                else if (i == 2)     //回车键启停秒表
                    StopwatchAction();
            }
            backup[i] = KeySta[i];   //刷新前一次的备份值
        }
    }
}

void KeyScan()
{
    unsigned char i;
    static unsigned char keybuf[4] = {  //按键扫描缓冲区
        0xFF, 0xFF, 0xFF, 0xFF
    };
    
    //按键值移入缓冲区
    keybuf[0] = (keybuf[0] << 1) | KEY1;
    keybuf[1] = (keybuf[1] << 1) | KEY2;
    keybuf[2] = (keybuf[2] << 1) | KEY3;
    keybuf[3] = (keybuf[3] << 1) | KEY4;
    //消抖后更新按键状态
    for (i=0; i<4; i++)
    {
        if (keybuf[i] == 0x00)
        {   //连续8次扫描值为0，即16ms内都是按下状态时，可认为按键已稳定的按下
            KeySta[i] = 0;
        }
        else if (keybuf[i] == 0xFF)
        {   //连续8次扫描值为1，即16ms内都是弹起状态时，可认为按键已稳定的弹起
            KeySta[i] = 1;
        }
    }
}

void LedScan()
{
    static unsigned char i = 0;  //动态扫描索引
    
    P0 = 0xFF;             //关闭所有段选位，显示消隐
    P1 = (P1 & 0xF8) | i;  //位选索引值赋值到P1口低3位
    P0 = LedBuff[i];       //缓冲区中索引位置的数据送到P0口
    if (i < 5)             //索引递增循环，遍历整个缓冲区
        i++;
    else
        i = 0;
}
void StopwatchCount()
{
	if(StopwatchRunning)
	{
		DecimalPart++;
		if(DecimalPart >= 100)
		{
			DecimalPart = 0;
			IntegerPart++;
			if(IntegerPart >= 10000)
			{
				IntegerPart = 0;
			}
		}
		StopwatchRefresh = 1;
	}
}

/* T0中断服务函数，完成数码管、按键扫描与秒表计数 */
void InterruptTimer0() interrupt 1
{
	static unsigned char tmr10ms = 0;

    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    LedScan();   //数码管扫描显示
    KeyScan();   //按键扫描

	tmr10ms++;
	if(tmr10ms >= 5)
	{
		tmr10ms = 0;
		StopwatchCount();
	}
  
}