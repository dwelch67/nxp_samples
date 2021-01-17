
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
.word 0x10001000  /* 0x00 */
.word reset       /* 0x04 */
.word hang        /* 0x08 */
.word hang        /* 0x0C */

.word hang        /* 0x10 */
.word hang        /* 0x14 */
.word hang        /* 0x18 */
.word hang        /* 0x1C */

.word hang        /* 0x20 */
.word hang        /* 0x24 */
.word hang        /* 0x28 */
.word hang        /* 0x2C */

.word hang        /* 0x30 */
.word hang        /* 0x34 */
.word hang        /* 0x38 */
.word hang        /* 0x3C */

.thumb_func
reset:
    bl notmain
    b hang
.thumb_func
hang:   b .

.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.thumb_func
.globl dummy
dummy:
    bx lr


