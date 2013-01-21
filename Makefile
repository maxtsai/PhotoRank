
CC=clang
FLAGS=-O2 -Wall -std=c89 -I./depend -I./functions -I./core

## Test Build
test: test_jpeglib test_function
	@test -d ./tmp || mkdir ./tmp
	depend/test_jpeglib
	functions/test_function

test_function: functions/test.o functions/sobel.o depend/jpeg.o core/core.o
	@$(CC) $(FLAGS) -g -o functions/test_function functions/test.o\
		functions/sobel.o\
		depend/jpeg.o\
		core/core.o\
		-ljpeg -lm

functions/test.o: functions/test.c
	@$(CC) $(FLAGS) -g -c functions/test.c -o functions/test.o

functions/sobel.o: functions/sobel.c
	@$(CC) $(FLAGS) -c functions/sobel.c -o functions/sobel.o

test_jpeglib: depend/test.o depend/jpeg.o core/core.o
	@$(CC) $(FLAGS) -g -o depend/test_jpeglib depend/test.o\
		depend/jpeg.o core/core.o -ljpeg

depend/test.o: depend/test.c core/core.o
	@$(CC) $(FLAGS) -g -c depend/test.c -o depend/test.o

depend/jpeg.o: depend/jpeg.c
	@$(CC) $(FLAGS) -c depend/jpeg.c -o depend/jpeg.o

core/core.o: core/core.c
	@$(CC) $(FLAGS) -c core/core.c -o core/core.o

clean:
	rm -rf depend/*.o
	rm -rf functions/*.o
	rm -rf tmp
	rm -rf depend/test_jpeglib
	rm -rf functions/test_function
