CC=c++
CFLAGS=-stdlib=libc++ -std=c++11 -emit-llvm -o Flame -L/usr/local/lib -lpng16
SOURCES=main.cpp Image.cpp ImageGen.cpp

all:
	$(CC) $(SOURCES) $(CFLAGS)

