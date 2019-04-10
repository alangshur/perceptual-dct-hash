CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDE = -Iinclude
MODULES = $(shell find src -name *.cpp)
LIBS = $(shell pkg-config --cflags --libs opencv4) # opencv
LIBS += $(shell pkg-config --cflags --libs openssl) # openssl
NAME = main

all: $(NAME)

$(NAME): $(MODULES)
	g++ $(CXXFLAGS) $(INCLUDE) $(LIBS) $^ -o $@

.PHONY: clean
clean: 
	rm -f $(NAME) *.o
	rm -f *.bmp
