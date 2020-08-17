
#include <stdio.h>


void best ( double target )
{
    unsigned int ra;
    double da;
    double bdiff;
    double ldiff;
    double diff;
    unsigned int bdiv;
    //double target;
    unsigned int mul;
    double div;
    double clock;

    clock=12000000.0;
    div=256.0;
    bdiff=clock;
    bdiv=(unsigned int)(clock/((16.0)*target));
    //bdiv=1;
    for(;bdiv<65536;bdiv++)
    {
        ldiff=clock;
        for(mul=1;mul<256;mul++)
        {
            da=clock/ (16.0 * bdiv * (1.0+(mul/div)));
            if(da>target) diff=da-target; else diff=target-da;
            //if(da>=target)
            {
                //diff=da-target;
                if(diff<ldiff)
                {
                    ldiff=diff;
                    if(diff<bdiff)
                    {
                        bdiff=diff;
                        printf("reg %u mul %u %lf %lf\n",bdiv,mul,da,bdiff);
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
}



int main ( void )
{
    best(38400);
    best(115200);
    return(0);
}
