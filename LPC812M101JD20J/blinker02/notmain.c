
extern unsigned int GET32 ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern void dummy ( unsigned int );

#define GPIODIRP0  0xA0002000
#define GPIOSETP0  0xA0002200
#define GPIOCLRP0  0xA0002280
#define GPIONOTP0  0xA0002300

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(GPIODIRP0);
    ra|=1<<14;
    ra|=1<<15;
    ra|=1<<16;
    ra|=1<<17;
    PUT32(GPIODIRP0,ra);
    PUT32(GPIOCLRP0,(1<<14)|(1<<15)|(1<<16)|(1<<17));
}
static void led_on ( void )
{
    PUT32(GPIOSETP0,(1<<14)|(1<<15)|(1<<16)|(1<<17));
}
static void led_off ( void )
{
    PUT32(GPIOCLRP0,(1<<14)|(1<<15)|(1<<16)|(1<<17));
}

static void do_delay ( unsigned int sec )
{
    unsigned int ra,rb,rc,rd;

    rb=GET32(STK_CVR);
    for(rd=0;rd<sec;)
    {
        ra=GET32(STK_CVR);
        rc=(rb-ra)&0x00FFFFFF;
        if(rc>=12000000)
        {
            rb=ra;
            rd++;
        }
    }
}

int notmain ( void )
{
    unsigned int rx;

    led_init();

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);

    for(rx=0;rx<5;rx++)
    {
        led_on();
        while(1) if(GET32(STK_CVR)&0x200000) break;
        led_off();
        while(1) if((GET32(STK_CVR)&0x200000)==0) break;
    }

    while(1)
    {
        led_on();
        do_delay(10);
        led_off();
        do_delay(1);
    }
    return(0);
}
