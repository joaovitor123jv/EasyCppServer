#Define the settings for your project here, these settings must be used till the end of project

#Compiler
CC=g++

MAIN_FILE=Main.cpp

MAIN_EXE_NAME=server

MAIN_EXE_DEPENDENCIES=Server.o Connection.o

FRAMEWORK_LIB_LINKERS=-Isrc/lib/

MAIN_EXE_LINKERS=-lpthread $(FRAMEWORK_LIB_LINKERS)

WARNINGS=-Wall
#WARNINGS=

OPTIMIZATION=
#OPTIMIZATION=-03

STD=
#STD=-std=c++11

.PHONY: clean all debug run
#default operations definitions
all: clean $(MAIN_EXE_NAME)

debug: all /usr/bin/valgrind
	/usr/bin/valgrind ./$(MAIN_EXE_NAME)

run: $(MAIN_EXE_NAME)
	@echo "Start running $^"
	@./$^

clean:
	@echo "Starting project cleaning"
	@echo "Removing main object file"
	@rm -f $(MAIN_FILE:.cpp=.o)
	@echo "Removing $(MAIN_EXE_NAME)"
	@rm -f $(MAIN_EXE_NAME)
	@echo "Removing Server.o"
	@rm -f Server.o
	@echo "Removing Connection.o"
	@rm -f Connection.o

#Main File Compile Section
$(MAIN_FILE): Server.o

$(MAIN_EXE_NAME): $(MAIN_FILE) $(MAIN_EXE_DEPENDENCIES)
	@echo "Generating $(MAIN_EXE_NAME) on ./bin directory"
	$(CC) $(STD) $(WARNINGS) $(OPTIMIZATION) $^ -o $@ $(MAIN_EXE_LINKERS) 
	#$(MAIN_EXE_DEPENDENCIES)



Server.o: src/lib/Server.cpp
	$(CC) $(STD) $(WARNINGS) $(OPTIMIZATION) -c $^ -o $@ $(MAIN_EXE_LINKERS)

Connection.o: src/lib/Connection.cpp
	$(CC) $(STD) $(WARNINGS) $(OPTIMIZATION) -c $^ -o $@ $(MAIN_EXE_LINKERS)