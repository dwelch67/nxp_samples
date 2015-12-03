
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2010
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ser.h"

unsigned char sdata[512];
unsigned char udata[512];
unsigned char rdata[5000];

//0 CMD_SUCCESS
//1 INVALID_COMMAND
//2 SRC_ADDR_ERROR
//3 DST_ADDR_ERROR
//4 SRC_ADDR_NOT_MAPPED
//5 DST_ADDR_NOT_MAPPED
//6 COUNT_ERROR
//7 INVALID_SECTOR
//8 SECTOR_NOT_BLANK
//9 SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION
//10 COMPARE_ERROR
//11 BUSY
//12 PARAM_ERROR
//13 ADDR_ERROR
//14 ADDR_NOT_MAPPED
//15 CMD_LOCKED
//16 INVALID_CODE
//17 INVALID_BAUD_RATE
//18 INVALID_STOP_BIT
//19 CODE_READ_PROTECTION_ENABLED

//-----------------------------------------------------------------------------
 unsigned char syncpatt[]=
{
    0x53, //[S]
    0x79, //[y]
    0x6E, //[n]
    0x63, //[c]
    0x68, //[h]
    0x72, //[r]
    0x6F, //[o]
    0x6E, //[n]
    0x69, //[i]
    0x7A, //[z]
    0x65, //[e]
    0x64, //[d]
    0x0D,
    0x0A,
};
//-----------------------------------------------------------------------------
 unsigned char syncpatt_ok[]=
{
    0x53, //[S]
    0x79, //[y]
    0x6E, //[n]
    0x63, //[c]
    0x68, //[h]
    0x72, //[r]
    0x6F, //[o]
    0x6E, //[n]
    0x69, //[i]
    0x7A, //[z]
    0x65, //[e]
    0x64, //[d]
    0x0D,
    0x0A,
    0x4F, //[O]
    0x4B, //[K]
    0x0D,
    0x0A,
};
//-----------------------------------------------------------------------------
 unsigned char okpatt[]=
{
    0x4F, //[O]
    0x4B, //[K]
    0x0D,
    0x0A,
};
//-----------------------------------------------------------------------------
 unsigned char crystalpatt[]=
{
    0x31, //[1]
    0x32, //[2]
    0x30, //[0]
    0x30, //[0]
    0x30, //[0]
    0x0D,
    0x0A,
};
//-----------------------------------------------------------------------------
 unsigned char crystalpatt_ok[]=
{
    0x31, //[1]
    0x32, //[2]
    0x30, //[0]
    0x30, //[0]
    0x30, //[0]
    0x0D,
    0x0A,
    0x4F, //[O]
    0x4B, //[K]
    0x0D,
    0x0A,
};
//-----------------------------------------------------------------------------
 unsigned char echo_off[]=
{
    0x41, //[A]
    0x20, //[ ]
    0x30, //[0]
    0x0D,
    0x0A,
};
//-----------------------------------------------------------------------------
 unsigned char echo_off_ok[]=
{
    0x41, //[A]
    0x20, //[ ]
    0x30, //[0]
    0x0D,
    0x0A,
    0x30, //[0]
    0x0D,
    //0x0A, //want this missing so that everything from here starts with 0xA
};
//-----------------------------------------------------------------------------
unsigned char cmd_success_short[]=
{
    0x30, //[0]
    0x0D,
    //0x0A,
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int sync_wait_for ( unsigned char *patt, unsigned int len )
{
    unsigned int ra,rb;
//    unsigned int rblast;

//printf("-- %u\n",len);

//    rblast=0;
    while(1)
    {
        rb=ser_copystring(rdata);
//if(rb!=rblast)
//{
    //for(ra=rblast;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
    //rblast=rb;
//}
        if(rb>=len)
        {
            for(ra=0;ra<len;ra++) if(patt[ra]!=rdata[ra]) break;
            if(ra<len)
            {
printf("fail\n");
for(ra=0;ra<len;ra++) printf("0x%02X 0x%02X %c\n",rdata[ra],patt[ra],rdata[ra]);


                ser_close();
                return(1);
            }
            break;
        }
    }
    ser_dump(len);
    return(0);
}
//-----------------------------------------------------------------------------
int watch_for ( void  )
{
    unsigned int ra,rb;
    unsigned int rblast;

printf("xx\n");

    rblast=0;
    while(1)
    {
        rb=ser_copystring(rdata);
if(rb!=rblast)
{
    for(ra=rblast;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
    rblast=rb;
}
    }
    return(0);
}
//-----------------------------------------------------------------------------
unsigned int wait_for_packet ( void )
{
    unsigned int ra,rb;

    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x0A)
            {
                for(ra=0;ra<rb;ra++)
                {
                    if(rdata[ra]==0x0A) break;
                    printf("discarding 0x%02X ",rdata[ra]);
                    if((rdata[ra]>=0x20)&&(rdata[ra]<127)) printf("[%c]",rdata[ra]);
                    printf("\n");
                    ser_dump(1);
                }
                continue;
            }
            for(ra=0;ra<rb;ra++) if(rdata[ra]==0x0D) break;
            if(ra<rb) break;
        }
    }
    ra++;
    ser_dump(ra);
    rdata[ra]=0;
    return(ra);
}
//-----------------------------------------------------------------------------
unsigned int get_return_code ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=wait_for_packet();
    if(rb<3) return(111);
    rb--;
    rc=0;
    for(ra=1;ra<rb;ra++)
    {
        if(rdata[ra]<0x30) return(112);
        if(rdata[ra]>0x39) return(113);
        rc*=10;
        rc+=rdata[ra]&0xF;
    }
    if(rc) printf("returned %u\n",rc);
    else { printf("."); fflush(stdout); }
    return(rc);
}
//-----------------------------------------------------------------------------
int flash_erase_all ( void )
{
    unsigned int ra;
    ra=0;
    sdata[ra++]='P';
    sdata[ra++]=' ';
    sdata[ra++]='0';
    sdata[ra++]=' ';
    sdata[ra++]='7';
    sdata[ra++]=0x0D;
    sdata[ra++]=0x0A;
    ser_senddata(sdata,ra);
    if(get_return_code()) return(1);

    ra=0;
    sdata[ra++]='E';
    sdata[ra++]=' ';
    sdata[ra++]='0';
    sdata[ra++]=' ';
    sdata[ra++]='7';
    sdata[ra++]=0x0D;
    sdata[ra++]=0x0A;
    ser_senddata(sdata,ra);
    if(get_return_code()) return(1);

    return(0);
}
//-----------------------------------------------------------------------------
int lpc_isp_synchronize ( void )
{
    sdata[0]='?';
    sdata[1]=0x0D;
    sdata[2]=0x0A;
    ser_senddata(sdata,3);
    if(sync_wait_for(syncpatt,sizeof(syncpatt))) return(1);
    ser_senddata(syncpatt,sizeof(syncpatt));
    if(sync_wait_for(syncpatt_ok,sizeof(syncpatt_ok))) return(1);
    ser_senddata(crystalpatt,sizeof(crystalpatt));
    if(sync_wait_for(crystalpatt_ok,sizeof(crystalpatt_ok))) return(1);
    printf("synchronized\n");
    return(0);
}
//-----------------------------------------------------------------------------
int do_nxp_stuff ( void )
{
    unsigned int ra;


    if(lpc_isp_synchronize()) return(1);

    //ra=0;
    //sdata[ra++]='J';
    //sdata[ra++]=0x0D;
    //sdata[ra++]=0x0A;
    //ser_senddata(sdata,ra);
    //watch_for();

    //ra=0;
    //sdata[ra++]='K';
    //sdata[ra++]=0x0D;
    //sdata[ra++]=0x0A;
    //ser_senddata(sdata,ra);
    //watch_for();

    //ra=0;
    //sdata[ra++]='N';
    //sdata[ra++]=0x0D;
    //sdata[ra++]=0x0A;
    //ser_senddata(sdata,ra);
    //watch_for();

    //Turn off echo
    ser_senddata(echo_off,sizeof(echo_off));
    if(sync_wait_for(echo_off_ok,sizeof(echo_off_ok))) return(1);
    printf("echo off\n");
    //----
    //the ra-1 is so that everything from the chip is 0x0A...stuff...0x0D
    //----
    //Unlock
    ra=0;
    sdata[ra++]='U';
    sdata[ra++]=' ';
    sdata[ra++]='2';
    sdata[ra++]='3';
    sdata[ra++]='1';
    sdata[ra++]='3';
    sdata[ra++]='0';
    sdata[ra++]=0x0D;
    sdata[ra++]=0x0A;
    ser_senddata(sdata,ra);
    //if(sync_wait_for(cmd_success,sizeof(cmd_success))) return(1);
    if(get_return_code()) return(1);
    printf("unlocked\n");

    printf("Erase All\n");
    if(flash_erase_all())
    {
        printf("flash_erase_all() failed\n");
        return(1);
    }
    printf("erased\n");

    return(0);
}
//-----------------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    if(argc<2)
    {
        fprintf(stderr,"prognxp_erase /dev/ttySOMETHING\n");
        return(1);
    }

    if(ser_open(argv[1]))
    {
        printf("ser_open(%s) failed\n",argv[1]);
        return(1);
    }
    printf("port opened\n");

    do_nxp_stuff();

    ser_close();

    return(0);
}
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2010
//-----------------------------------------------------------------------------

