CC = g++
CFLAGS = -I. -std=c++11
DEPS = token.hpp scanner.hpp parser.hpp semantics.hpp
OBJ = token.o scanner.o parser.o semantics.o p3_main.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

stat-sem: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o stat-sem *.txt
