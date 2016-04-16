
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define USART0BASE 0x40064000

#define GPIO_DIR0 0xA0002000
#define GPIO_SET0 0xA0002200
#define GPIO_CLR0 0xA0002280

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(GPIO_DIR0);
    ra|=1<<23;
    PUT32(GPIO_DIR0,ra);


    while(1)
    {
        PUT32(GPIO_SET0,1<<23);
        for(ra=0;ra<100000;ra++) dummy(ra);
        PUT32(GPIO_CLR0,1<<23);
        for(ra=0;ra<100000;ra++) dummy(ra);
    }
    return(0);
}
