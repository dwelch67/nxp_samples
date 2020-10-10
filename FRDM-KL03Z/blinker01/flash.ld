
MEMORY
{
    vec : ORIGIN = 0x00000000, LENGTH = 0x400
    cfg : ORIGIN = 0x00000400, LENGTH = 0x10
    rom : ORIGIN = 0x00000410, LENGTH = 0x1000
    ram : ORIGIN = 0x20000000, LENGTH = 0x1000
}
SECTIONS
{
    .vector  : { *(.vectors*)       } > vec
    .fconfig : { *(.flashconfig*)   } > cfg
    .text    : { *(.text*)          } > rom
    .rodata  : { *(.rodata*)        } > rom
    .bss     : { *(.bss*)           } > ram
}
