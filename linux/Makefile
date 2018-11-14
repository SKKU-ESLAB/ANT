BIN_DIR=./bin

CORE_SRC_DIR=./core/src
CORE_OBJ_DIR=./core/obj

COMMON_SRC_DIR=./common/src
COMMON_OBJ_DIR=./common/obj

DEV_SRC_DIR=./device/src
DEV_OBJ_DIR=./device/obj

CC=g++
FLAG=-std=c++11 -DLOG_LEVEL=0 -g
LIB=-lbluetooth -lpthread
MKDIR_P=mkdir -p

$(CORE_OBJ_DIR)/%.o: $(CORE_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(FLAG) $(LIB)

_CORE_OBJ=API.o Core.o Device.o NetworkEstimator.o NetworkSwitcher.o NetworkMonitor.o P2PServer.o ProtocolManager.o SegmentManager.o ServerAdapter.o ServerSocket.o ControlMessageReceiver.o ControlMessageSender.o SenderThread.o ReceiverThread.o
CORE_OBJ=$(patsubst %, $(CORE_OBJ_DIR)/%, $(_CORE_OBJ))

$(COMMON_OBJ_DIR)/%.o: $(COMMON_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(FLAG) $(LIB)

_COMMON_OBJ=ChildProcess.o DebugLog.o
COMMON_OBJ=$(patsubst %, $(COMMON_OBJ_DIR)/%, $(_COMMON_OBJ))

$(DEV_OBJ_DIR)/%.o: $(DEV_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(FLAG) $(LIB) 
_DEV_OBJ=BtServerAdapter.o BtDevice.o BtP2PServer.o RfcommServerSocket.o WfdServerAdapter.o TcpServerSocket.o WfdDevice.o WfdP2PServer.o EthServerAdapter.o NetworkInitializer.o
DEV_OBJ=$(patsubst %, $(DEV_OBJ_DIR)/%, $(_DEV_OBJ))

all : outdir exp tests

outdir :
	$(MKDIR_P) $(BIN_DIR) $(CORE_OBJ_DIR) $(COMMON_OBJ_DIR) $(DEV_OBJ_DIR)

exp: trace_runner

tests: comm_init_test wpa_cli_test udhcpd_test

trace_runner: exp/trace_runner.cpp exp/TraceRunner.cpp $(CORE_OBJ) $(COMMON_OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

wpa_cli_test: tests/wpa_cli.cpp $(CORE_OBJ) $(COMMON_OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

udhcpd_test: tests/udhcpd.cpp $(CORE_OBJ) $(COMMON_OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

comm_init_test: tests/comm_init.cpp $(CORE_OBJ) $(COMMON_OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAG) $(LIB) 

clean:
	rm -rf ./obj/*.o ./bin/* ./device/obj/*.o || true