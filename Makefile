INC=./inc
SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin

DEV_INC=./device/inc
DEV_SRC=./device/src
DEV_OBJ_DIR=./device/obj

CC=g++
FLAG=-std=c++11 -DLOG_LEVEL=0
LIB=-lpthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< -I$(INC) $(FLAG)

_OBJ=communicator.o protocol_manager.o segment_manager.o network_manager.o network_adapter.o network_switcher.o
OBJ=$(patsubst %, $(OBJ_DIR)/%, $(_OBJ)) 

$(DEV_OBJ_DIR)/%.o: $(DEV_SRC)/%.cpp
	$(CC) -o $@ -c $< -I$(INC) -I$(DEV_INC) $(FLAG)
_DEV_OBJ=wifi_control.o tcp_server_over_eth.o tcp_server_over_wfd.o wifi_control.o rfcomm_over_bt.o
DEV_OBJ=$(patsubst %, $(DEV_OBJ_DIR)/%, $(_DEV_OBJ)) 

all : file_test_low

test: tests/communicator_test.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(LIB) $(FLAG) -I$(DEV_INC) -lbluetooth

file_test_low: tests/file_test_low.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(LIB) $(FLAG) -I$(DEV_INC) -lbluetooth

power_exp: tests/power.cpp $(OBJ) $(DEV_OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(LIB) $(FLAG) -I$(DEV_INC) -lbluetooth

wfd_test: tests/wfd_test.cpp device/src/wifi_control.cpp
	$(CC) -o $(BIN_DIR)/$@ $^ -I$(INC) $(LIB) $(FLAG) -I$(DEV_INC)

clean:
	rm ./obj/*.o ./bin/* ./device/obj/*.o
