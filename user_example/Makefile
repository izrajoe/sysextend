# Make to end all make

# TODO
# bring in line with http://aegis.sourceforge.net/auug97.pdf
# header .d files for tracking header dependencies (-MM -MG)
# better bin and archive solution than copying
# find executable source?

# ===============================================
# User configured variables
# ===============================================

IDIRS:= . ../include
ODIR:=./obj
LDIRS:= .
CFLAGS:=-pthread -O0 -g 
CXXFLAGS:= -pthread -std=c++11 -O0 -g
LIBS:=-lpthread
LDFLAGS:= 
SRCDIRS:= .
ARCHIVENAME:=
BINDIR:= ./bin
ARCHIVEDIR:= ./lib

# executables are files with main methods
# within the source directory
# they are compiled independently of each other
# to avoid conflicts.  Since we remove these
# from the source list, the pathing scheme 
# (relative vs absolute) must be the same
# for both SRCDIRS and EXECUTABLES so we can
# do the matching
# generated targets have the same name 
# without the file extension

EXECUTABLES:= example.cpp


HOSTNAME:= $(shell hostname)
ifeq ($(HOSTNAME),vax)
	CC:=clang
	CXX:=clang++
else
	CC:=gcc
	CXX:=g++
endif
LD:= $(CXX)


# exec : compiles all executables
# lib : compiles archive (.a) library file
default : exec


# example complex command
#.PHONY: complex-command
#complex-command : _LINK_ARGS +=  -latlas_full_pcache_5gb
#complex-command : _CXX_COMP_ARGS +=  -DATLAS -DPCACHE
#complex-command : exec lib clean_memory

# =============================================
# Automatic stuff. Hopefully you don't need to
# access below here.
# =============================================

# -------------------------
# Functions
# -------------------------

_SEP:=-_

# mangle takes a file path and converts it into 
# a basename for a single file
mangle=$(subst /,$(_SEP),$(basename $1))

# unmangle takes a file name and converts it
# into a full file path 
unmangle=$(subst $(_SEP),/,$(notdir $(basename $1)))

# mangletarget takes an executable and converts it into 
# a basename for a single file
#mangletarget=$(subst /,$(_SEP),$1)
mangletarget=$(subst $(_SEP),/,$(strip $1))

# because make doesn't have a newline character
define \n

endef

# because make lacks equals
eq = $(and $(findstring $(1),$(2)),$(findstring $(2),$(1)))

# -------------------------
# Generate variables
# -------------------------

_HOSTNAME := $(shell hostname)
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
DIR_PATH := $(patsubst %/,%,$(dir $(MKFILE_PATH)))
CURRENT_DIR := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))

_DEPDIRS:=$(IDIRS)
_IDIRS:=$(foreach d, $(IDIRS), -I $d)
_LDIRS:=$(foreach d, $(LDIRS), -L $d)
_C_COMP_ARGS:= $(CFLAGS) $(_IDIRS)
_CXX_COMP_ARGS:= $(CXXFLAGS) $(_IDIRS)
_LINK_ARGS:= $(LIBS) $(_LDIRS) $(LDFLAGS)
_ODIR:=$(ODIR)
_BINDIR:=$(BINDIR)
_ARCHIVEDIR:=$(ARCHIVEDIR)

ifeq ($(strip $(ARCHIVENAME)),)
	_ARCHIVENAME:=lib$(CURRENT_DIR).a
else
	_ARCHIVENAME:=$(ARCHIVENAME)
endif


_EXECUTABLE_FULL_PATHS:=$(foreach x, $(EXECUTABLES), $(patsubst ./%,%,$(x)))
_TARGETS :=$(foreach x,$(_EXECUTABLE_FULL_PATHS),$(notdir $(basename $(x))).bin)
_TARGETS_MOVED :=$(foreach x,$(_EXECUTABLE_FULL_PATHS),$(_BINDIR)/$(notdir $(basename $(x))).bin)
_ARCHIVE_MOVED :=$(_ARCHIVEDIR)/$(notdir $(_ARCHIVENAME))
_DEPS_H :=$(foreach d, $(_DEPDIRS), $(shell find $d/ -iname '*.h' -type f | sed 's/ /\\ /g'))
_DEPS_HPP :=$(foreach d, $(_DEPDIRS), $(shell find $d/ -iname '*.hpp' -type f | sed 's/ /\\ /g'))
_DEPS :=$(_DEPS_H) $(_DEPS_HPP) $(MKFILE_PATH)
_SRCS_C :=$(foreach d, $(SRCDIRS), $(shell find $d/ -iname '*.c' -type f | sed 's/ /\\ /g'))
_SRCS_CPP :=$(foreach d, $(SRCDIRS), $(shell find $d/ -iname '*.cpp' -type f | sed 's/ /\\ /g'))
_SRCS := $(foreach x, $(_SRCS_C) $(_SRCS_CPP), $(patsubst ./%,%,$(x)))
_SRCS_FULL_PATHS :=$(filter-out $(_EXECUTABLE_FULL_PATHS), $(foreach x, $(_SRCS), $(x)))
_OBJECTS :=$(foreach x, $(basename $(_SRCS)), $(x).o)
_OBJECTS_MOVED :=$(foreach x, $(_SRCS_FULL_PATHS), $(_ODIR)/$(call mangle, $(x)).o)






