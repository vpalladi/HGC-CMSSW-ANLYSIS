
# THE compiler
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

# dictionary dir
DIC_DIR = ./dic


##########################
### flags and libs and INC
##########################

# root
ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_GLIBS = $(shell root-config --glibs)


# CXX flags
CXXFLAGS =  -Wall -std=c++11 -Wno-write-strings
CXXFLAGS += $(ROOT_CFLAGS)
CXXFLAGS += -I$(INC_DIR)

# CXX LIBS
CXXLIBS =  $(ROOT_GLIBS)


#############
### FILES ###
#############

# all .cpp (we will make exec from them)
MAINS_CPP := $(wildcard ./*.cpp)
EXE       := $(MAINS_CPP:.cpp=.exe)

# all sources .cc 
SOURCES_CC  := $(wildcard $(SRC_DIR)/*.cc)
OBJ         := $(SOURCES_CC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

# all the dictionaries 
DICTS     = HGCC3D HGCC2D HGCTC HGCgen HGCgenpart HGChit ### add here the classes you wanna add to ROOT ###
DICTS_CXX = $(DICTS:%=Dict%.cxx)
DICTS_H   = $(DICTS:%=$(INC_DIR)/%.h)


#############
### RULES ###
#############

all: $(EXE) 


$(EXE): %.exe: %.cpp $(OBJ) dict
	@mkdir -p $(BIN_DIR)
	$(CXX) -o bin/$(notdir $@) $(DICTS_CXX) $< $(CXXFLAGS) $(CXXLIBS) $(OBJ) 


$(OBJ): $(OBJ_DIR)%.o: $(SRC_DIR)%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CXXLIBS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm Dict*

init:
	@mkdir -p $(INC_DIR)
	@mkdir -p $(SRC_DIR)

dict: $(DICTS)
	@echo ROOT dictionaries generated 

$(DICTS):
	rootcint -f Dict$@.cxx -c $(CXXFLAGS) -p $(INC_DIR)/$@.h $(INC_DIR)/$@linkDef.h

echo: 
	echo $(DIC_H)    
	echo $(DIC_CXX)
	echo $(DIC_LINKDEF) 
#	echo $(HEPMCFLAGS)
#	echo $(EXE)
#	echo $(OBJ)
#	echo $(SOURCES_CC)
#	echo $(CURRENT_DIR)
##	echo $(BOOST_DIR)
#	echo $(INC_DIR)
#	echo $(SRC_DIR)
#	echo $(SOURCES_CC)
#	echo $(OBJ)
	echo $(MAINS_CPP)
	echo $(EXE)

