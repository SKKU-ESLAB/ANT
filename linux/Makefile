INC=./inc
SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin

DEV_INC=./device/inc
DEV_SRC=./device/src
DEV_OBJ_DIR=./device/obj

CC=g++
FLAG=-std=c++11 -DLOG_LEVEL=0 -g
LIB=-lbluetooth -lpthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< -I$(INC) $(FLAG) $(LIB)

_OBJ=API.o Core.o Device.o NetworkSwitcher.o P2PServer.o ProtocolManager.o SegmentManager.o ServerAdapter.o ServerSocket.o
OBJ=$(patsubst %, $(OBJ_DIR)/%, $(_OBJ)) 

$(DEV_OBJ_DIR)/%.o: $(DEV_SRC)/%.cpp
	$(CC) -o $@ -c $< -I$(INC) -I$(DEV_INC) $(FLAG) $(LIB) 
_DEV_OBJ=BtDevice.o BtP2PServer.o RfcommServerSocket.o TcpServerSocket.o Util.o WfdDevice.o WfdP2PServer.o EthServerAdapter.o
DEV_OBJ=$(patsubst %, $(DEV_OBJ_DIR)/%, $(_DEV_OBJ)) 

all : exp tests

exp: trace_runner file_sender

tests: bt_only_test bt_control_wfd_data_test wpa_cli_test udhcpd_test

trace_runner: exp/trace_runner.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(FLAG) $(LIB) -I$(DEV_INC)

file_sender: exp/file_sender.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(FLAG) $(LIB) -I$(DEV_INC)

bt_only_test: tests/bt_only.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(FLAG) $(LIB) -I$(DEV_INC)

bt_control_wfd_data_test: tests/bt_control_wfd_data.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(FLAG) $(LIB) -I$(DEV_INC)

wpa_cli_test: tests/wpa_cli.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(FLAG) $(LIB) -I$(DEV_INC)

udhcpd_test: tests/udhcpd.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(FLAG) $(LIB) -I$(DEV_INC)

clean:
	rm ./obj/*.o ./bin/* ./device/obj/*.o
