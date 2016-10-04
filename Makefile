INC=./inc
SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin
CC=g++
FLAG=-std=c++11
LIB=-lpthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< -I$(INC) $(FLAG) -g

_OBJ=communicator.o protocol_manager.o\
segment_manager.o network_manager.o network_adapter.o
OBJ=$(patsubst %, $(OBJ_DIR)/%, $(_OBJ)) 

all : test

test: tests/communicator_test.cpp $(OBJ) 
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(LIB) $(FLAG) -I./device/inc -g
