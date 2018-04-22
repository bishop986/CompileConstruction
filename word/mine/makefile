CC=g++
FLAG=-std=c++11

WordAnalysis:Mid.o
	$(CC) $(FLAG) -o WordAnalysis Mid.o
Mid.o:scanner.cc
	$(CC) $(FLAG) -o Mid.o -c scanner.cc
clean:
	rm *.o
