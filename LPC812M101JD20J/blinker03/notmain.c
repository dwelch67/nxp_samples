
extern unsigned int GET32 ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern void dummy ( unsigned int );

#define SYSAHBCLKCTRL 0x40048080

#define GPIODIRP0  0xA0002000
#define GPIOSETP0  0xA0002200
#define GPIOCLRP0  0xA0002280
#define GPIONOTP0  0xA0002300

#define MRT_BASE  0x40004000
#define MRT_INTVAL0 (MRT_BASE+0x00)
#define MRT_TIMER0  (MRT_BASE+0x04)
#define MRT_CTRL0   (MRT_BASE+0x08)
#define MRT_STAT0   (MRT_BASE+0x0C)

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
static void delay ( unsigned int x )
{
    unsigned int ra;
    for(ra=0;ra<x;)
    {
        if(GET32(MRT_STAT0)&1)
        {
            PUT32(MRT_STAT0,1);
            ra++;
        }
    }
}
int notmain ( void )
{
    unsigned int ra;

    ra=GET32(SYSAHBCLKCTRL);
    ra|=1<<10; //enable MRT
    PUT32(SYSAHBCLKCTRL,ra);

    led_init();
    led_off();

    PUT32(MRT_CTRL0,(0<<2)|(0<<0));
    PUT32(MRT_INTVAL0,12000000-1);

    while(1)
    {
        led_on();
        delay(3);
        led_off();
        delay(1);
    }


    return(0);
}
