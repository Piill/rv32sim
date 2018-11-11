flags = -Werror -Wall

all:
	gcc $(flags) ./frontend.c ./core.c -o ./build/rv

clean:
	rm ./build/rv

tests: all
	cd ./tests && make


