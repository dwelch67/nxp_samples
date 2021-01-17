
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

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define GPIOPIN0   0xA0002100

static void uart_init ( void )
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
    if(0)
    {
        //38400 baud
        //reg 10-1 mul 244 38400.000000 0.000000
        PUT32(UARTFRGDIV,256-1);
        PUT32(UARTFRGMULT,244);
        PUT32(USART0BASE+0x20,10-1);
    }
    if(1)
    {
        //115200 baud
        //reg 6-1 mul 22 115107.913669 92.086331
        PUT32(UARTFRGDIV,256-1);
        PUT32(UARTFRGMULT,22);
        PUT32(USART0BASE+0x20,6-1);
    }
    if(0)
    {
        //115200 baud
        //reg 4-1 mul 161 115107.913669 92.086331
        PUT32(UARTFRGDIV,256-1);
        PUT32(UARTFRGMULT,161);
        PUT32(USART0BASE+0x20,4-1);
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


//static void uart_string ( const char *s )
//{
	//while(*s)
	//{
		//uart_send(*s);
		//s++;
	//}
//}

int notmain ( void )
{
    uart_init();
    hexstring(0x12345678);

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);
	
if(0)
{	
	while(1)
	{
		hexstring(GET32(STK_CVR));
		//counts down.
	}
}

if(0)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int max;
    max=0;
    while(1)
    {
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        //rising edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))!=0) break;
        }
        rb=GET32(STK_CVR);
        rc=(ra-rb)&STK_MASK;
        if(rc>max) max=rc;
        hexstrings(rc);
        hexstring(max);
    }
}
//000011E9 0001760C 
//00001260 0001760C 
//0000120B 0001760C 
//000175FB 0001760C 
//000018C0 0001760C 
//000011FA 0001760C 
//00001282 0001760C 
//000018AF 0001760C 
//0x1760C /12000000 = 0.007979
//8ms
//



if(0)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    //unsigned int rd;
    unsigned int max;
    unsigned int min;
    max=0;
	min=0; min--;
    while(1)
    {
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        //rising edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))!=0) break;
        }
        rb=GET32(STK_CVR);
        rc=(ra-rb)&STK_MASK;
		//12000000*0.004 = 48000
		if(rc<50000) continue;
        if(rc>max) max=rc;
        if(rc<min) min=rc;
        hexstrings(rc);
        hexstrings(min);
        hexstring(max);
    }
}
//0001761D 00017573 0001761D 
//000175D9 00017573 0001761D 
//000175EA 00017573 0001761D 
//000175FB 00017573 0001761D 
//0x00017573/12000000 = 0x0079669
//0x0001761D/12000000 = 0x0079810
//wow that is quite narrow.


if(0)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int max;
    unsigned int min;
    max=0;
	min=0; min--;
    while(1)
    {
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        //rising edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))!=0) break;
        }
        rb=GET32(STK_CVR);
        rc=(ra-rb)&STK_MASK;
		//12000000*0.004 = 48000
		if(rc<50000) continue;
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        rd=(rb-ra)&STK_MASK;
        if(rd>max) max=rd;
        if(rd<min) min=rd;
        hexstrings(rc);
        hexstrings(rd);
        hexstrings(min);
        hexstring(max);
    }
}
//000175FC 0000B891 0000B6E8 0000B8F7 
//000175EB 0000B880 0000B6E8 0000B8F7 
//0001760D 0000B85E 0000B6E8 0000B8F7 
//000175EB 0000B891 0000B6E8 0000B8F7 
//000175DA 0000B8B3 0000B6E8 0000B8F7 
//000175B8 0000B8C4 0000B6E8 0000B8F7 
//0001760D 0000B86F 0000B6E8 0000B8F7 
//000175DA 0000B8A2 0000B6E8 0000B8F7 
//0x0000B6E8/12000000 = 0.003902
//0x0000B8F7/12000000 = 0.003945



if(0)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
	unsigned int data[34];
    while(1)
    {
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        //rising edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))!=0) break;
        }
        rb=GET32(STK_CVR);
        rc=(ra-rb)&STK_MASK;
		//12000000*0.004 = 48000
		if(rc<50000) continue;
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        rc=(rb-ra)&STK_MASK;
		if(rc<0xB000) continue;
		for(rd=0;rd<34;) //experimentally determined
		{
			//rising edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))!=0) break;
			}
			rb=GET32(STK_CVR);
			rc=(ra-rb)&STK_MASK;
			data[rd++]=rc;
			//falling edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))==0) break;
			}
			ra=GET32(STK_CVR);
			rc=(rb-ra)&STK_MASK;
			data[rd++]=rc;
		}
		hexstring(0);
		hexstring(0);
		for(rd=0;rd<34;rd+=2)
		{
			hexstrings(data[rd+0]);
			hexstring(data[rd+1]);
		}
    }
}
//00000000 
//00000000 
//000018D2 000016EE 
//0000177F 00001710 
//0000175D 0000447C 
//000018F5 000016CC 
//000017A1 00004449 
//000018A0 000015DE 
//00001906 000016BB 
//000017C3 000016CC 
//000017A1 0000B890  <---- hmmm
//000018B1 00001710 
//0000176E 0000446B 
//000018F5 000016CC 
//000017A1 000016DD 
//00001790 000016FF 
//0000176E 00001721 
//000017D4 000016BB 
//000017C3 000016BB


