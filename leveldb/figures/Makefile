# Makefile to convert all .puml -> .svg using PlantUML
# Usage:
#   make            # builds all SVGs from PUML files in this directory
#   make clean      # removes generated SVGs
# Optionally:
#   make PLANTUML="java -jar /path/to/plantuml.jar"

# find all .puml files in current directory
PUML := $(wildcard *.puml)
SVG  := $(PUML:.puml=.svg)

# plantuml command (override on make command line if you want)
PLANTUML ?= plantuml

.PHONY: all clean

all: $(SVG)
	@echo "Built $(words $(SVG)) SVG(s): $(SVG)"

# pattern rule: .puml -> .svg
%.svg: %.puml
	@echo "Rendering $< -> $@"
	$(PLANTUML) -tsvg $<

clean:
	@echo "Cleaning generated SVGs..."
	$(RM) $(SVG)