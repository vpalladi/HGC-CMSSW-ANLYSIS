
# compiler
CXX = g++


################
### ext libs ###
################
#PYTHON_EXE = /usr/include/python2.6
#HEPMC_DIR  = $(HEPMCSYS)
#BOOST_DIR  = $(BOOSTSYS)
#RAWDATA_DIR = $(SIMRAWDATA)


####################
### useful paths ###
####################

# top and current dirs
#MKFILE_PATH = $(abspath $(lastword $(MAKEFILE_LIST)))
#TOP = $(patsubst %/,%,$(dir $(MKFILE_PATH)))
CURRENT_DIR = $(shell pwd)

# python
#PYTHON_CODE = $(TOP)/python

# naming
#NAMING_DIR = $(PYTHON_CODE)/naming

# local interface
#INTERFACE_DIR = $(TOP)/interface
#NAVIGATION_DIR = $(INTERFACE_DIR)/navigation

# bin dir
BIN_DIR = $(CURRENT_DIR)/bin

# config dir 
#CONFIG_DIR = $(TOP)/config

##############
### define ###
##############

#DEFINES = 
#DEFINES += -DRELATIVE_PATH
#DEFINES += -DRAW_DATA_PATH=\"$(SIMRAWDATA)\"
#DEFINES += -DCONFIG_PATH=\"$(TOP)/config\"
#DEFINES += -DWRITE_ANALYSIS


################################################
### copy the launcher in the $CURRENT folder ###
################################################
#LS_LAUNCHER = $(shell ls launcherIC.py 2>/dev/null)


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

# HepMC flags
#HEPMC_FLAGS = -I$(HEPMC_DIR)/include
#HEPMC_LIBS = -L$(HEPMC_DIR)/lib -lHepMC #-lHepMCfio 

# CXX flags
CXXFLAGS =  -Wall -std=c++11 -Wno-write-strings
CXXFLAGS += $(ROOT_CFLAGS)
#CXXFLAGS += $(BOOST_FLAGS)
#CXXFLAGS += $(HEPMC_FLAGS)
#CXXFLAGS += -I$(NAVIGATION_DIR) 
#CXXFLAGS += -I$(INTERFACE_DIR)
#CXXFLAGS += -I$(TOP)/src 
#CXXFLAGS += -I$(PYTHON_EXE)

# CXX LIBS
CXXLIBS =  $(ROOT_GLIBS)
#CXXLIBS += $(ROOT_LIBS)
#CXXLIBS += $(HEPMC_LIBS)
#CXXLIBS += $(BOOST_LIBS)

# all my files .ccp .cc and .hh
MAINS_CPP := $(wildcard ./*.cpp)
SOURCES_CC := $(wildcard ./*.cc)
INCLUDES_HH := $(wildcard ./*.hh)

EXES := $(MAINS_CPP:.cpp=.exe)

all: $(EXES) 

%.exe: %.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) -o bin/$(notdir $@) $< $(DEFINES) $(CXXFLAGS) $(CXXLIBS) 

#py_lib:
#	make TOP=$(TOP) -C $(INTERFACE_DIR)

clean:
	rm $(CURRENT_DIR)/bin/*
#        rm launcherIC.py
echo:
#	echo $(HEPMCFLAGS)
#	echo $(TOP)
	echo $(CURRENT_DIR)
#	echo $(BOOST_DIR)
#	echo $(INTERFACE_DIR)
#	echo $(CXX) $(CXXFLAGS) -o bin/$@ $<
#	echo $(EXES)
	echo $(MAINS_CPP)

#cplauncher:
#ifeq  ($(LS_LAUNCHER),launcherIC.py) 
#else
#	@echo " >>> copyng 'launcherIC.py' in the current directory"
#	@cp $(TOP)/python/launcherIC/launcherIC.py .
#	@chmod u+x launcherIC.py
#endif


#
#link_to_libs:
#	if [ ! -d $(LIB_DIR) ]; then mkdir $(LIB_DIR); ln -s $(BOOST_DIR) $(BOOST_LN); ln -s  $(HEPMC_DIR) $(HEPMC_LN); fi;
#
#link_to_config:
#	if [ ! -d $(CONFIG_DIR_LN) ]; then ln -s $(CONFIG_DIR_ABS) $(CONFIG_DIR_LN); fi;
#
#link_to_rawdata:
#	if [ ! -d $(RAWDATA_LN) ]; then ln -s $(RAWDATA_DIR) $(RAWDATA_LN); fi;
#
