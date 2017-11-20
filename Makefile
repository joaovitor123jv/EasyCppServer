CC=g++

MAIN_FILE=main.cpp

RUN_PARAMS=

STD=-std=c++11

LINKERS=-lpthread

MAKE_EXEC=-o

GENERATE_OBJ=-c

EXE_NAME=server

SOURCES_FOLDER=src

all: $(EXE_NAME)
	@echo "Build with success"

build: $(EXE_NAME)
	@echo "End of building"

run: $(EXE_NAME)
	./$(EXE_NAME) $(RUN_PARAMS)


Connection.o:
	@echo "Generating Connection.o"
	@$(CC) $(GENERATE_OBJ) $(SOURCES_FOLDER)/Connection.cpp

Server.o: Connection.o
	@echo "Generating Server.o"
	@$(CC) $(GENERATE_OBJ) $(SOURCES_FOLDER)/Server.cpp

$(EXE_NAME): CLEAN_BEFORE_BUILD Server.o Connection.o
	@echo "Building main file $(MAIN_FILE)"
	@$(CC) $(MAIN_FILE) $(MAKE_EXEC) $(EXE_NAME) $(LINKERS) -I $(SOURCES_FOLDER)


clean:
	@echo "Removing Server.o"
	@rm -f Server.o
	@echo "Removing Connection.o"
	@rm -f Connection.o

CLEAN_BEFORE_BUILD:
	@echo "Removing file from before building"
	@rm -f $(EXE_NAME)
