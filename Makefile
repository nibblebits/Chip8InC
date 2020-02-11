INCLUDES = -I ./include -I /usr/include -I ./chip8/include
OBJECT_FILES =  ./build/emulator.o ./build/test.o 
FLAGS = -g
all: ${OBJECT_FILES}
	gcc ${INCLUDES} ${OBJECT_FILES} main.c -o main -lSDL2main -lchip8 -lSDL2_image -lSDL2

./build/emulator.o: ./src/emulator.c
	gcc ${FLAGS} -c ${INCLUDES} ./src/emulator.c -o ./build/emulator.o

./build/test.o: ./src/test.c
	gcc ${FLAGS} -c ${INCLUDES} ./src/test.c -o ./build/test.o

clean:
	rm ${OBJECT_FILES}