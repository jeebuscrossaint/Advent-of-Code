# Compiler and flags
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++23 -O2
LDFLAGS = -lcurl -lgumbo
BASH_COMPLETION_DIR = $(PREFIX)/share/bash-completion/completions
ZSH_COMPLETION_DIR = $(PREFIX)/share/zsh/site-functions
FISH_COMPLETION_DIR = $(PREFIX)/share/fish/vendor_completions.d

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

	@echo "Installing completions..."
	@mkdir -p $(BASH_COMPLETION_DIR)
	@mkdir -p $(ZSH_COMPLETION_DIR)
	@mkdir -p $(FISH_COMPLETION_DIR)
	@install -m 644 completions/aocli_completions.bash $(BASH_COMPLETION_DIR)/aocli
	@install -m 644 completions/aocli_completions.zsh $(ZSH_COMPLETION_DIR)/_aocli
	@install -m 644 completions/aocli_completions.fish $(FISH_COMPLETION_DIR)/aocli.fish

	@echo "Installation complete! You can now use 'aocli' from anywhere."
	@echo "To use the completions:"
	@echo "Bash: source /usr/local/share/bash-completion/completions/aocli"
	@echo "Zsh: # Add to fpath for testing"
	@echo "fpath=(completions $fpath)"
	@echo "autoload -U compinit"
	@echo "compinit"
	@echo "Fish:"
	@echo "source /usr/local/share/fish/vendor_completions.d/aocli.fish"


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
	@echo "Removing completions..."
	@rm -f $(BASH_COMPLETION_DIR)/aocli
	@rm -f $(ZSH_COMPLETION_DIR)/_aocli
	@rm -f $(FISH_COMPLETION_DIR)/aocli.fish
	@echo "Uninstall complete!"

.PHONY: all dirs clean run install uninstall
