CC := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Werror -MMD -MP -Iinclude
DEBUG_FLAGS := -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
RELEASE_FLAGS := -O2

SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := $(BINDIR)/procview

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY: all debug release run clean dirs

all: release

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean dirs $(TARGET)

release: CFLAGS += $(RELEASE_FLAGS)
release: clean dirs $(TARGET)

run: all
	./$(TARGET) $$$$

dirs:
	@mkdir -p $(BUILDDIR) $(BINDIR)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(OBJECTS:.o=.d)

clean:
	@rm -rf $(BUILDDIR) $(BINDIR)
