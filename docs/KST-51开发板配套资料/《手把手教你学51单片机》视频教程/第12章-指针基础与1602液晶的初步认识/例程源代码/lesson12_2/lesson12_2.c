#include <reg52.h>

bit cmdArrived = 0;   //命令到达标志，即接收到上位机下发的命令
unsigned char cmdIndex = 0;
unsigned char *ptrTxd;
unsigned char cntTxd = 0;

unsigned char array1[1] = {1};
unsigned char array2[2] = {1,2};
unsigned char array3[4] = {1,2,3,4};
unsigned char array4[8] = {1,2,3,4,5,6,7,8};
void ConfigUART(unsigned int baud);
void main()
{
    EA = 1;  //开总中断
    ConfigUART(9600);  //配置波特率为9600
    
    while (1)
    {
		if(cmdArrived)
		{
			cmdArrived = 0;
			switch(cmdIndex)
			{
				case 1:
					ptrTxd = array1;
					cntTxd = sizeof(array1);
					TI = 1;
					break;
				case 2:
					ptrTxd = array2;
					cntTxd = sizeof(array2);
					TI = 1;
					break;
				case 3:
					ptrTxd = array3;
					cntTxd = sizeof(array3);
					TI = 1;
					break;
				case 4:
					ptrTxd = array4;
					cntTxd = sizeof(array4);
					TI = 1;
					break;
				default:
					break;	
			}	
		}
        
    }
}

void ConfigUART(unsigned int baud)
{
    SCON  = 0x50;  //配置串口为模式1
    TMOD &= 0x0F;  //清零T1的控制位
    TMOD |= 0x20;  //配置T1为模式2
    TH1 = 256 - (11059200/12/32)/baud;  //计算T1重载值
    TL1 = TH1;     //初值等于重载值
    ET1 = 0;       //禁止T1中断
    ES  = 1;       //使能串口中断
    TR1 = 1;       //启动T1
}

void InterruptUART() interrupt 4
{
    if (RI)  //接收到字节
    {
        RI = 0;  //清零接收中断标志位
		cmdIndex = SBUF;
		cmdArrived = 1;       
    }
    if (TI)  //字节发送完毕
    {
        TI = 0;  //清零发送中断标志位
		if(cntTxd > 0)
		{
			SBUF = *ptrTxd;
			cntTxd--;
			ptrTxd++;
		}
        
    }
}