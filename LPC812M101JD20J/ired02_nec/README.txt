
read ired00 README first.

I have a/some remotes handy that use the NEC protocol.

If you do not then that is fine this code and readme walk you through
the process.  This is really a handful of examples in one, there
are some left justifide if(0)'s in the code (well one is an if(1))
so start with the first one and make it an if(1) then later an if(0)
and work your way through them.

First off as in ired01 you need to either use that solution or
use the uart and sample the input pin, have to figure out if it idles
(no ir detected) high or low.  This code is all written for high but
you can easily change it once you see what is going on here.

Protocols like the NEC protocol have a start or sync pattern that
starts with a long pulse.  So if the input idles high and the pulse
is long it will be a long low pulse.  So we look for a falling edge
read the timer, look for a rising edge, read the timer then subtract
the times.  The systick timer in this part counts down so you
subtract the first time measurement from the second.  Note that it is
a 24 bit timer not 32 so you have to mask it with 0x00FFFFFF for the
math to work (for rollarounds to work right, also the timer has
to count to 0x00000000 and roll to 0x00FFFFFF for the math to work).
If you are using a count up timer then subtract in the other
direction.

Also just use a uart and polling to figure out some remote you have
do not try interrupts or going without the uart or something like
the uart that you can see lists of numbers.  Just do it this way.

The first if(0) is going to demonstrate that the counter counts down,
even if you think you remember, just do this to be sure, and not
end up driving yourself crazy.  You defintely do not have to but it
may help your understanding if you know the relationship between
the timer you are using and a wall clock.  I determined I am getting
the full system clock of 12mhz being counted out.  To develop a
decoder for a specific remote or the protocol that remote uses you
dont actually need to know this as you will see.

So next is to simply measure a/the first pulse.  Lets assume we dont
know the protcol at first, this will help.

Assuming your room is not filled with IR, you dont have the sunlight
diretly aimed at the receiver, etc.  Then the first pulse it sees
should only happen when you first press a button (and sometimes repeat
depending on the remote and how long you hold the button).  I like
to measure the max so that if I am getting various times (noise in
the room etc).  I can see what the biggest one is and then modify
this code to filter just for that as we see in the next example.

//0001A6FD 0001A6FD   0x1A6FD/12000000  = 0.009023
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

The NEC protocol starts with a 9ms on pulse ("on" is low for us)

and we are seeing roughly that.  Now while we are here we can for
fun look at those other numbers they all seem to be in the 0x1B00
range plus or minus a bit.

0x1B00/12000000 = 576us

If you look at the protocol the "on" periods are either the first
pulse which is 9ms or other pulses that are all supposed to be 560us.
The encoding of ones and zeros has to do with the distance between
the 560us pulses, so this is looking good so far, I do not care that
576 is a lot of ms greater than 560.

Next if(0) is measuring the whole sync pattern.

We saw some 0x1Axxx numbers above so 0x10000 gives us plenty of margin
and should be well above the other pulses from this remote (as we
saw above).

        if(rd<0x10000) continue;

So this filters out the first low pulse to be the start pulse, and
then we display both the start pulse and the off period after.

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

This remote happens to support the repeat command, the other remote
I used repeats a different way, it just sends the full code which
can be a PITA, or not, your call.  The above shows me that although
we dont see the exact

12000000*0.00900 = 0x1A5E0
12000000*0.00450 = 0xD2F0
12000000*0.00225 = 0x6978

We can easily distinguish the start pulse and a normal gap and a
repeat gap.

Next if(0)



first filter looks for the start pulse

        if(rc<0x10000) continue;

second looks for the normal, not repeat, gap, filters out the
repeats

        if(rc<0xC000) continue;

So we dive in assuming that this is a NEC protocol as documented.
Carefully not that there is at the end an extra pulse if you will.
This sbproject pages indicates a bit is a pulse with a gap, so to
get the last bit of the 32 bits you need an extra pulse (33 pulses
with 32 gaps).

There are protocols that look like NEC but are not, look at the
JVC one on the sbprojects page.  It has a start pulse that is long
then a gap that is half the size, then pulses with long or short
gaps.  But you do not have 32 gaps.  If you just go in and count
up to 32 and only press the button once without a repeat (in a dark or
noise free room).  It wont get through this loop.  So if I had not
seen the thing on the scope I would start this next loop much smaller

        for(rd=0;rd<64;)

