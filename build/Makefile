all:
	riscv32-unknown-elf-as -c test.s -o test.o
	riscv32-unknown-elf-objcopy test.o --dump-section .text=test.bin
	./rv -i -f ./test.bin
