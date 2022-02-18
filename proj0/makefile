CC = g++
CFLAGS = -I. -std=c++11
DEPS = tree.hpp node.hpp
OBJ = tree.o main.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

P0: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o P0 *.*_order
