OUTPUT_ARCH( "riscv" )
ENTRY( _start )

MEMORY
{
    INST    (rx)    : ORIGIN = 0x0000, LENGTH = 0x3000
    RODATA  (r)     : ORIGIN = 0x3000, LENGTH = 0x2000
    SRODATA (r)     : ORIGIN = 0x5000, LENGTH = 0x2000
    DATA    (rw)    : ORIGIN = 0x7000, LENGTH = 0x0400
    BSS     (rw)    : ORIGIN = 0x7400, LENGTH = 0x0400
    STACK   (rw)    : ORIGIN = 0x7800, LENGTH = 0x0800
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