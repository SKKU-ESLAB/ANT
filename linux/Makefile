SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin

DEV_SRC=./device/src
DEV_OBJ_DIR=./device/obj

CC=g++
FLAG=-std=c++11 -DLOG_LEVEL=0 -g
LIB=-lbluetooth -lpthread
MKDIR_P=mkdir -p

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(FLAG) $(LIB)

_OBJ=API.o Core.o Device.o NetworkSwitcher.o NetworkMonitor.o P2PServer.o ProtocolManager.o SegmentManager.o ServerAdapter.o ServerSocket.o
OBJ=$(patsubst %, $(OBJ_DIR)/%, $(_OBJ)) 

$(DEV_OBJ_DIR)/%.o: $(DEV_SRC)/%.cpp
	$(CC) -o $@ -c $< $(FLAG) $(LIB) 
_DEV_OBJ=BtDevice.o BtP2PServer.o RfcommServerSocket.o TcpServerSocket.o Util.o WfdDevice.o WfdP2PServer.o EthServerAdapter.o CommInitializer.o
DEV_OBJ=$(patsubst %, $(DEV_OBJ_DIR)/%, $(_DEV_OBJ)) 

all : outdir exp tests

outdir :
	$(MKDIR_P) $(OBJ_DIR) $(BIN_DIR) $(DEV_OBJ_DIR)

exp: trace_runner

tests: bt_only_test bt_control_wfd_data_test comm_init_test wpa_cli_test udhcpd_test

trace_runner: exp/trace_runner.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

bt_only_test: tests/bt_only.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

bt_control_wfd_data_test: tests/bt_control_wfd_data.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

wpa_cli_test: tests/wpa_cli.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

udhcpd_test: tests/udhcpd.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

comm_init_test: tests/comm_init.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

clean:
	rm -rf ./obj/*.o ./bin/* ./device/obj/*.o || true