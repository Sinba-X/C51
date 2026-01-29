#include <reg52.h>
sbit LED = P0^0;
void  main()
{
    extern unsigned int i;
    while(1)
    {
         LED = 0;                 //点亮小灯
         for(i=0;i<30000;i++);  //延时
         LED = 1;                 //熄灭小灯
         for(i=0;i<30000;i++); //延时
    }
}
unsigned  int  i = 0;
... ...