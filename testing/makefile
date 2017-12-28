INCLUDE = ../include/
SRC = ../src/
CCFLAGS = -c -pthread -I $(INCLUDE)

#create executalbe file by linking the object files
a.out: Server.o main.o
	g++ -pthread Server.o main.o

#creating all object files from source code
GameInfo.o: $(SRC)GameInfo.cpp $(INCLUDE)GameInfo.h
	g++ $(CCFLAGS) $(SRC)GameInfo.cpp
	
GamesInfoLists.o: $(SRC)GamesInfoLists.cpp $(INCLUDE)GamesInfoLists.h $(INCLUDE)GameInfo.h
	g++ $(CCFLAGS) $(SRC)GameInfo.cpp

CloseCommand.o: $(SRC)CloseCommand.cpp $(INCLUDE)CloseCommand.h $(INCLUDE)Command.h
	g++ $(CCFLAGS) $(SRC)CloseCommand.cpp
	
JoinCommand.o: $(SRC)JoinCommand.cpp $(INCLUDE)JoinCommand.h $(INCLUDE)Command.h
	g++ $(CCFLAGS) $(SRC)JoinCommand.cpp

ListGamesCommand.o: $(SRC)ListGamesCommand.cpp $(INCLUDE)ListGamesCommand.h $(INCLUDE)Command.h
	g++ $(CCFLAGS) $(SRC)ListGamesCommand.cpp
	
PlayCommand.o: $(SRC)PlayCommand.cpp $(INCLUDE)PlayCommand.h $(INCLUDE)Command.h
	g++ $(CCFLAGS) $(SRC)PlayCommand.cpp
	
StartGameCommand.o: $(SRC)StartGameCommand.cpp $(INCLUDE)StartGameCommand.h $(INCLUDE)Command.h
	g++ $(CCFLAGS) $(SRC)StartGameCommand.cpp

CommandManager.o: $(SRC)CommandManager.cpp $(INCLUDE)CommandManager.h $(INCLUDE)Command.h
	g++ $(CCFLAGS) $(SRC)CommandManager.cpp

Server.o: $(SRC)Server.cpp $(INCLUDE)Server.h
	g++ $(CCFLAGS) $(SRC)Server.cpp
	
main.o: $(SRC)main.cpp $(INCLUDE)Server.h
	g++ $(CCFLAGS) $(SRC)main.cpp