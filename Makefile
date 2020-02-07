INCLUDES = -I ./include -I /usr/include
OBJECT_FILES = ./build/emulator.o

all: ${OBJECT_FILES}
	gcc ${INCLUDES} ${OBJECT_FILES} main.c -o main -lSDL2main -lSDL2_image -lSDL2


./build/emulator.o: emulator.c
	gcc -c ${INCLUDES} emulator.c -o ./build/emulator.o

clean:
	rm ${OBJECT_FILES}