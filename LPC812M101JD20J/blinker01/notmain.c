
extern unsigned int GET32 ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern void dummy ( unsigned int );

#define GPIODIRP0  0xA0002000
#define GPIOSETP0  0xA0002200
#define GPIOCLRP0  0xA0002280
#define GPIONOTP0  0xA0002300

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(GPIODIRP0);
    ra|=1<<10;
    ra|=1<<14;
    ra|=1<<15;
    ra|=1<<16;
    ra|=1<<17;
    PUT32(GPIODIRP0,ra);

    PUT32(GPIOSETP0,(1<<14)|(1<<16));
    PUT32(GPIOCLRP0,(1<<15)|(1<<17));

    while(1)
    {
        PUT32(GPIONOTP0,(1<<14)|(1<<15)|(1<<16)|(1<<17));
        for(ra=0;ra<200000;ra++) dummy(ra);
    }

    return(0);
}
