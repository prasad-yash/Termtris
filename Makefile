BUILD_DIR=bin/debug
.PHONY: build run all
.DEFAULT_GOAL:=all


all: run

dirs:
	mkdir -p $(BUILD_DIR)

build: dirs
	g++ main.cpp -lncurses -Wall -o $(BUILD_DIR)/main

run: build
	$(BUILD_DIR)/main