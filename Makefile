# SkiaPlot Makefile
# Alternative build system for users who prefer Make over CMake

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
INCLUDES := -I./include

# Skia configuration
ifndef SKIA_DIR
    $(error SKIA_DIR is not set. Please set it: export SKIA_DIR=/path/to/skia)
endif

SKIA_INCLUDES := -I$(SKIA_DIR)/include
SKIA_LIB_DIR := $(SKIA_DIR)/out/Release
SKIA_LIBS := -L$(SKIA_LIB_DIR) -lskia

# System libraries
# Note: For Windows builds, use CMake or Visual Studio instead of this Makefile
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    SYS_LIBS := -lpthread -lfontconfig -lfreetype -lGL
endif
ifeq ($(UNAME_S),Darwin)
    SYS_LIBS := -framework CoreFoundation -framework CoreGraphics -framework CoreText -framework OpenGL
endif
ifeq ($(OS),Windows_NT)
    $(error Windows is not supported with this Makefile. Please use CMake instead.)
endif

LIBS := $(SKIA_LIBS) $(SYS_LIBS)

# Source files
SRC_DIR := src
EXAMPLE_DIR := examples
BUILD_DIR := build

# Object files
SKIAPLOT_OBJ := $(BUILD_DIR)/skiaplot.o

# Example programs
EXAMPLES := simple_plot sine_wave multiple_series
EXAMPLE_BINS := $(addprefix $(BUILD_DIR)/,$(EXAMPLES))

.PHONY: all clean examples

all: $(BUILD_DIR) $(EXAMPLE_BINS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SKIAPLOT_OBJ): $(SRC_DIR)/skiaplot.cpp include/skiaplot.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SKIA_INCLUDES) -c $< -o $@

$(BUILD_DIR)/simple_plot: $(EXAMPLE_DIR)/simple_plot.cpp $(SKIAPLOT_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SKIA_INCLUDES) $^ -o $@ $(LIBS)

$(BUILD_DIR)/sine_wave: $(EXAMPLE_DIR)/sine_wave.cpp $(SKIAPLOT_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SKIA_INCLUDES) $^ -o $@ $(LIBS)

$(BUILD_DIR)/multiple_series: $(EXAMPLE_DIR)/multiple_series.cpp $(SKIAPLOT_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SKIA_INCLUDES) $^ -o $@ $(LIBS)

examples: $(EXAMPLE_BINS)

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "SkiaPlot Makefile"
	@echo ""
	@echo "Usage:"
	@echo "  make all       - Build all examples"
	@echo "  make examples  - Build all examples"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make help      - Show this help message"
	@echo ""
	@echo "Platform Support:"
	@echo "  - Linux: Fully supported"
	@echo "  - macOS: Fully supported"
	@echo "  - Windows: Not supported (use CMake instead)"
	@echo ""
	@echo "Requirements:"
	@echo "  - Set SKIA_DIR environment variable"
	@echo "  - Example: export SKIA_DIR=/path/to/skia"
	@echo ""
	@echo "Example programs will be in $(BUILD_DIR)/"
