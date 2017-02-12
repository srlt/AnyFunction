NAME := $(notdir $(lastword $(abspath .)))
BIN  := bin/$(NAME)
SRC  := src
HDR  := ../include

HDRS_C   := $(wildcard $(SRC)/*.h) $(wildcard $(HDR)/*.h)
HDRS_CPP := $(HDRS_C) $(wildcard $(SRC)/*.hpp) $(wildcard $(HDR)/*.hpp)
SRCS     := $(wildcard $(SRC)/*.S) $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*.cpp)
OBJS     := $(SRCS:%=%.o)

AS       := $(AS)
ASFLAGS  :=
CC       := clang
CCFLAGS  := -Wall -Wfatal-errors -O2 -std=c11 -I$(HDR)
CXX      := clang++
CXXFLAGS := -Wall -Wfatal-errors -O2 -std=c++14 -I$(HDR)
LD       := clang++
LDFLAGS  :=

.PHONY: build run clean

build: $(BIN)
run: $(BIN)
	@$(BIN)
clean:
	$(RM) $(OBJS) $(BIN)

%.S.o: %.S Makefile
	$(AS) $(ASFLAGS) -o $@ $<
%.c.o: %.c $(HDRS_C) Makefile
	$(CC) $(CCFLAGS) -c -o $@ $<
%.cpp.o: %.cpp $(HDRS_CPP) Makefile
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN): $(OBJS) Makefile
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
