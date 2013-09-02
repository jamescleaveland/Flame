CC=c++
CFLAGS=-stdlib=libc++ -std=c++11 -emit-llvm -o bin/Flame -L/usr/local/lib -lpng16
SOURCES=main.cpp Image.cpp ImageGen.cpp Plot.cpp ToneMap.cpp ImageGenConfigurator.cpp Transforms.cpp

all:
	$(CC) $(SOURCES) $(CFLAGS)

run:
	$(CC) $(SOURCES) $(CFLAGS)
	bin/Flame 1024 1024 1.0 "half a2 a3 | spherical swirl" 0 0
	open test.png
