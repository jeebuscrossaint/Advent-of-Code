# Compiler and flags
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++23 -O2
LDFLAGS = -lcurl -lgumbo

# Directories
SRCDIR = src
BUILDDIR = build
BINDIR = $(BUILDDIR)/bin

# Installation directory
PREFIX = /usr/local
INSTALL_DIR = $(PREFIX)/bin

# Target executable name
TARGET = $(BINDIR)/aocli

# Source and object files
SOURCES = $(wildcard $(SRCDIR)/*.cc)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)

# Default target
all: dirs $(TARGET)

# Create build directories
dirs:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Install the binary
install: all
	@echo "Installing to $(INSTALL_DIR)..."
	@mkdir -p $(INSTALL_DIR)
	@install -m 755 $(TARGET) $(INSTALL_DIR)
	@echo "Installation complete! You can now use 'aocli' from anywhere."

# Run the program
run: all
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILDDIR)

# Uninstall
uninstall:
	@echo "Removing $(INSTALL_DIR)/aocli..."
	@rm -f $(INSTALL_DIR)/aocli
	@echo "Uninstall complete!"

.PHONY: all dirs clean run install uninstall
