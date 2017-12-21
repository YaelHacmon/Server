#create executalbe file by linking the object files
server.out: compileAll
			g++ *.o
			rm -f *.o

#creating all object files from source code
compileAll: *.h *.cpp
			g++ -c *.cpp