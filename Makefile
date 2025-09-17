TARGET = bin/program

CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2

OBJDIR = bin

SRC = main.cpp algorithms/algorithms.cpp algorithms/vnd.cpp 

OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*

.PHONY: all clean
