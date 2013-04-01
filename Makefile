################################################################################
#
# Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
#
# NOTICE TO USER:   
#
# This source code is subject to NVIDIA ownership rights under U.S. and 
# international Copyright laws.  
#
# NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE 
# CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR 
# IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH 
# REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF 
# MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.   
# IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL, 
# OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
# OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
# OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE 
# OR PERFORMANCE OF THIS SOURCE CODE.  
#
# U.S. Government End Users.  This source code is a "commercial item" as 
# that term is defined at 48 C.F.R. 2.101 (OCT 1995), consisting  of 
# "commercial computer software" and "commercial computer software 
# documentation" as such terms are used in 48 C.F.R. 12.212 (SEPT 1995) 
# and is provided to the U.S. Government only as a commercial end item.  
# Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through 
# 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the 
# source code with only those rights set forth herein.
#
################################################################################
#
# Build script for project
#
################################################################################

# OS Name (Linux or Darwin)
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])

# Flags to detect 32-bit or 64-bit OS platform
OS_SIZE = $(shell uname -m | sed -e "s/i.86/32/" -e "s/x86_64/64/")
OS_ARCH = $(shell uname -m | sed -e "s/i386/i686/")

# These flags will override any settings
ifeq ($(i386),1)
	OS_SIZE = 32
	OS_ARCH = i686
endif

ifeq ($(x86_64),1)
	OS_SIZE = 64
	OS_ARCH = x86_64
endif

# Flags to detect either a Linux system (linux) or Mac OSX (darwin)
DARWIN = $(strip $(findstring DARWIN, $(OSUPPER)))

# Location of the CUDA Toolkit binaries and libraries
CUDA_PATH       ?= /usr/local/cuda-5.0
CUDA_INC_PATH   ?= $(CUDA_PATH)/include
CUDA_BIN_PATH   ?= $(CUDA_PATH)/bin
ifneq ($(DARWIN),)
  CUDA_LIB_PATH  ?= $(CUDA_PATH)/lib
else
  ifeq ($(OS_SIZE),32)
    CUDA_LIB_PATH  ?= $(CUDA_PATH)/lib
  else
    CUDA_LIB_PATH  ?= $(CUDA_PATH)/lib64
  endif
endif

# Common binaries
NVCC            ?= $(CUDA_BIN_PATH)/nvcc
GCC             ?= g++

# Extra user flags
EXTRA_NVCCFLAGS ?=
EXTRA_LDFLAGS   ?=
EXTRA_CCFLAGS   ?=

# CUDA code generation flags
GENCODE_SM10    := -gencode arch=compute_10,code=sm_10
GENCODE_SM20    := -gencode arch=compute_20,code=sm_20
GENCODE_SM30    := -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35
GENCODE_FLAGS   := $(GENCODE_SM10) $(GENCODE_SM20) $(GENCODE_SM30)

# OS-specific build flags
ifneq ($(DARWIN),) 
      LDFLAGS   := -Xlinker -rpath $(CUDA_LIB_PATH) -L$(CUDA_LIB_PATH) -lcudart
      CCFLAGS   := -arch $(OS_ARCH) 
else
  ifeq ($(OS_SIZE),32)
      LDFLAGS   := -L$(CUDA_LIB_PATH) -lcudart
      CCFLAGS   := -m32
  else
      LDFLAGS   := -L$(CUDA_LIB_PATH) -lcudart
      CCFLAGS   := -m64
  endif
endif

# OS-architecture specific flags
ifeq ($(OS_SIZE),32)
      NVCCFLAGS := -m32
else
      NVCCFLAGS := -m64
endif

# OpenGL specific libraries 
ifneq ($(DARWIN),)
    # Mac OSX specific libraries and paths to include
    LIBPATH_OPENGL  := -L../../common/lib/darwin -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -lGL -lGLU ../../common/lib/darwin/libGLEW.a
else
    # Linux specific libraries and paths to include
    LIBPATH_OPENGL  := -L../NVIDIA_CUDA-5.0_Samples/common/lib/linux/$(OS_ARCH) -L/usr/X11R6/lib -lGL -lGLU -lX11 -lXi -lXmu -lglut ../NVIDIA_CUDA-5.0_Samples/common/lib/linux/$(OS_ARCH)/libGLEW.a
endif

# Debug build flags
ifeq ($(dbg),1)
      CCFLAGS   += -g
      NVCCFLAGS += -g -G
      TARGET := debug
else
      TARGET := release
endif

# Common includes and paths for CUDA
INCLUDES      := -I$(CUDA_INC_PATH) -I. -I.. -I../NVIDIA_CUDA-5.0_Samples/common/inc
LDFLAGS       += $(LIBPATH_OPENGL)

CC = g++
#CFLAGS = -g -I. -I/usr/local/gcc280/lib/g++-include
CFLAGS = -g -I. -O3 -Wall -I/usr/local/lib/g++-include

# Target rules
all: build

build: slam

SRC = mt-rand.o ThisRobot.o basic.o map.o lowMap.o low.o highMap.o high.o slam.o parallel.o
slam : $(SRC)
	$(CC) $(CFLAGS) -o slam $(SRC) $(LDFLAGS)
	mkdir -p ../../bin/$(OSLOWER)/$(TARGET)
	cp $@ ../../bin/$(OSLOWER)/$(TARGET)

parallel.o: parallel.cu parallel.h
	$(NVCC) $(NVCCFLAGS) $(EXTRA_NVCCFLAGS) $(GENCODE_FLAGS) $(INCLUDES) -o $@ -c $<

slam.o : slam.cpp high.h
	$(CC) $(CFLAGS) $(INCLUDES) -c slam.cpp

high.o : high.c high.h highMap.h
	$(CC) $(CFLAGS) $(INCLUDES) -c high.c

highMap.o : highMap.c highMap.h low.h 
	$(CC) $(CFLAGS)  $(INCLUDES) -c highMap.c

low.o : low.c low.h lowMap.h
	$(CC) $(CFLAGS)  $(INCLUDES) -c low.c

lowMap.o : lowMap.c lowMap.h map.h
	$(CC) $(CFLAGS) $(INCLUDES)  -c lowMap.c

mt-rand.o : mt-rand.c mt-rand.h
	$(CC) $(CFLAGS)  $(INCLUDES) -c mt-rand.c

ThisRobot.o : ThisRobot.h basic.h
	$(CC) $(CFLAGS)  $(INCLUDES) -c ThisRobot.cpp

basic.o : basic.h
	$(CC) $(CFLAGS)  $(INCLUDES) -c basic.c

map.o : laser.h map.h map.c
	$(CC) $(CFLAGS)  $(INCLUDES) -c map.c

run: build
	./slam

clean:
	rm -f slam *.o *.bin