also I might not know what protocol it is yet, some have fixed sized
gaps and long and short pulses and some are biphase-l meaning a
zero is a 01 or a gap and a pulse (of the same size) and a one is a
10 or a a pulse and a gap, so if you have a number of zeros in a row
you will get 01010101 or all the same pulses and gaps. likewise ones
would be 1010101, but one is how did it start and two is if you get a code
that is 1001 the pulses will be 10010110  so there are some short
ones and long ones 1 00 1 0 11 0 because the bit cells combined.  So
those are different software to decode, but a short loop that is
simply measuring the pulses and gaps without validating them and
just printing them out, is the next step after you figure out if you
have a start pattern and are decoding that start pattern.

If you dont know the protocol this is where you figure it out, different
buttons on the remote may change the pulse/gap measurements and you
can decide how you want to decode it...

In this case with a proper nec protocol I did 64 measurements, 32
pulses and 32 gaps, and then for grins waited for the last pulse to
finish (if this was not printing stuff on the uart it would get
filtered out by the sync pattern measurement it would be too short).

It should be obvious that this if(0) was developed in stages, first
measure some number of pulses and gaps, then seeing what was going
on or confirming it was the nec protocol then creating a filter
for long vs short on the gap which encodes zero/one.  Here again
I dont care if that is 2.25ms-560us or 1.12ms-560us exactly I just
took what I saw and picked a filter value somewhere in the middle.

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

The first column is the pulses the second the gaps and as we saw
before the pulses are all about the same length.  The gaps though are
short and long.

2250-560 = 1690
1120-560 = 560
1690/560 = 3

The longs are 3 times as long as the shorts. And we are seeing that.
So I was easily able to decode the ones and zeros then I added code
to capture the 32 bit code (address plus command as stated on the
sb-projects page) and print that (along with the average of the
pulses just to see how they were doing compared to 560us).

The next if(0) was also built in parts, take the prior remove the
recording of lengths and just start decoding.  Put a few filters
in in case the pulses are not the right size, etc...

Evolve that to check to see if the protocol matches what the sb-project
page is showing with respect to the first 8 bits is address, the
next 8 is the address inverted, the next 8 the command and the last 8
the command inverted.

//00FF629D
//00000000 11111111 01100010 10011101

For that one code that is correct, using the if(1) within this example
to always check the code confirmed that all the buttons were producing
codes with the proper inverted checks.  Then since we see that this
is really 8 bits of address and 8 bits of command which is 16 bits total
I added another if(1) to reduce the result code down to its 16 bit
command.  I also added a repeat feature if desired.

A fun/interesting way to see if your remote uses a repeat pattern
(a common pattern for all buttons as in the NEC protocol), is say
press the volumen down once, put your hand in front of the remote
led, press some other button volume up or power off or whatever, press
and hold so it starts and keeps repeating, then move your hand.  If it
keeps doing the volume down that is a repeat if it repeats the
actual button being pressed then not.

One of the nec protocol remotes simply repeats the same full command
which is a pain because it is like having a non-debounced button
you might want to just hit a button once, but it is so sensitive you
get two or three codes that come out which like non-debounced button
you have to filter in your application.  This sparkfun remote sends
one full command then repeats if you do not want to support a
repeat or only want to support repeats for certain commands (like
volume up and down only) then you can add code like if a repeat
pattern and the code is volume down then repeat down-ing the volume.

First time I did this took me a long time to figure out. There was
a public internet and early search engines, but I still didnt know what
I was looking at.  And I was just learning to program my first MCU
which you had to pull out of the socket and put into a programmer with
a zif socket to program. So I was dealing with all of that.  And pretty
sure I didnt have a uart working yet, had not realized the massive
value there, probably had one led I could blink.  I have reached this
point in the code turn the led on and infinite loop. If the pulse is
longer than X then blink the led once. and keep increasing X until
it doesnt work.  I am pretty sure at some point once I could get the
right transceiver to convert ttl level to RS232C levels (RS232 is a
connector/pinout spec and voltage level spec it is NOT A PROTOCOL SPEC)
then I was able to bit bang uart signals and then with a bit banged
uart could send data and not blink leds...So this probably took me
weeks the first time.  Now it takes minutes to write this code
from scratch.

some remotes burst a few on one button press, some a few then go
into repeats, some one code then repeats, it varies.

The sparkfun remote that I received, I get one code then if I hold
it I get repeats.  So I dont have to "debounce" the commands coming
in I can just ignore repeats and require a press and relase and can
essentually use this as several buttons of user interface to my mcu
with only needing a three pin small device and not several buttons
on the board.
