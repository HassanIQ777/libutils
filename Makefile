# Makefile for libutils (header-only library)
# It's header-only, but we'll still pretend it's a mighty beast.

LIB_NAME := libutils
SRC_DIR  := src
INCLUDE  := -I$(SRC_DIR)
HEADERS  := $(wildcard $(SRC_DIR)/*.hpp)
BINDIR   := build
LIB_FILE := $(BINDIR)/$(LIB_NAME).a

.PHONY: all static install clean

all: $(LIB_FILE)
	@echo "$(LIB_NAME): Nothing to compile (header-only). Archive created for show."

# Fake static lib: we "pack" the headers into an archive for formality
$(LIB_FILE): $(HEADERS)
	@mkdir -p $(BINDIR)
	@rm -f $(LIB_FILE)
	@ar rcs $(LIB_FILE) $^
	@echo "Packed headers into $(LIB_FILE) (completely unnecessary, but sinister)."

# Install headers system-wide (default: /usr/local/include/libutils)
PREFIX ?= /usr/local
install: $(HEADERS)
	@mkdir -p $(PREFIX)/include/$(LIB_NAME)
	@cp -u $(HEADERS) $(PREFIX)/include/$(LIB_NAME)/
	@echo "Installed headers to $(PREFIX)/include/$(LIB_NAME)"

# Clean up the ashes
clean:
	@rm -rf $(BINDIR)
	@echo "$(LIB_NAME): All traces erased... the headers still haunt /src."
