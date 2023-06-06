OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
    INST    (rx)    : ORIGIN = 0x0000, LENGTH = 0x4000
    RODATA  (r)     : ORIGIN = 0x4000, LENGTH = 0x3600
    SRODATA (r)     : ORIGIN = 0x7600, LENGTH = 0x0200
    DATA    (rw)    : ORIGIN = 0x7800, LENGTH = 0x0200
    BSS     (rw)    : ORIGIN = 0x7A00, LENGTH = 0x0300
    STACK   (rw)    : ORIGIN = 0x7D00, LENGTH = 0x0300
}

SECTIONS
{
  . = 0x0000;

  .text : {
    . = ALIGN(4);
    *(.text .text.*)
  }> INST

  .rodata : {
    . = ALIGN(4);
    *(.rodata .rodata.*)
  }> RODATA

  .srodata : {
    . = ALIGN(4);
    *(.srodata .srodata.*)
  }> SRODATA

  .data : {
    . = ALIGN(4);
    *(.data .data.*)
  }> DATA

  .bss : {
    . = ALIGN(4);
    *(.bss .bss.*)
  }> BSS
}