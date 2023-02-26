# ARP - ARP Pac-Man
A minituarized version of Pac-Man game for a RISC-V based embedded system. Made for [ARP](https://github.com/kiclu/ARP), an FPGA project which aims to implement a small RISC-V computer on an Altera DE0 development board.
## Instructions
### Prerequesites
To build this application, you will need riscv32-gnu-toolchain from [here](https://github.com/riscv-collab/riscv-gnu-toolchain) by running\
`git clone https://github.com/riscv/riscv-gnu-toolchain`\
Sadly it doesn't come precompiled so you will have to wait for it to build and install,
expect it to take at least half an hour.

While waiting for the gnu toolchain to finish downloading, make sure to get all the
prerequesites for installing it beforehand. For Debian based systems you can run the following command:\
`sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build`

After the toolchain has finished downloading and all the prerequesites are installed, 
toolchain installer needs to be configured for rv32i architecture. To do that run the following command
inside the riscv32-gnu-toolchain directory:\
`./configure --prefix=/usr/riscv32-gnu-toolchain --with-arch=rv32i --with-abi=ilp32 --disable-linux`\
After the installer is done configuring you can start it by running the `make` command. The installer
will now start doing its job. Be prepared to wait a bit because this is the longest part.

### Building
After risc32-gnu-toolchain is done installing you should be able to run `make all` inside this directory
without any errors. If the compilation was successful, two files should appear, `arppm` and `arppm.asm`.
`arppm` is the output file from linker while the 'arppm.asm' is a decompiled version of previous file
used for debugging purposes only.

To finally prepare the project for conversion into Intel's .mif file format, you need to strip the
binary and convert into address - value pairs. To do this run `make mifdump`. This will
create a `arppm.mifdump` file which is now ready to be passed to the mifgen.

## TODO
- [ ] finish mifgen
- [ ] make a test program for ARP

## Links
[riscv32-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain)\
[arpmifgen](https://github.com/kiclu/arpmifgen)\
[RISC-V from scratch](https://twilco.github.io/)
