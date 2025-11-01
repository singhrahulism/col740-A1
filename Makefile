# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Targets and source files
TARGET = run_doxygen
SRC = main.cpp

# PlantUML settings
FIGURES_DIR = leveldb/figures
PLANTUML ?= plantuml
PUML := $(wildcard $(FIGURES_DIR)/*.puml)
SVG  := $(PUML:.puml=.svg)

all: $(TARGET) figures

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: all
	./$(TARGET)

figures: $(SVG)
	@echo "Built $(words $(SVG)) SVG(s): $(notdir $(SVG))"

$(FIGURES_DIR)/%.svg: $(FIGURES_DIR)/%.puml
	@echo "Rendering $< -> $@"
	$(PLANTUML) -tsvg $<

clean:
	rm -f $(TARGET)
	@echo "Cleaning generated SVGs..."
	$(RM) $(SVG)

.PHONY: all run clean figures
