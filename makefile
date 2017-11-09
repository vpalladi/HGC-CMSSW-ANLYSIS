
# compiler
CXX = g++

####################
### useful paths ###
####################

# top and current dirs
#CURRENT_DIR = $(shell pwd)

# inc and src dirs 
INC_DIR = ./inc
SRC_DIR = ./src

# obj and bin dirs
OBJ_DIR = ./obj
BIN_DIR = ./bin

##########################
### flags and libs and INC
##########################

# root
ROOT_CFLAGS = $(shell root-config --cflags)
#ROOT_LIBS  = $(shell root-config --libs)
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
#CXXFLAGS += -I$(INC_DIR) 

# CXX LIBS
CXXLIBS =  $(ROOT_GLIBS)
#CXXLIBS += $(ROOT_LIBS)
#CXXLIBS += $(HEPMC_LIBS)
#CXXLIBS += $(BOOST_LIBS)

# all my files .ccp .cc and .hh
MAINS_CPP := $(wildcard ./*.cpp)
EXE := $(MAINS_CPP:.cpp=.exe)

SOURCES_CC  := $(wildcard $(SRC_DIR)/*.cc)
OBJ         := $(SOURCES_CC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

INCLUDES_HH := $(wildcard $(INC_DIR)/*.h)
DICT_H = $(INC_DIR)/HGCC2D.h 

all: $(EXE) 

$(EXE): %.exe: %.cpp $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o bin/$(notdir $@) DictHGCC2D.cxx DictHGCTC.cxx $< $(CXXFLAGS) $(CXXLIBS) -I$(INC_DIR) $(OBJ) 

$(OBJ): $(OBJ_DIR)%.o: $(SRC_DIR)%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)

init:
	@mkdir -p $(INC_DIR)
	@mkdir -p $(SRC_DIR)

dict:
	rootcint -f DictHGCTC.cxx -c inc/HGCTC.h HGCTClinkDef.h
	rootcint -f DictHGCC2D.cxx -c inc/HGCC2D.h HGCC2DlinkDef.h

echo:
#	echo $(HEPMCFLAGS)
	echo $(EXE)
#	echo $(OBJ)
	echo $(SOURCES_CC)
#	echo $(INCLUDES_HH)
#	echo $(CURRENT_DIR)
##	echo $(BOOST_DIR)
#	echo $(INC_DIR)
#	echo $(SRC_DIR)
#	echo $(SOURCES_CC)
	echo $(OBJ)
#	echo $(MAINS_CPP)

