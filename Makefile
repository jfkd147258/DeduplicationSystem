CC=g++
CFLAGS=-Iinclude -std=c++17
LDFLAGS=-lssl -lcrypto
DEPS = include/sha256.h include/rabin_karp.h
OBJ = src/sha256.o src/rabin_karp.o src/main.o 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

DeduplicationSystem: $(OBJ)
	$(CC) -o bin/$@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) bin/DeduplicationSystem

run: DeduplicationSystem
	bin/DeduplicationSystem

