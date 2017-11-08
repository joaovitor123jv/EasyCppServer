CC=g++

MAIN_FILE=main.cpp

RUN_PARAMS=

STD=-std=c++11

LINKERS=-lpthread

MAKE_EXEC=-o

GENERATE_OBJ=-c

EXE_NAME=server

SOURCES_FOLDER=src

all: clean $(EXE_NAME)
	@echo "Compilação terminada com sucesso"

build: $(EXE_NAME)
	@echo "Compilação terminada com sucesso"

run: $(EXE_NAME)
	./$(EXE_NAME) $(RUN_PARAMS)


Connection.o:
	@echo "Gerando Connection.o"
	@$(CC) $(GENERATE_OBJ) $(SOURCES_FOLDER)/Connection.cpp

Server.o: Connection.o
	@echo "Gerando Server.o"
	@$(CC) $(GENERATE_OBJ) $(SOURCES_FOLDER)/Server.cpp

$(EXE_NAME): Server.o Connection.o
	@echo "Compilando arquivo principal"
	@$(CC) $(MAIN_FILE) $(MAKE_EXEC) $(EXE_NAME) $(LINKERS)


clean:
	@echo "Removendo Server.o"
	@rm Server.o
	@echo "Removendo Connection.o"
	@rm Connection.o
	@echo "Removendo server"
	@rm server


