
MEMORY
{
    ram : ORIGIN = 0x10000800, LENGTH = 0x800
}

SECTIONS
{
    .text : { *(.text*) } > ram
    .rodata : { *(.rodata*) } > ram
    .bss : { *(.bss*) } > ram
}

