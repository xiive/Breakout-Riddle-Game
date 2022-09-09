# C++ compiler
CXX := g++

# Compiler flags
CXX_VERSION_FLAG := -std=c++17

# Paths
SRCDIR := src
HDRDIR := include
BUILDDIR := build
BINDIR := bin

# File Extensions
SRCEXT := cpp
HDREXT := hpp

# Escape Codes
BOLD := \033[1m
BLUE := \033[34m
RESET := \033[0m

# Fetches source files
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
# Performs string substitution on each source file
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
# Linker flag
LINKER_FLAG := -L /usr/local/lib
# Linked dynamic libraries
LIBS := -l sfml-system -l sfml-window -l sfml-graphics -l sfml-audio -l sfml-network -Wl
# Include 
INC := -I include
# Unix Target Executable
TARGET := $(BINDIR)/Breakout

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo "$(BOLD)$(BLUE)==>$(RESET) Linking target executable < $@ >";
	$(CXX) $(CXX_VERSION_FLAG) $^ -o $(TARGET) $(INC) $(LINKER_FLAG) $(LIBS)
	
# Pattern rule that compiles every .cpp file into a .o file
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HDRDIR)/%.$(HDREXT)
	@mkdir -p $(BUILDDIR)
	@echo "$(BOLD)$(BLUE)==>$(RESET) Building OBJ file < $@ >";
	$(CXX) $(CXX_VERSION_FLAG) -Wall $(INC) -c -o $@ $<

# Testing rule
run:
	@./$(TARGET)

.PHONY: clean
clean:
	@echo "$(BOLD)$(BLUE)==>$(RESET) Cleaning up...";
	$(RM) -r core $(BUILDDIR) $(TARGET)
