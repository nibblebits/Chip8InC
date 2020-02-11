INCLUDES = -I ./include -I /usr/include
OBJECT_FILES = ./build/chip8.o ./build/screen.o ./build/registers.o ./build/simulator.o ./build/keyboard.o ./build/cmemory.o
FLAGS = -g -fPIC
all: ${OBJECT_FILES}
	gcc ${INCLUDES} ${OBJECT_FILES} -shared  -o ./bin/libchip8.so

./build/chip8.o: ./src/chip8.c
	gcc ${FLAGS} -c ${INCLUDES} ./src/chip8.c -o ./build/chip8.o


./build/screen.o: ./src/screen.c
	gcc ${FLAGS} -c ${INCLUDES} ./src/screen.c -o ./build/screen.o

./build/registers.o: ./src/registers.c
	gcc ${FLAGS} -c ${INCLUDES} ./src/registers.c -o ./build/registers.o

./build/simulator.o: ./src/simulator.c
	gcc ${FLAGS} -c ${INCLUDES} ./src/simulator.c -o ./build/simulator.o

./build/keyboard.o: ./src/keyboard.c
	gcc ${FLAGS} -c ${INCLUDES} ./src/keyboard.c -o ./build/keyboard.o


./build/cmemory.o: ./src/cmemory.c
	gcc ${FLAGS} -c ${INCLUDES} ./src/cmemory.c -o ./build/cmemory.o
clean:
	rm ${OBJECT_FILES}