CC=c++
CFLAGS=-stdlib=libc++ -std=c++11 -emit-llvm -o bin/Flame -L/usr/local/lib -lpng16
SOURCES=main.cpp Image.cpp ImageGen.cpp Plot.cpp ToneMap.cpp

all:
	$(CC) $(SOURCES) $(CFLAGS)

run:
	$(CC) $(SOURCES) $(CFLAGS)
	bin/Flame
