sources:= $(wildcard ./task1/*.s)
sources+= $(wildcard ./task2/*.s)
sources+= $(wildcard ./instruction_tests/*.s)

o:= $(patsubst %.s, %.o, $(sources))
out:= $(patsubst %.s, %.bin, $(sources))
run:= $(patsubst %.s, %.out, $(sources))

asm=riscv32-unknown-elf-as

all: $(o) $(out) $(run) task3
	./test.sh

sim:
	cd .. & make

clean:
	- rm $(o)
	- rm $(out)
	- rm $(run)

%.o: %.s
	 $(asm) -R -c $< -o $@

%.bin: %.o
	riscv32-unknown-elf-objcopy $< --dump-section .text=$@


%.out: %.bin
	../build/rv -f $< -o $@ -s

.PHONY:task3
task3:
	 $(asm) -R -c ./task3/mult.s -o ./task3/mult.o
	riscv32-unknown-elf-objcopy ./task3/mult.o --dump-section .text=./task3/mult.bin
	../build/rv -f ./task3/loop.bin -o ./task3/loop.out
	../build/rv -f ./task3/mult.bin -o ./task3/mult.out
