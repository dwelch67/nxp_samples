
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


static void uart_string ( const char *s )
{
	while(*s)
	{
		uart_send(*s);
		s++;
	}
}

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
    //unsigned int rd;
    unsigned int max;
    unsigned int min;
    max=0;
	min=0; min--;
	//Sony SIRC 2.4ms start pulse
	//12000000 * 0.0024 = 0x7080
	//12000000 * 0.0012 = 0x3840
	//12000000 * 0.0006 = 0x1C20
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
		if(rc<0x5000) continue;
        if(rc>max) max=rc;
        if(rc<min) min=rc;
        hexstrings(rc);
        hexstrings(min);
        hexstring(max);
    }
}
//Sony SIRC
//12000000 * 0.0024 = 0x7080
//00007035 00006E04 000072EE 
//00006FE0 00006E04 000072EE 
//00006EF2 00006E04 000072EE 
//00007057 00006E04 000072EE 
//000073A9 00006E04 000073A9 
//00007178 00006E04 000073A9 
//00007123 00006E04 000073A9 
//000072DD 00006E04 000073A9 
//0000719A 00006E04 000073A9 
//00007156 00006E04 000073A9 

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
	//Sony SIRC 2.4ms start pulse
	//12000000 * 0.0024 = 0x7080
	//12000000 * 0.0012 = 0x3840
	//12000000 * 0.0006 = 0x1C20
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
		//if(rc<0x5000) continue;
		if(rc<0x6000) continue;
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
//Sony SIRC 2.4ms start pulse
//12000000 * 0.0024 = 0x7080
//12000000 * 0.0012 = 0x3840
//12000000 * 0.0006 = 0x1C20
//00007124 00001A63 000018A9 00001B95 
//00007146 00001A30 000018A9 00001B95 
//00007179 00001A0E 000018A9 00001B95 
//0000729A 000018ED 000018A9 00001B95 
//000072DE 000018BA 000018A9 00001B95 
//00007179 00001A0E 000018A9 00001B95 
//00007135 00001A63 000018A9 00001B95 
//000072EF 00001898 00001898 00001B95 
//0000719B 000019FD 00001898 00001B95 
//00007157 00001A41 00001898 00001B95 
//00007179 00001A0E 00001898 00001B95 
//00007135 00001A52 00001898 00001B95 



if(0)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
	unsigned int rd;
	unsigned int data[24];
	//Sony SIRC 2.4ms start pulse
	//12000000 * 0.0024 = 0x7080
	//12000000 * 0.0012 = 0x3840
	//12000000 * 0.0006 = 0x1C20
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
		//if(rc<0x5000) continue;
		if(rc<0x6000) continue;
		for(rd=0;rd<24;)
		{
			//falling edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))==0) break;
			}
			ra=GET32(STK_CVR);
			rc=(rb-ra)&STK_MASK;
			data[rd++]=rc;
			//rising edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))!=0) break;
			}
			rb=GET32(STK_CVR);
			rc=(ra-rb)&STK_MASK;
			data[rd++]=rc;
		}
		hexstring(0);
		hexstring(0);
		for(rd=0;rd<24;rd+=2)
		{
			hexstrings(data[rd+0]);
			hexstring(data[rd+1]);
		}
    }
}
//12 bit version
//00000000 
//00000000 
//00001B79 00003822 
//00001B6B 00001D3D 
//00001B7C 00003822 
//00001B6B 00001D4E 
//00001B6B 00001D4E 
//00001B5A 00001D4E 
//00001B6B 00003833 
//00001B5A 00001D5F 
//00001B5A 00001D4E 
//00001B5A 00003844 
//00001B5A 00001D5F 
//00001B49 00001D5F 
//00000000 
//00000000 
//00001B46 00003855 
//00001B38 00001D81 
//00001B27 00003866 
//00001B38 00001D81 
//00001B27 00001D92 
//00001B27 00001D92 
//00001B16 00003877 
//00001B27 00001D92 
//00001B16 00001DA3 
//00001B16 00003800 
//00001B8D 00001D2C 
//00001B8D 00001D2C 

if(0)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
	unsigned int rd;
	unsigned int data[30];
	//Sony SIRC 2.4ms start pulse
	//12000000 * 0.0024 = 0x7080
	//12000000 * 0.0012 = 0x3840
	//12000000 * 0.0006 = 0x1C20
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
		//if(rc<0x5000) continue;
		if(rc<0x6000) continue;
		for(rd=0;rd<30;)
		{
			//falling edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))==0) break;
			}
			ra=GET32(STK_CVR);
			rc=(rb-ra)&STK_MASK;
			data[rd++]=rc;
			//rising edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))!=0) break;
			}
			rb=GET32(STK_CVR);
			rc=(ra-rb)&STK_MASK;
			data[rd++]=rc;
		}
		hexstring(0);
		hexstring(0);
		for(rd=0;rd<30;rd+=2)
		{
			hexstrings(data[rd+0]);
			hexstring(data[rd+1]);
		}
    }
}
//15 bit version
//00000000 
//00000000 
//00001B24 00003877 
//00001B16 00001DA3 
//00001B16 00003800 
//00001B8D 00001DA3 
//00001B16 00001D2C 
//00001B8D 00001D2C 
//00001B8D 00003811 
//00001B7C 00001D3D 
//00001B7C 00001D2C 
//00001B7C 00003822 
//00001B7C 00001D3D 
//00001B6B 00001D4E 
//00001B6B 00001D4E 
//00001B6B 00001D4E 
//00001B6B 00003833 
//00000000 
//00000000 
//00001B46 00001D70 
//00001B38 00003866 
//00001B38 00003866 
//00001B27 00001D81 
//00001B27 00001D92 
//00001B27 00001D92 
//00001B27 00003877 
//00001B16 00001DA3 
//00001B16 00001DA3 
//00001B05 00003811 
//00001B8D 00001D2C 
//00001B7C 00001D3D 
//00001B7C 00001D3D 
//00001B7C 00001D2C 
//00001B8D 00003811 


