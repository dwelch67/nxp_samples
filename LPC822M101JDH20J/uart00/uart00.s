
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
    b .
    ldr r1,=0x4006401C
    mov r0,#0x55
    str r0,[r1]
    b .

.end
