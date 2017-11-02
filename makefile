
# compiler
CXX = g++



####################
### useful paths ###
####################

# top and current dirs
CURRENT_DIR = $(shell pwd)


# obj, inc and src 
OBJ_DIR = $(CURRENT_DIR)/obj
INC_DIR = $(CURRENT_DIR)/inc
SRC_DIR = $(CURRENT_DIR)/src


# bin dir
BIN_DIR = $(CURRENT_DIR)/bin

##########################
### flags and libs and INC
##########################

# root
ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS  = $(shell root-config --libs)
ROOT_GLIBS = $(shell root-config --glibs)

# Boost
#BOOST_FLAGS = -I$(BOOST_DIR)/include
#BOOST_LIBS = -L$(BOOST_DIR)/lib -lboost_filesystem -lboost_system


# CXX flags
CXXFLAGS =  -Wall -std=c++11 -Wno-write-strings
CXXFLAGS += $(ROOT_CFLAGS)
#CXXFLAGS += $(wildcard pwd)
#CXXFLAGS += $(BOOST_FLAGS)
#CXXFLAGS += $(HEPMC_FLAGS)
CXXFLAGS += -I$(INC_DIR) 

# CXX LIBS
CXXLIBS =  $(ROOT_GLIBS)
#CXXLIBS += $(ROOT_LIBS)
#CXXLIBS += $(HEPMC_LIBS)
#CXXLIBS += $(BOOST_LIBS)

# all my files .ccp .cc and .hh
MAINS_CPP := $(wildcard ./*.cpp)
SOURCES_CC := $(wildcard ./src/*.cc)
INCLUDES_HH := $(wildcard ./inc/*.hh)

EXES := $(MAINS_CPP:.cpp=.exe)
OBJ  := $(SOURCES_CC:.cc=.o)
OBJ  := $(subst src,obj,${OBJ})

all: $(EXES)

%.exe: %.cpp obj/HGC.o
	@mkdir -p $(BIN_DIR)
	$(CXX) -o bin/$(notdir $@) $< $(CXXFLAGS) $(CXXLIBS) obj/HGC.o 


obj/HGC.o: src/HGC.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(CURRENT_DIR)/bin/*
	rm $(CURRENT_DIR)/obj/*
echo:
#	echo $(HEPMCFLAGS)
#	echo $(TOP)
	echo $(CURRENT_DIR)
#	echo $(BOOST_DIR)
	echo $(INC_DIR)
	echo $(SRC_DIR)
#	echo $(CXX) $(CXXFLAGS) -o bin/$@ $<
	echo $(OBJ)
	echo $(MAINS_CPP)

