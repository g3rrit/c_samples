TARGET ?= a.out

BUILD_DIR ?= ./build

CC := gcc

SRC_DIR ?= ./src

#unittest
SRC_UNI_DIR ?= $(SRC_DIR)/unittest

#testfiles
TEST_DIR ?= $(SRC_DIR)/test

#container
CON_TEST_F ?= $(TEST_DIR)/container_test.c

SRC_CON_DIR ?= $(SRC_DIR)/container
SRCF_CON := $(shell find $(SRC_CON_DIR) -name *.c)

container:
	$(CC) -I$(SRC_UNI_DIR) -I$(SRC_CON_DIR) $(SRCF_CON) $(CON_TEST_F) -o $(BUILD_DIR)/$(TARGET) -w
	$(BUILD_DIR)/$(TARGET)

#event
EVE_TEST_F ?= $(TEST_DIR)/event_test.c

SRC_EVE_DIR ?= $(SRC_DIR)/event
SRCF_EVE := $(shell find $(SRC_EVE_DIR) -name *.c)

event:
	$(CC) -I$(SRC_UNI_DIR) -I$(SRC_EVE_DIR) -I$(SRC_CON_DIR) $(SRCF_EVE) $(SRCF_CON) $(EVE_TEST_F) -o $(BUILD_DIR)/$(TARGET) -w
	$(BUILD_DIR)/$(TARGET)

#fft
SRCF_FFT := $(SRC_DIR)/fft/fft.c
SRCFM_FFT := $(SRC_DIR)/fft/main.c

fft:
	$(CC) $(SRCF_FFT) $(SRCFM_FFT) -lm -o $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)
