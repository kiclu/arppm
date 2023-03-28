# DO NOT EDIT/REMOVE THIS FILE!
# _start function is set up to be at address 0x0000 because PC is initialized
# to zero at startup. Use main() function as you would with any other C program.
# _start also initializes stack pointer to 0x8000.

.globl _start
.globl main
.extern gpu_init
.align 4

_start:
    nop             # buffer time for board to initialize
    li sp, 0x8000   # initialize stack pointer
    call gpu_init   # initialize GPU
    j main          # jump to main