if(0)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
	unsigned int data[34];
    while(1)
    {
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        //rising edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))!=0) break;
        }
        rb=GET32(STK_CVR);
        rc=(ra-rb)&STK_MASK;
		//12000000*0.004 = 48000
		if(rc<50000) continue;
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        rc=(rb-ra)&STK_MASK;
		if(rc<0xB000) continue;
		for(rd=0;rd<34;) //experimentally determined
		{
			//rising edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))!=0) break;
			}
			rb=GET32(STK_CVR);
			rc=(ra-rb)&STK_MASK;
			data[rd++]=rc;
			//falling edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))==0) break;
			}
			ra=GET32(STK_CVR);
			rc=(rb-ra)&STK_MASK;
			data[rd++]=rc;
		}
		hexstring(0);
		hexstring(0);
		for(rd=0;rd<34;rd+=2)
		{
			hexstrings(data[rd+0]);
			hexstrings(data[rd+1]);
			hexstring(data[rd+0]+data[rd+1]);
		}
    }
}
//12345678 
//00000000 
//00000000 
//000018E3 000016CC 00002FAF 
//000017A1 000016EE 00002E8F 
//00001790 00004449 00005BD9 
//000018A0 00001721 00002FC1 
//000017C3 00004416 00005BD9 
//000018D3 000016EE 00002FC1 
//00001790 000016FF 00002E8F 
//0000176E 00001710 00002E7E 
//0000175D 0000B8E5 0000D042 
//000018E4 000016CC 00002FB0 
//000017A1 00004449 00005BEA 
//000018A0 00001710 00002FB0 
//0000175D 00001732 00002E8F 
//000017C3 000016CC 00002E8F 
//000017A1 000016EE 00002E8F 
//00001790 000016EE 00002E7E 
//0000177F 00001710 00002E8F 
//12000000*0.002 = 0x5DC0
//12000000*0.001 = 0x2EE0
//12000000*0.004526 = 0xD428

if(1)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
	unsigned int code;
    while(1)
    {
		code=0;
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        //rising edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))!=0) break;
        }
        rb=GET32(STK_CVR);
        rc=(ra-rb)&STK_MASK;
		//12000000*0.004 = 48000
		if(rc<50000) continue;
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        rc=(rb-ra)&STK_MASK;
		if(rc<0xB000) continue;
		for(rd=0;rd<8;rd++)
		{
			//rising edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))!=0) break;
			}
			rb=GET32(STK_CVR);
			rc=(ra-rb)&STK_MASK;
			if(rc<3156) break;
			if(rc>9468) break;
			//falling edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))==0) break;
			}
			ra=GET32(STK_CVR);
			rc+=(rb-ra)&STK_MASK;
			//limit check?
			code<<=1;
			if(rc>18000) code|=1;
			//else code|=0;
		}
		if(rd<8) continue;
		//rising edge
		while(1)
		{
			if((GET32(GPIOPIN0)&(1<<7))!=0) break;
		}
		rb=GET32(STK_CVR);
		rc=(ra-rb)&STK_MASK;
		if(rc<3156) continue;
		if(rc>9468) continue;
		//falling edge
		while(1)
		{
			if((GET32(GPIOPIN0)&(1<<7))==0) break;
		}
		ra=GET32(STK_CVR);
		rc+=(rb-ra)&STK_MASK;
		if(rc<42000) continue;
		if(rc>54000) continue;
		for(rd=0;rd<8;rd++)
		{
			//rising edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))!=0) break;
			}
			rb=GET32(STK_CVR);
			rc=(ra-rb)&STK_MASK;
			if(rc<3156) break;
			if(rc>9468) break;
			//falling edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))==0) break;
			}
			ra=GET32(STK_CVR);
			rc+=(rb-ra)&STK_MASK;
			//limit check?
			code<<=1;
			if(rc>18000) code|=1;
			//else code|=0;
		}
		if(rd<8) continue;
		hexstring(code);
    }
}










    return(0);
}
