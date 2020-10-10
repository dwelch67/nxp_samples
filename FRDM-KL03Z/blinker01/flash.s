
.cpu cortex-m0
.thumb

.section .vectors, "a"

.word 0x20000400
.word reset
.word hang
.word hang

.word hang
.word hang
.word hang
.word hang

.word hang
.word hang
.word hang
.word hang

.word hang
.word hang
.word hang
.word hang

.section .flashconfig, "a"

.word 0xFFFFFFFF
.word 0xFFFFFFFF
.word 0xFFFFFFFF
.word 0xFFFF3DFE

.text
.thumb


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

