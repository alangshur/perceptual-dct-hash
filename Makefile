CXX = g++
CXXFLAGS = -std=c++17 -Wall
CXXFLAGS_WARN_OFF += -Wno-unused-private-field
INCLUDE = -Iinclude
MODULES = $(shell find src -name *.cpp)
LIBS = $(shell pkg-config --cflags --libs opencv4) # opencv
NAME = pure-image

all: $(NAME)

$(NAME): $(MODULES)
	g++ $(CXXFLAGS) $(CXXFLAGS_WARN_OFF) $(INCLUDE) $(LIBS) $^ -o $@

.PHONY: clean
clean: 
	rm -f $(NAME) *.o
	rm -f *.bmp
