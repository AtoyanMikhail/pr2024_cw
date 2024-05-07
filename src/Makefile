CC := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++14 -Wshadow -Wformat=2 -Wconversion -Weffc++ -Wfloat-equal -fstack-protector-strong -fPIE -pie
SRC_DIR := source
INCDIR := include
OBJDIR := obj


SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE := cw

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CXXFLAGS) $^ -o cw

$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) -I$(INCDIR) -c $< -o $(EXECUTABLE)

clean:
	$(RM) -r $(OBJDIR) $(EXECUTABLE)
