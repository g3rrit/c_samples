TARGET ?= a.out

BUILD_DIR ?= ./build

CC ?= gcc

SRC_DIR ?= ./src

#unittest
SRC_UNI_DIR ?= $(SRC_DIR)/unittest

#container
SRC_CON_DIR ?= $(SRC_DIR)/container
SRCF_CON := $(shell find $(SRC_CON_DIR) -name *.c)

container:
	$(CC) -I$(SRC_UNI_DIR) $(SRCF_CON) -o $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)



	
