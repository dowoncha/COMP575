SRC = $(shell find . -name *.cpp)

OBJ = $(SRC:%.cpp=%.o)

BIN = bin/raytracer

all: $(BIN)

$(BIN): $(OBJ)
		g++ $(OBJ)

%.o: %.cpp
	g++ -std=c++11 -c $< -o $@
