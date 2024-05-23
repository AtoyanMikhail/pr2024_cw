CC := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++14 -Wshadow -Wformat=2 -Wconversion -Weffc++ -Wfloat-equal -fstack-protector-strong -fPIE -pie
LIBS := 
SRC_DIR := source
INCDIR := include
OBJDIR := obj


SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE := cw

DOXYGEN_CONFIG := Doxyfile

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LIBS)

$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clear:
	$(RM) -r $(OBJDIR) $(EXECUTABLE)

docs:
	@doxygen $(DOXYGEN_CONFIG) 

.PHONY: all clean docs