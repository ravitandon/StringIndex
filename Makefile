all: StringIndex 

StringIndex: header.h tree.cpp utils.cpp stridx.cpp    
	g++ -o stridx header.h tree.cpp utils.cpp stridx.cpp  -lpthread
