flags = -Werror -Wall

all:
	mkdir -p build
	gcc $(flags) ./src/frontend.c ./src/core.c -o ./build/rv

clean:
	rm ./build/rv

tests: all
	cd ./tests && make clean && make

install: all
	sudo cp ./build/rv /usr/bin/

