
CC=gcc
FLAGS=-g3


test: test_jpeglib
	depend/test_jpeglib

test_jpeglib: depend/test.o depend/jpeglib.o
	$(CC) $(FLAGS) -o depend/test_jpeglib depend/test.o depend/jpeglib.o -ljpeg
depend/test.o: depend/test.c
	$(CC) $(FLAGS) -c depend/test.c -o depend/test.o
depend/jpeglib.o: depend/jpeglib.c
	$(CC) $(FLAGS) -c depend/jpeglib.c -o depend/jpeglib.o


clean:
	rm -rf depend/*.o
	rm -rf functions/*.o
	rm -rf tmp/*
	rm -rf depend/test_jpeglib
