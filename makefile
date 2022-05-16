CC = g++
CFLAGS = -I. -std=c++11
DEPS = token.hpp scanner.hpp parser.hpp semantics.hpp
OBJ = token.o scanner.o parser.o semantics.o main.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

compfs: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o compfs *.asm
