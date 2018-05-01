brainfuck:brainfuck.o Pointer.o
	g++ -o brainfuck -std=c++11 brainfuck.o Pointer.o
brainfuck.o:brainfuck.cc
	g++ -o brainfuck.o -std=c++11 -c brainfuck.cc
Pointer.o:Pointer.cc
	g++ -o Pointer.o -std=c++11 -c Pointer.cc
clean:
	rm *.o
