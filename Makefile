parser: parser.y scanner.l ast.hh ast.cpp
		bison -vd parser.y
		flex scanner.l

		g++ -std=c++11 -o parser parser.tab.c lex.yy.c ast.cpp -ll -g -ldl -lfl `llvm-config --cppflags --libs --libfiles core --ldflags --system-libs`
		# `llvm-config --cppflags` `llvm-config --ldflags` `llvm-config --libs`
		

clean:
	rm -rf lex.yy.c parser.tab.c parser.tab.h parser