
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
.thumb_func
reset:
    ldr r0,=0x10001000
    mov sp,r0
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


