
extern unsigned int GET32 ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern void dummy ( unsigned int );

#define GPIODIRP0  0xA0002000
#define GPIOSETP0  0xA0002200
#define GPIOCLRP0  0xA0002280
#define GPIONOTP0  0xA0002300
#define GPIOPIN0   0xA0002100

int notmain ( void )
{
    unsigned int ra;
    ra=GET32(GPIODIRP0);
	ra&=(~(1<<7));
    ra|=1<<14;
    PUT32(GPIODIRP0,ra);

	while(1)
	{
		ra=GET32(GPIOPIN0);
		if(ra&1<<7) PUT32(GPIOCLRP0,(1<<14));
		else		PUT32(GPIOSETP0,(1<<14));
	}

    return(0);
}
