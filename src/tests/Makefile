sources:= $(wildcard ./task1/*.s)
o:=$(patsubst %.s, %.o, $(sources))
out:=$(patsubst %.s, %.bin, $(sources))
run:=$(patsubst %.s, %.out, $(sources))
asm=riscv32-unknown-elf-gcc

all: $(o) $(out) $(run)
	./test.sh

clean:
	rm $(out)
	rm $(run)

%.o: %.s
	 $(asm) -c $< -o $@

%.bin: %.o
	riscv32-unknown-elf-objcopy $< --dump-section .text=$@


%.out: %.bin
	../build/rv -f $< -o $@


