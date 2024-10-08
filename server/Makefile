# Makefile for building the server project

# Variables
ENGINE_DIR := ./src/engine
BUILD_DIR := ./build/engine/build/Release
SRC_DIR := ./src
ENGINE_FILES := $(wildcard $(ENGINE_DIR)/src/*.cpp $(ENGINE_DIR)/src/*.c $(ENGINE_DIR)src/*.h)
OUTPUT_NODE := ./src/engine/build/Release/engine.node

.PHONY: all build build-engine build-tsc start

all: build

# Build both engine and TypeScript
build: build-engine build-tsc

# Build engine using node-gyp
$(OUTPUT_NODE): $(ENGINE_FILES)
	@echo "Rebuilding engine because one or more of these files changed: $?"	
	cd $(ENGINE_DIR) && \
	node-gyp configure && \
	node-gyp build

build-engine: $(OUTPUT_NODE)

# Build TypeScript and copy engine output
build-tsc:
	cd $(SRC_DIR) && \
	tsc
	mkdir -p $(BUILD_DIR)
	cp $(ENGINE_DIR)/build/Release/engine.node $(BUILD_DIR)/engine.node

# Start the server
start:
	node ./build/index.js
