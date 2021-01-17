
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


int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int max;

    uart_init();
    hexstring(0x12345678);

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);
	
if(0)
{	
	hexstring(GET32(STK_CVR));
    //counts down.
}

if(0)
{
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
//I am using NEC protocol so am looking for 9ms as the start pulse
//https://www.sbprojects.net/knowledge/ir/nec.php
//0001A6FD 0001A6FD   0x1A6FD/12000000  = 0.009
//00001C12 0001A6FD 
//00001AE0 0001A6FD 
//00001AF1 0001A6FD 
//00001B02 0001A6FD 
//00001B13 0001A6FD 
//00001C45 0001A6FD 
//00001C23 0001A6FD 
//00001C01 0001A6FD 
//00001C67 0001A6FD 
//00001C45 0001A6FD 
//00001C23 0001A6FD 
//00001C12 0001A6FD 

if(0)
{
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
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        rc=GET32(STK_CVR);
        rd=(ra-rb)&STK_MASK;
        if(rd<0x10000) continue;
        hexstrings(rd);
        rd=(rb-rc)&STK_MASK;
        hexstring(rd);
    }
}
//0001A687 0000D06E  0xD06E/12000000 4.446 ms
//0001A7DB 0000CEF8 
//0001A676 0000D05D 
//0001A6BA 0000D008 
//0001A7FD 0000CED6 
//0001A81F 0000CED6 
//0001A6CB 0000D02A 
//0001A687 0000671C <--- repeats here not all remotes do this
//0001A6FE 000066D8  0x66DB/12000000  2.194 ms
//0001A6ED 000066D8 
//0001A6FE 000066C7 
//0001A687 0000674F 
//0001A6FE 000066D8 
//0001A6FE 000066D8 
//0001A6BA 0000671C 
//0001A70F 000066C7 
//0001A6FE 000066C7 
//0001A6A9 0000670B 
//0001A698 0000D07F 
//0001A6ED 0000D008 
//0001A676 0000D06E 
//0001A6A9 0000D03B 
//so seeing the 9ms/4.5ms sync pattern
//for repeats am seeing 9ms/2.25ms.

//you are not going to see exact values and that is just fine.

if(0)
{
    unsigned int data[64];
    unsigned int code;
    while(1)
    {
        //look for sync pattern
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
        if(rc<0x10000) continue;
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        rc=(rb-ra)&STK_MASK;
        if(rc<0xC000) continue;
        for(rd=0;rd<64;)
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
        //rising edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))!=0) break;
        }
        hexstring(0);
        hexstring(0);
        max=0;
        code=0;
        for(rd=0;rd<64;rd+=2)
        {
            max+=data[rd+0];
            hexstrings(data[rd+0]);
            hexstrings(data[rd+1]);
            if(data[rd+1] < 0x3000)
            {
                uart_send('0');
                code<<=1;
                code|=0;
            }
            else
            {
                uart_send('1');
                code<<=1;
                code|=1;
            }
            uart_send(0x0D);
            uart_send(0x0A);
        }
        max>>=5;
        hexstring(max);
        hexstring(code);
        
        
    }
}
//00001C57 00001AEA 0
//00001AE2 00001B2E 0
//00001B15 00001AEA 0
//00001AE2 00001B1D 0
//00001B26 00001AEA 0
//00001AD1 00001B2E 0
//00001B26 00001AEA 0
//00001AD1 00001B2E 0
//00001B26 00004BDB 1
//00001C36 00004C0E 1
//00001C14 00004C30 1
//00001C69 00004BDB 1
//00001C47 00004BFD 1
//00001C14 00004C41 1
//00001BE1 00004C63 1
//00001C36 00004C0E 1
//00001C14 00001B2E 0
//00001B15 00004BEC 1
//00001C36 00004C0E 1
//00001C03 00001B50 0
//00001B04 00001AFB 0
//00001AC0 00001B3F 0
//00001B15 00004BEC 1
//00001C25 00001B2E 0
//00001B15 00004BEC 1
//00001C36 00001B0C 0
//00001AC0 00001B50 0
//00001AF3 00004C0E 1
//00001C14 00004C30 1
//00001BF2 00004C52 1
//00001C47 00001AFB 0
//00001AD1 00004C41 1
//00001B8F              1b8F/12000000  = 5.88 ms
//00FF629D 
//
//same button same code each time.
//all the buttons from this sparkfun remote
//00FF629D  power
//00FF22DD  A
//00FF02FD	B
//00FFC23D  C
//00FF9867	up
//00FF38C7  down
//00FF30CF  left
//00FF7A85  right
//00FF18E7  middle
//Different remote
//00001B36 000017A9 0
//00001A27 00004955 1
//00001A49 00001776 0
//000019E3 000015EF 0
//00001B6A 00001787 0
//00001A49 00004933 1
//000019F4 000017CB 0
//00001A16 0000489A 1
//00001A05 00004977 1
//00001A38 00001787 0
//00001A49 00004944 1
//000019E3 000048CD 1
//00001A5A 00004922 1
//00001A05 000017BA 0
//00001C8B 00004702 1
//00001A27 0000159A 0
//00001B48 00004966 1
//00001A38 00001787 0
//00001A49 00001776 0
//000019E3 000015DE 0
//00001B7B 00001776 0
//00001A5A 00001765 0
//000019F4 000017CB 0
//00001A05 000015CD 0
//00001B04 000017DC 0
//00001A05 00004988 1
//00001A16 00004966 1
//00001A49 00004867 1
//00001A38 00004944 1
//000019E3 000049AA 1
//00001A05 00004977 1
//00001A27 000048EF 1
//00001A5F 
//45BA807F 
//
//45BA807F 
//45BA40BF 
//45BAC03F 
//45BA20DF 
//45BAA05F 
//45BA609F 
//and so on

