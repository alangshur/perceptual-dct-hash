CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
INCLUDE = -Iinclude
SRC_PATH = ./src
MODULES = $(shell ./utils/modules.sh)
NAME = main

$(NAME): $(MODULES)
	$(CXX) $^ -o $@

%.o: $(SRC_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

.PHONY: all
all:
	@make --silent clean
	@make --silent main
	@make --silent run
	@make --silent clean

.PHONY: comp
comp: $(MODULES)
	@make --silent clean

.PHONY: clean
clean: 
	rm -f $(NAME) *.o

.PHONY: run
run:
	./$(NAME)
