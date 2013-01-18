
CC=gcc
FLAGS=-O2 -Wall -I./inc

## Test Build
test: test_jpeglib functions/sobel.o
	@test -d ./tmp || mkdir ./tmp
	depend/test_jpeglib

functions/sobel.o: functions/sobel.c
	@$(CC) $(FLAGS) -c functions/sobel.c -o functions/sobel.o

test_jpeglib: depend/test.o depend/jpeglib.o
	@$(CC) $(FLAGS) -g -o depend/test_jpeglib depend/test.o depend/jpeglib.o -ljpeg

depend/test.o: depend/test.c
	@$(CC) $(FLAGS) -g -c depend/test.c -o depend/test.o

depend/jpeglib.o: depend/jpeglib.c
	@$(CC) $(FLAGS) -c depend/jpeglib.c -o depend/jpeglib.o

clean:
	rm -rf depend/*.o
	rm -rf functions/*.o
	rm -rf tmp
	rm -rf depend/test_jpeglib
