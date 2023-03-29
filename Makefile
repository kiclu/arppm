DIR_BUILD = build
DIR_INC	  = h
DIR_SRC	  = src

ARPPM_IMG 		= arppm
ARPPM_ASM 		= arppm.asm
ARPPM_MIFDUMP	= arppm.mifdump

# Try to infer the correct TOOLPREFIX if not set
ifndef TOOLPREFIX
TOOLPREFIX := $(shell if riscv32-unknown-elf-objdump -i 2>&1 | grep 'elf32-big' >/dev/null 2>&1; \
	then echo 'riscv32-unknown-elf-'; \
	elif riscv32-linux-gnu-objdump -i 2>&1 | grep 'elf32-big' >/dev/null 2>&1; \
	then echo 'riscv32-linux-gnu-'; \
	elif riscv32-unknown-linux-gnu-objdump -i 2>&1 | grep 'elf64-big' >/dev/null 2>&1; \
	then echo 'riscv32-unknown-linux-gnu-'; \
	else echo "***" 1>&2; \
	echo "*** Error: Couldn't find a riscv32 version of GCC/binutils." 1>&2; \
	echo "*** To turn off this error, run 'gmake TOOLPREFIX= ...'." 1>&2; \
	echo "***" 1>&2; exit 1; fi)
endif

AS 		= ${TOOLPREFIX}as
CC 		= ${TOOLPREFIX}gcc
LD 		= ${TOOLPREFIX}ld
STRIP 	= ${TOOLPREFIX}strip
OBJDUMP = ${TOOLPREFIX}objdump

ASFLAGS	= -g -march=rv32i -mabi=ilp32

CFLAGS += -nostdlib -Ih
CFLAGS += -march=rv32i -mabi=ilp32
CFLAGS += -O3

LDSCRIPT = arppm.ld
LDFLAGS  = -m elf32lriscv -T ${LDSCRIPT}

OBJECTS =

SOURCES_ASM = $(shell find . -name "*.S" -printf "%P ")
OBJECTS += $(addprefix ${DIR_BUILD}/,${SOURCES_ASM:.S=.o})
vpath %.S $(sort $(dir ${SOURCES_ASM}))

SOURCES_C = $(shell find . -name "*.c" -printf "%P ")
OBJECTS += $(addprefix ${DIR_BUILD}/,${SOURCES_C:.c=.o})
vpath %.c $(sort $(dir ${SOURCES_C}))

all: ${ARPPM_IMG}

${ARPPM_IMG}: ${OBJECTS} ${LDSCRIPT} | ${DIR_BUILD}
	${LD} ${LDFLAGS} -o ${@} ${OBJECTS}
	${OBJDUMP} -S ${ARPPM_IMG} > ${ARPPM_ASM}

${DIR_BUILD}/%.o: %.s Makefile | ${DIR_BUILD}
	@mkdir -p $(dir ${@})
	${AS} ${ASFLAGS} -o ${@} ${<}

${DIR_BUILD}/%.o: %.c Makefile | ${DIR_BUILD}
	@mkdir -p $(dir ${@})
	${CC} -c ${CFLAGS} -Wa,-a,-ad,-alms=${DIR_BUILD}/${<:.c=lst} -o ${@} ${<}

${DIR_BUILD}:
	mkdir ${@}

strip:
	@${STRIP} --strip-unneeded ${ARPPM_IMG}
	@${STRIP} -R .riscv.attributes ${ARPPM_IMG}
#	${STRIP} -R .sbss ${ARPPM_IMG}
	@${STRIP} -R .comment ${ARPPM_IMG}

textdump: all
	${OBJDUMP} -D -S ${ARPPM_IMG} --prefix-addresses --show-raw-insn > dump.asm

mifdump: strip
	rm -f ${ARPPM_MIFDUMP}
	@${OBJDUMP} -D -S ${ARPPM_IMG} --prefix-addresses --show-raw-insn | grep 0x | cut -d ' ' -f 1,2 | sed 's/0x//g' | sed 's/ / : /g' >> ${ARPPM_MIFDUMP}
	
mif: mifdump
	@python3 mifgen/mifgen.py ${ARPPM_MIFDUMP} ${ARPPM_IMG}.mif

clean:
	rm -f ${ARPPM_IMG} ${ARPPM_ASM}
	rm -fr ${DIR_BUILD}
	rm -f *.asm
	rm -f *.mif
	rm -f *.mifdump
	rm -f ${DIR_SRC}/*.s

.PRECIOUS: %.o

-include $(wildcard ${DIR_BUILD}/*.d)
