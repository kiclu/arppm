OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
    INST    (rx)    : ORIGIN = 0x0000, LENGTH = 0x2000
    DATA    (rw)    : ORIGIN = 0x2000, LENGTH = 0x1000
    RODATA  (r)     : ORIGIN = 0x3000, LENGTH = 0x1000
    BSS     (rw)    : ORIGIN = 0x4000, LENGTH = 0x1000
}

SECTIONS
{
  .text : {
    . = ALIGN(4);
    *(.text .text.*)
  }> INST

  .data : {
    . = ALIGN(4);
    *(.data .data.*)
  }> DATA

  .rodata : {
    . = ALIGN(4);
    *(.rodata .rodata.*)
  }> RODATA

  .bss : {
    . = ALIGN(4);
    *(.bss .bss.*)
  }> BSS
}