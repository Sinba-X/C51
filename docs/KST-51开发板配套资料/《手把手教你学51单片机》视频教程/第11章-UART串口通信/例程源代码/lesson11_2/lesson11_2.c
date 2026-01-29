#include<reg52.h>
void ConfigUART(unsigned int baud);

void main()
{
	ConfigUART(9600);

	while(1)
	{
		while(!RI);
		RI = 0;
		SBUF = SBUF + 1;
		while(!TI);
		TI = 0;		
	}
}

void ConfigUART(unsigned int baud)
{
	SCON = 0x50;
	TMOD &= 0X0F;
	TMOD |= 0x20;
	TH1 = 256 - (11059200/12/32)/baud;
	TL1 = TH1;
	ET1 = 0;
	TR1 = 1;
}