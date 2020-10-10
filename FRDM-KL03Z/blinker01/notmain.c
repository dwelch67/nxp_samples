
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

//#define GPIOB_PDOR 0x400FF040
//#define GPIOB_PSOR 0x400FF044
//#define GPIOB_PCOR 0x400FF048
//#define GPIOB_PTOR 0x400FF04C
//#define GPIOB_PDIR 0x400FF050
//#define GPIOB_PDDR 0x400FF054

#define PORTB_PCR10 0x4004A028

#define GPIOB_PDOR 0xF8000040
#define GPIOB_PSOR 0xF8000044
#define GPIOB_PCOR 0xF8000048
#define GPIOB_PTOR 0xF800004C
#define GPIOB_PDIR 0xF8000050
#define GPIOB_PDDR 0xF8000054

#define SIM_SCGC5 0x40048038
#define SIM_COPC  0x40048100
int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    PUT32(SIM_COPC,0x00000000);

    ra=GET32(SIM_SCGC5);
    ra|=1<<10;
    PUT32(SIM_SCGC5,ra);

    ra=GET32(PORTB_PCR10);
    ra&=((~7)<<8);
    ra|=(( 1)<<8);
    PUT32(PORTB_PCR10,ra);

    ra=GET32(GPIOB_PDDR);
    ra|=1<<10;
    PUT32(GPIOB_PDDR,ra);

    while(1)
    {
        PUT32(GPIOB_PTOR,1<<10);
        for(rx=0;rx<40000;rx++) dummy(rx);
/*        PUT32(GPIOB_PSOR,1<<10);
        for(rx=0;rx<40000;rx++) dummy(rx);*/
    }
    return(0);
}
