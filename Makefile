INC=./inc
SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin

DEV_INC=./device/inc
DEV_SRC=./device/src
DEV_OBJ_DIR=./device/obj

CC=g++
FLAG=-std=c++11 -DLOG_LEVEL=0
LIB=-lbluetooth -lpthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< -I$(INC) $(FLAG) $(LIB)

_OBJ=Communicator.o Device.o NetworkSwitcher.o P2PServer.o ProtocolManager.o SegmentManager.o ServerAdapter.o ServerSocket.o
OBJ=$(patsubst %, $(OBJ_DIR)/%, $(_OBJ)) 

$(DEV_OBJ_DIR)/%.o: $(DEV_SRC)/%.cpp
	$(CC) -o $@ -c $< -I$(INC) -I$(DEV_INC) $(FLAG) $(LIB) 
_DEV_OBJ=BtDevice.o BtP2PServer.o RfcommServerSocket.o TcpServerSocket.o Util.o WfdDevice.o WfdP2PServer.o EthServerAdapter.o
DEV_OBJ=$(patsubst %, $(DEV_OBJ_DIR)/%, $(_DEV_OBJ)) 

all : file_based_test trace_runner

trace_runner: tests/trace_runner.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(FLAG) $(LIB) -I$(DEV_INC)

file_based_test: tests/file_based_test.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(FLAG) $(LIB) -I$(DEV_INC)

clean:
	rm ./obj/*.o ./bin/* ./device/obj/*.o