createDirs := $(foreach d, $(_ODIR) $(_BINDIR) $(_ARCHIVEDIR) $(dir $(_ARCHIVENAME)), $(shell test -d $(d) || mkdir -p $(d)))

# -------------------------
# Print variables
# -------------------------

$(info Dependencies: ${_DEPS})
$(info Include Dirs: ${IDIRS})
$(info Lib Dirs: ${LDIRS})
$(info Executable Files: ${_EXECUTABLE_FULL_PATHS})
$(info Nonexecutable Source Files: ${_SRCS_FULL_PATHS})
$(info Targets: ${_TARGETS_MOVED})
$(info Mangled Nonexecutable Object Files: ${_OBJECTS_MOVED})
$(info Archive File: ${_ARCHIVE_MOVED})
$(info Path: ${DIR_PATH})
$(info Host: ${_HOSTNAME})
$(info ${\n})


# ------------------------
# Makefile pragmas
# ------------------------

.SECONDEXPANSION: # for sorcery.  just look it up in the manual.
.SUFFIXES: # for getting rid of implicit rules, because they make debugging hard


# ------------------------
# Build rules for target
# ------------------------

exec: $(_TARGETS)

$(_TARGETS): $$(_ODIR)/$$(call mangle, $$@).o $(_OBJECTS_MOVED) $(_DEPS) 
	$(if $(findstring .atl,$@), \
		$(LD) -o $@ $(_OBJECTS_MOVED) $(_ODIR)/$(call mangle, $@).o  $(_LINK_ARGS) ,  \
		$(LD) -o $@ $(_OBJECTS_MOVED) $(_ODIR)/$(call mangle, $@).o) $(_LINK_ARGS) 
	$(if $(call eq $(shell readlink -f $@), $(shell readlink -f $(_BINDIR)/$(notdir $@)) ), \
		, \
		mv $@ $(_BINDIR)/$(notdir $@))
	

# ------------------------------
# Default build rules for c/c++
# ------------------------------

# Compile c code
$(_ODIR)/%.o: $$(call unmangle, $$@).c $(_DEPS)
	$(CC) -c $(call unmangle, $@).c -o $(call unmangle, $@).o $(_C_COMP_ARGS) 
	mv $(call unmangle, $@).o $@

# Compile cpp code
$(_ODIR)/%.o: $$(call unmangle, $$@).cpp $(_DEPS)
	$(CXX) -c $(call unmangle, $@).cpp -o $(call unmangle, $@).o  $(_CXX_COMP_ARGS) 
	mv $(call unmangle, $@).o $@

# ---------------------
# Library build rules
# ---------------------

# builds a library of all 
# source (minus executables)
lib: $(_ARCHIVENAME)

$(_ARCHIVENAME): $(_OBJECTS_MOVED) $(_DEPS) 
	ar rcs $(_ARCHIVENAME) $(_OBJECTS_MOVED)
	$(if $(call eq $(shell readlink -f $@), $(shell readlink -f $(_ARCHIVEDIR)/$(notdir $@)) ), \
		, \
		mv $@ $(_ARCHIVEDIR)/$(notdir $@))

# ---------------------
# Auxiliary build rules
# ---------------------

clean:
	rm $(_TARGETS) $(_ODIR)/*.o $(_ARCHIVENAME) $(_TARGETS_MOVED) $(_ARCHIVE_MOVED) Makefile~ 2> /dev/null || true
	$(foreach d, $(SRCDIRS), $(shell rm $d/*.c~ $d/*.cpp~ $d/*.h~ $d/*.hpp~ 2> /dev/null || true) )
	$(foreach d, $(SRCDIRS), $(info rm ${d}/*.c~ ${d}/*.cpp~ ${d}/*.h~ ${d}/*.hpp~ 2> /dev/null || true) )


clean_memory:
	rm -f -r /dev/shm/* || true



























