
CC=clang
FLAGS=-O3 -Wall -std=c89 -I./depend -I./functions -I./core

## Test Build
test: test_jpeglib test_function
	@test -d ./tmp || mkdir ./tmp
	@echo "\n"
	#depend/test_jpeglib
	functions/test_function

test_function: functions/test.o\
	functions/sobel.o\
	depend/jpeg.o\
	core/core.o\
	functions/fast-canny.o\
	functions/blurdetect.o
	@$(CC) $(FLAGS) -g -o functions/test_function functions/test.o\
		functions/fast-canny.o\
		functions/blurdetect.o\
		functions/sobel.o\
		depend/jpeg.o\
		core/core.o\
		-ljpeg -lm -lpthread

functions/test.o: functions/test.c
	@$(CC) $(FLAGS) -g -c functions/test.c -o functions/test.o

functions/blurdetect.o: functions/blurdetect.c
	@$(CC) $(FLAGS) -g -c functions/blurdetect.c -o functions/blurdetect.o

functions/sobel.o: functions/sobel.c
	@$(CC) $(FLAGS) -c functions/sobel.c -o functions/sobel.o

functions/fast-canny.o: functions/fast-canny.c
	@$(CC) $(FLAGS) -c functions/fast-canny.c -o functions/fast-canny.o

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
	rm -rf core/*.o
	rm -rf tmp
	rm -rf depend/test_jpeglib
	rm -rf functions/test_function