//00FF629D
//00000000 11111111 01100010 10011101
//for this sparkfun remote the address 0 is inverted 
//and the command 0x62 is inverted to 0x9D


//for this pioneer cd player
//45BA807F 
//01000101 10111010 10000000 01111111
//and the address 0x45 and command 0x80 are also invertd properly
//so you can check the codes too.


if(1)
{
    unsigned int code;
	unsigned int lastcode;
	lastcode=0;
    while(1)
    {
        //look for sync pattern
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
        if(rc<0x10000) continue;
        //falling edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))==0) break;
        }
        ra=GET32(STK_CVR);
        rc=(rb-ra)&STK_MASK;
        if(rc<0xC000)
		{ 
			if(0) //repeat or not
			{
				//hexstring(rc);
				//12000000*0.0025 = 0x7530
				if(rc>0x5000)
				{
					//call this a repeat
					if(lastcode)
					{
						code=lastcode;
						hexstring(code);
					}
				}
			}
			continue;
		}
		code=0;
        for(rd=0;rd<32;rd++)
        {
            //rising edge
            while(1)
            {
                if((GET32(GPIOPIN0)&(1<<7))!=0) break;
            }
            rb=GET32(STK_CVR);
            rc=(ra-rb)&STK_MASK;
			if(rc>0x3000) break;
			if(rc<0x1000) break;
            //falling edge
            while(1)
            {
                if((GET32(GPIOPIN0)&(1<<7))==0) break;
            }
            ra=GET32(STK_CVR);
            rc=(rb-ra)&STK_MASK;
			code<<=1;
			if(rc>0x3000) code|=1;
        }
        //rising edge
        while(1)
        {
            if((GET32(GPIOPIN0)&(1<<7))!=0) break;
        }
		if(1)
		{
			//check code
			ra=(code&0xFF00FF00)>>8;
			rb=(code&0x00FF00FF);
			rc=ra^rb;
			if(rc!=0x00FF00FF)
			{
				lastcode=0;
				continue;
			}
			if(1)
			{
				//reduce to the 16 bit address + command
				code=((code>>16)&0xFF00)|((code>>8)&0xFF);
			}
		}
		lastcode=code;
        hexstring(code);
    }
}










    return(0);
}
