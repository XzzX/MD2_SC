# files:
OBJ = CellSubdivision.o Color.o Configuration.o Event.o MDSystem.o MeanVar.o Particle.o Window.o main.o util.o
BIN = main

# compiler options
CXX = g++
CCFLAGS = -O3 -march=native -Wall -Werror -Wextra -ansi -pedantic
#CCFLAGS = -O0 -g -march=native -Wall -Werror -Wextra -ansi -pedantic
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lglut -lGLU -lboost_system -lboost_timer -fopenmp


SRC = $(OBJ:%.o=%.cpp)
HDR = $(OBJ:%.o=%.hpp)
DEPENDFILE = .depend

.PHONY: all
all:	dep $(BIN)

.PHONY: run
run:	$(BIN)
	./$(BIN)

# dependencies
-include $(DEPENDFILE)
.PHONY: dep
dep:	$(SRC)
	$(CXX) -MM $(SRC) > $(DEPENDFILE)

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ) $(DEPENDFILE)

# linking
$(BIN):	$(OBJ)
	$(CXX) $(CCFLAGS) $+ -o $@ $(LDFLAGS)

# compiling
%.o: %.cc
	$(CXX) $(CCFLAGS) -c $< -o $@

