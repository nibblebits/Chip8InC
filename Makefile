INCLUDES = -I ./include -I /usr/include
OBJECT_FILES = ./build/emulator.o ./build/screen.o ./build/registers.o

all: ${OBJECT_FILES}
	gcc ${INCLUDES} ${OBJECT_FILES} main.c -o main -lSDL2main -lSDL2_image -lSDL2


./build/emulator.o: ./src/emulator.c
	gcc -c ${INCLUDES} ./src/emulator.c -o ./build/emulator.o

./build/screen.o: ./src/screen.c
	gcc -c ${INCLUDES} ./src/screen.c -o ./build/screen.o

./build/registers.o: ./src/registers.c
	gcc -c ${INCLUDES} ./src/registers.c -o ./build/registers.o


clean:
	rm ${OBJECT_FILES}