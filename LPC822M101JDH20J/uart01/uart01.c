
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define USART0BASE 0x40064000

int notmain ( void )
{
    unsigned int rx;
    for(rx=0;;rx++)
    {
        while(1) if(GET32(USART0BASE+0x08)&(1<<2)) break;
        PUT32(USART0BASE+0x1C,0x30+(rx&7));
    }
    return(0);
}
