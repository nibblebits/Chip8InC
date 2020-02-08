INCLUDES = -I ./include -I /usr/include
OBJECT_FILES = ./build/emulator.o ./build/screen.o ./build/registers.o ./build/simulator.o ./build/keyboard.o ./build/cmemory.o
FLAGS = -g
all: ${OBJECT_FILES}
	gcc ${INCLUDES} ${OBJECT_FILES} main.c -o main -lSDL2main -lSDL2_image -lSDL2


./build/emulator.o: ./src/emulator.c
	gcc ${FLAGS} -c ${INCLUDES} ./src/emulator.c -o ./build/emulator.o

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