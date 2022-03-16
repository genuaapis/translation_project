CC = g++
CFLAGS = -I. -std=c++11
DEPS = token.hpp scanner.hpp test_scanner.hpp
OBJ = token.o scanner.o test_scanner.o p1_main.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

scanner: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o scanner
