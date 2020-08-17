
extern unsigned int GET32 ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern void dummy ( unsigned int );

#define PRESETCTRL 0x40048004
#define USART0BASE 0x40064000
#define SYSAHBCLKCTRL 0x40048080
#define UARTCLOCKDIV 0x40048094
#define UARTFRGDIV   0x400480F0
#define UARTFRGMULT  0x400480F4
#define PINASSIGN0 0x4000C000

void uart_init ( void )
{
    unsigned int ra;

    ra=GET32(SYSAHBCLKCTRL);
    ra|=1<<14; //uart0
    PUT32(SYSAHBCLKCTRL,ra);

    //ra=GET32(PRESETCTRL);
    //ra&=(~(1<<3)); //uart0
    //PUT32(PRESETCTRL,ra);

    //ra=GET32(PRESETCTRL);
    //ra|=1<<3; //uart0
    //PUT32(PRESETCTRL,ra);

    ra=GET32(PINASSIGN0);
    ra&=(~(0xFF<<0));
    ra|=      4<<0;  //U0_TXD_O PIN4
    ra&=(~(0xFF<<8));
    ra|=      0<<8;  //U0_RXD_I PIN0
    PUT32(PINASSIGN0,ra);

    PUT32(UARTCLOCKDIV,0x00000001);
    if(1)
    {
        //38400 baud
        //12000000/16*((1+(7/256))*(19) )
        //reg 19 mul 7 38423.053832 23.053832
        PUT32(UARTFRGDIV,255);
        PUT32(UARTFRGMULT,7);
        PUT32(USART0BASE+0x20,19);
    }
    if(0) //why doesnt this work?!
    {
        //38400 baud
        //12000000/16*((1+(7/256))*(19) )
        //reg 10 mul 244 38400.000000 0.000000
        PUT32(UARTFRGDIV,255);
        PUT32(UARTFRGMULT,244);
        PUT32(USART0BASE+0x20,10);
    }
    if(0) //why doesnt this work?!
    {
        //115200 baud
        //12000000/16*((1+(22/256))*(6) )
        //reg 6 mul 22 115107.913669 92.086331
        PUT32(UARTFRGDIV,255);
        PUT32(UARTFRGMULT,23);
        PUT32(USART0BASE+0x20,6);
    }
    PUT32(USART0BASE+0x00,0x00000005);
}

static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART0BASE+0x08)&(1<<2)) break;
    PUT32(USART0BASE+0x1C,x);
}

static void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}

static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}


int notmain ( void )
{
    unsigned int ra;

    uart_init();
    hexstring(0x12345678);

    for(ra=0;ra<100;ra++)
    {
        uart_send(0x55);
    }

    for(ra=0x30;;ra++)
    {
        ra&=0x37;
        uart_send(ra);
    }



    return(0);
}
