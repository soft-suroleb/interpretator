CC=g++
FLAGS=-std=c++2a -Wall -Werror -Wextra -g

all: main

main: main.o lex.o Ident.o tabl_ident.o scanner.o parser.o poliz.o executer.o interpretator.o
	$(CC) $(FLAGS) -o main main.o lex.o Ident.o tabl_ident.o scanner.o parser.o poliz.o executer.o interpretator.o

main.o: main.cpp scanner.hpp
	$(CC) $(FLAGS) -c main.cpp

lex.o: lex.cpp type_of_lex.hpp
	$(CC) $(FLAGS) -c lex.cpp

Ident.o: Ident.cpp Ident.hpp type_of_lex.hpp
	$(CC) $(FLAGS) -c Ident.cpp

tabl_ident.o: tabl_ident.cpp tabl_ident.hpp Ident.hpp
	$(CC) $(FLAGS) -c tabl_ident.cpp

scanner.o: scanner.cpp scanner.hpp tabl_ident.hpp Ident.hpp type_of_lex.hpp
	$(CC) $(FLAGS) -c scanner.cpp

parser.o: parser.cpp parser.hpp
	$(CC) $(FLAGS) -c parser.cpp

poliz.o: poliz.cpp poliz.hpp
	$(CC) $(FLAGS) -c poliz.cpp
	
executer.o: executer.cpp executer.hpp
	$(CC) $(FLAGS) -c executer.cpp

interpretator.o: interpretator.cpp interpretator.hpp
	$(CC) $(FLAGS) -c interpretator.cpp

clean:
	rm -rf *.o