CC = g++
CFLAGS = -I. -std=c++11
DEPS = token.hpp scanner.hpp parser.hpp test_parser.hpp
OBJ = token.o scanner.o parser.o test_parser.o p2_main.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

parser: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o parser *.txt
