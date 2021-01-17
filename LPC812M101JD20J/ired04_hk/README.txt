
read ired00 README first.

I have a DirecTV RC 23 remote, in there there is a code to program
a Hello Kitty TV code 10451.

This appears to be Mitsubishi protocol on the SB Projects page.

8Ms pulse
4ms gap
8 bits of 
526us pulse 
1 is 2ms-526us
0 is 1m2-526us
There is a extra 526us pulse to close out the 8th bit.
Then a 4ms gap
and then 8 more bits with an extra pulse to close out the last bit.


so not knowing this

First if(0) detects a long pulse of 

//000011E9 0001760C 
//00001260 0001760C 
//0000120B 0001760C 
//000175FB 0001760C 
//000018C0 0001760C 
//000011FA 0001760C 
//00001282 0001760C 
//000018AF 0001760C 

//0x1760C /12000000 = 0.007979

second if(0) checks the min max

//0001761D 00017573 0001761D 
//000175D9 00017573 0001761D 
//000175EA 00017573 0001761D 
//000175FB 00017573 0001761D 
//0x00017573/12000000 = 0x0079669
//0x0001761D/12000000 = 0x0079810
//wow that is quite narrow.

next if(0)

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

the off period between the main pulse and the next pulse is 4ms,
half of the big pulse, no surprise there.


next if(0)

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

Before this point we suspect the mitshubishi protocol based on the
sb-projects page (there are likely protocols not listed there, I
happen to know this for a fact). But at this point we see this 4ms
thing in the middle further re-inforcing that this is that protocol.

The first numbers, 12000000*0.000526 = 6312 = 0x18A8.  So this
looks to be on par.  We could min max it but...

next if(0) only difference is printing the sum

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

And the last if(0) works fine.  

It breaks it into four parts, the start pattern,
The first 8 bits of the code (address)
The 4ms gap
The second 8 bits of the code (command)

000028A8 
000028A8 
000028A8 
000028A8 
00002848 
00002848 
00002848 
00002828 
00002848 
000028E8 
000028E8 
000028E8 
000028E8 
000028E8 
000028E8 
00002818 
00002818 
00002818 

So that is the mitshubishi protocol.
