INCLUDE = ../
SRC = ../
CCFLAGS = -c -I $(INCLUDE)

#create executalbe file by linking the object files
a.out: Server.o main.o
	g++ Server.o main.o

#creating all object files from source code

Server.o: $(SRC)Server.cpp $(INCLUDE)Server.h
	g++ $(CCFLAGS) $(SRC)Server.cpp
	
main.o: $(SRC)main.cpp $(INCLUDE)Server.h
	g++ $(CCFLAGS) $(SRC)main.cpp