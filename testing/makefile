INCLUDE = ../include/
SRC = ../src/
CCFLAGS = -Wall -c -pthread -I $(INCLUDE) -ggdb

#create executalbe file by linking the object files
a.out: GameInfo.o Utility.o GamesInfoLists.o CloseCommand.o JoinCommand.o ListGamesCommand.o PlayCommand.o StartGameCommand.o CommandsManager.o ClientHandler.o Task.o ThreadPool.o Server.o main.o
	g++ -pthread -Wall GameInfo.o Utility.o GamesInfoLists.o CloseCommand.o JoinCommand.o ListGamesCommand.o PlayCommand.o StartGameCommand.o CommandsManager.o ClientHandler.o Task.o ThreadPool.o Server.o main.o

#creating all object files from source code
Utility.o: $(SRC)Utility.cpp $(INCLUDE)Utility.h
	g++ $(CCFLAGS) $(SRC)Utility.cpp

GameInfo.o: $(SRC)GameInfo.cpp $(INCLUDE)GameInfo.h
	g++ $(CCFLAGS) $(SRC)GameInfo.cpp
	
GamesInfoLists.o: $(SRC)GamesInfoLists.cpp $(INCLUDE)GamesInfoLists.h $(INCLUDE)GameInfo.h
	g++ $(CCFLAGS) $(SRC)GamesInfoLists.cpp

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

CommandsManager.o: $(SRC)CommandsManager.cpp $(INCLUDE)CommandsManager.h $(INCLUDE)Command.h $(INCLUDE)Server.h
	g++ $(CCFLAGS) $(SRC)CommandsManager.cpp
	
ClientHandler.o: $(SRC)ClientHandler.cpp $(INCLUDE)ClientHandler.h $(INCLUDE)CommandsManager.h
	g++ $(CCFLAGS) $(SRC)ClientHandler.cpp
	
Task.o: $(SRC)Task.cpp $(INCLUDE)Task.h
	g++ $(CCFLAGS) $(SRC)Task.cpp
	
ThreadPool.o: $(SRC)ThreadPool.cpp $(INCLUDE)ThreadPool.h $(INCLUDE)Task.h
	g++ $(CCFLAGS) $(SRC)ThreadPool.cpp
	
Server.o: $(SRC)Server.cpp $(INCLUDE)Server.h
	g++ $(CCFLAGS) $(SRC)Server.cpp
	
main.o: $(SRC)main.cpp $(INCLUDE)Server.h
	g++ $(CCFLAGS) $(SRC)main.cpp
