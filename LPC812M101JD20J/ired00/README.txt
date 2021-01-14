There is no code in this directory just screen captures from my scope.

NO SCOPE IS NEEDED FOR DECODING INFRARED REMOTES.

This simply gives a visual of what my remote looks like through
the receiver I purchased.

I bought one of these

https://www.sparkfun.com/products/14865

And a few of these (spares)

https://www.sparkfun.com/products/10266

This is a Vishay TSOP38238 which you can get cheaper elsewhere
certainly.  But I like to support sparkfun (and adafruit) from time
to time even though their prices and shipping are sometimes not
competitive.

Now because I wrote this after doing the work I happen to know this
sparkfun remote uses the NEC protocol.

https://www.sbprojects.net/knowledge/ir/nec.php

I like this sb-projects page for thse IR codes.  There are other
sites for sure that describe the protocols.  The above site explains
the concept in general you blink an infra-red (below red, slower than
the slowest frequency our eyes can see) led at a "carrier frequency".
Then the receiver does what is similar to an AM (radio) de-modulation
so you end up with a burst of blinking light that makes a pulse
come out of the receiver otherwise the receiver goes back to its not
excited idle state.

Sparkfun called it an IR Receiver Diode, I think it is more than
just a diode as there are receiver diodes that you have to add more
stuff to, Vishay who makes it calls it an IR Receiver Module which
I like better than IR Receiver Diode.  There are other models and brands
out there.  Also understand there are multiple carrier frequencies
supported and also the frequency of the infrared light can vary too,
so you may see many specs.  TV and other remote controls are in the
40Khz and slower carrier frequencies usually, I have found over time
that you can still decode with the wrong frequency receiver.  Due
to various principles of physics, even though you see the exact
pulse durations of say the NEC protocol say 9ms and 4.5ms, what you 
see come out of the receiver is not exactly that, how far away the
remote and the receiver are from each other, and other factors apply.
But it is more than good enough to reliably decode a remote and have
fun.

Yes I did not spend a lot of time on the scope capture measurements I
did them quickly so I could start on the software.  Also I could have
taken the remote apart and measured right at the transmitter led
to see the carrier frequency, etc.  But did not.  Maybe I will do that
some day.

I greatly prefer the protocols that start with a sync pattern that is
easy to detect from the data itself.  Makes it very easy to know 
you are at the start of the pattern.  You can look through the sb-projects
page and see a number of them are like this.  You may have a universal
remote that came with a tv or a dish or cable box or something and
you can program it to dozens of different remotes which means one of
a small list of protocols but different code/commands per.  You might
find a number of philips products use the same protocol but all have
different codes.  This sparkfun remote is actually a really good one
to start with note I was able to get a cr2032 battery in there even
though it takes a cr2025 (cr2032 is thicker and lasts longer)

A protocol with a long sync pattern in particular makes it easier to
get started without a scope.  I have been doing this off and on for
fun for MANY years and most of that time never used a scope, just
figured it out by counting timer ticks between input state changes
coming out of the ir receiver module.

Also note that not all ir receiver modules are pinned out the same
they are usually three pins, power, ground and output, but they
are not the same pinout so you can let smoke out of the part
if you are not super careful.  I have too many of these things and
if I dont keep track of them by putting them back in the bag with
the part number then I may simply toss them as I cant remember which
one was which sometimes. (sometimes you can tell by looking as they
may not all look alike).

See the other ired examples in order.

From the Vishay datasheet:

A PIN diode and a preamplifier are assembled on a leadframe, the epoxy
package contains an IR filter.

Okay I guess it is a diode.  There is some sort of a low pass filter
though.

