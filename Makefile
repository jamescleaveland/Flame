CC=c++
CFLAGS=-stdlib=libc++ -std=c++11 -emit-llvm -o bin/Flame -L/usr/local/lib -lpng16
SOURCES=main.cpp Image.cpp ImageGen.cpp Plot.cpp ToneMap.cpp ImageGenConfigurator.cpp Transforms.cpp

all:
	$(CC) $(SOURCES) $(CFLAGS)

run:
	$(CC) $(SOURCES) $(CFLAGS)
	bin/Flame 1440 900 10 "dbl half a2 | handkerchief spherical swirl tangent half" -.4 0
	open test.png