if(0)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
	unsigned int rd;
	unsigned int code;
	//Sony SIRC 2.4ms start pulse
	//12000000 * 0.0024 = 0x7080
	//12000000 * 0.0012 = 0x3840
	//12000000 * 0.0006 = 0x1C20
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
		if(rc<0x5000) continue;
		//if(rc<0x6000) continue;
		for(rd=0;rd<15;rd++)
		{
			//falling edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))==0) break;
			}
			ra=GET32(STK_CVR);
			rc=(rb-ra)&STK_MASK;
			if(rc<0x1820) break;
			if(rc>0x2020) break;
			//rising edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))!=0) break;
			}
			rb=GET32(STK_CVR);
			rc=(ra-rb)&STK_MASK;
			if(rc<0x1820) break;
			if(rc>0x3C40) break;
			code<<=1;
			if(rc>0x2A30) code|=1;
			//else code|=0;
		}
		if(rd<15) continue;
		hexstring(code);
    }
}
//00005621 
//00005621 
//00005621 
//00005121 
//00005121 
//00005121 
//00001621 
//00001621 
//00001621 
//00005121 
//00005121 
//00005121 
//00002621 
//00002621 

//Hmm so this is a car stereo AIWA remote, is interesting has
//source disp, seek, mode, etc buttons and a volume up down
//one touch you get multiple hits, the SB-projects page says repeats
//every 45ms until released, and I see that on my scope too.
//Without a scope could easily look for the start pattern more than 
//once and see how far apart they are using the timer.

//Im not interested in the command and address parts lsb first just
//using the bits as read and printed.

//the volume up is just the one pattern
//00002421 
//00002421 
//00002421 
//00002421 
//for as long as you hold it

//volume down same thing
//00006421 
//00006421 
//00006421 

//but the other buttons, disp, seek, source, etc.  
//00003121 
//00003121 
//00003121 
//00003121 
//00005121 
//00005121
//a pattern as long as you hold it but then when you release the 0x5121s


if(1)
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
	unsigned int rd;
	unsigned int code;
	unsigned int state;
	//Sony SIRC 2.4ms start pulse
	//12000000 * 0.0024 = 0x7080
	//12000000 * 0.0012 = 0x3840
	//12000000 * 0.0006 = 0x1C20
	state=0;
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
		if(rc<0x5000) continue;
		//if(rc<0x6000) continue;
		for(rd=0;rd<15;rd++)
		{
			//falling edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))==0) break;
			}
			ra=GET32(STK_CVR);
			rc=(rb-ra)&STK_MASK;
			if(rc<0x1820) break;
			if(rc>0x2020) break;
			//rising edge
			while(1)
			{
				if((GET32(GPIOPIN0)&(1<<7))!=0) break;
			}
			rb=GET32(STK_CVR);
			rc=(ra-rb)&STK_MASK;
			if(rc<0x1820) break;
			if(rc>0x3C40) break;
			code<<=1;
			if(rc>0x2A30) code|=1;
			//else code|=0;
		}
		if(rd<15) continue;
		//hexstring(code);
		switch(code)
		{
			case 0x5121:
			{
				switch(state)
				{
					case 0x0A21:
					{
						state=0;
						uart_string("DSPL\r\n");
						break;
					}
					case 0x7121:
					{
						state=0;
						uart_string("MODE\r\n");
						break;
					}
					case 0x3121:
					{
						state=0;
						uart_string("SOURCE\r\n");
						break;
					}
					case 0x6621: //PRESET+
					{
						state=0;
						uart_string("PRESET+\r\n");
						break;
					}
					case 0x2621: //PRESET-
					{
						state=0;
						uart_string("PRESET-\r\n");
						break;
					}
					case 0x5621: //SEEK-
					{
						state=0;
						uart_string("SEEK-\r\n");
						break;
					}
					case 0x1621: //SEEK+
					{
						state=0;
						uart_string("SEEK+\r\n");
						break;
					}
					case 0x5821: //OFF
					{
						state=0;
						uart_string("OFF\r\n");
						break;
					}
					case 0x1421: //ATT
					{
						state=0;
						uart_string("ATT\r\n");
						break;
					}
					
				}
				state=0;
				break;
			}
			case 0x2421:
			{
				uart_string("VOLUP\r\n");
				state=0;
				break;
			}
			case 0x6421:
			{
				uart_string("VOLDN\r\n");
				state=0;
				break;
			}
			case 0x0A21: //DSPL
			case 0x7121: //MODE
			case 0x3121: //SOURCE
			case 0x6621: //PRESET+
			case 0x2621: //PRESET-
			case 0x5621: //SEEK-
			case 0x1621: //SEEK+
			case 0x5821: //OFF
			case 0x1421: //ATT
			{
				state=code;
				break;
			}
		}
		
		
		
		
		
		
    }
}























    return(0);
}
