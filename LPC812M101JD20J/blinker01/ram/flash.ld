
MEMORY
{
    rom : ORIGIN = 0x10000300, LENGTH = 0x1000
}
SECTIONS
{
    .text   : { *(.text*)   } > rom
    .rodata : { *(.rodata*) } > rom
    .bss    : { *(.bss*)    } > rom
}

