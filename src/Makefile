# Parellelize make
MAKEFLAGS+=-j8

CC=clang++
CPPFLAGS=-Wall -Wextra -Wpedantic -std=c++17

LDIR=../lib
ROOT=$(shell cd ..; pwd)
INC=$(wildcard $(ROOT)/src/vendor/*/) $(ROOT)/include $(ROOT)/src
INC_PARAMS=$(foreach d, $(INC), -I $d)
LIBS=-L$(LDIR) -lGLEW -lglfw3 -Wl,-Bdynamic -lGL -ldl -lX11 -lstdc++ -pthread -lm -static

# Flags according to build type
DEBUG=-D_GLIBCXX_DEBUG -g -O0
VERBOSE=-v
RELEASE=-Ofast

# Filenames
CPPFILES=$(wildcard *.cpp) $(wildcard vendor/*/*.cpp)
OBJS=$(patsubst %.cpp, %.o, $(CPPFILES))
DEPENDS=$(OBJS:.o=.d)



.DEFAULT: make
.PHONY: make main



make: CPPFLAGS += $(DEBUG)
make: main

# Verbose
verbose: CPPFLAGS += $(DEBUG)
verbose: CPPFLAGS += $(VERBOSE)
verbose: main

# Release
release: CPPFLAGS += $(RELEASE)
release: main


main: $(OBJS)
	$(CC) $(OBJS) -L $(LDIR) $(LIBS) -o main


# Generate dependency files for recompilation on header file changes

-include $(DEPENDS)

%.o: %.cpp
	$(CC) -c $(INC_PARAMS) $(CPPFLAGS) -MMD -MF $(patsubst %.o, %.d, $@)  -o $@ $<
	@echo -------
	@echo

run: make
	./main

clean:
	rm -f main a.out $(OBJS) $(DEPENDS)

#
# imgui
# GLEW
# GLFW
