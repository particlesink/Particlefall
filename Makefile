.PHONY: all help linux msys2 doc html clean distclean clean-linux clean-msys2 clean-doc clean-tests clean-examples

PYTHON ?= python3

STD ?= -std=gnu++17
export STD

ifeq ($(OS),Windows_NT)
DEFAULT_TARGET := msys2
else
DEFAULT_TARGET := linux
endif

all: $(DEFAULT_TARGET)

help:
	@echo ""
	@echo "Supported build targets"
	@echo ""
	@echo "  make linux            build with src/Makefile.linux"
	@echo "  make msys2            build with src/Makefile.msys2"
	@echo "  make html             build the Sphinx HTML docs in doc/_build/html"
	@echo "  make doc              alias for make html"
	@echo "  make clean-linux      remove Linux build artifacts"
	@echo "  make clean-msys2      remove MSYS2 build artifacts"
	@echo "  make clean-doc        remove generated doc build artifacts"
	@echo "  make clean-tests      remove generated test artifacts"
	@echo "  make clean-examples   remove generated example artifacts"
	@echo "  make clean            remove build, doc, test, and example artifacts"
	@echo "  make distclean        also remove the top-level build directory"
	@echo ""
	@echo "Examples:"
	@echo "  make linux"
	@echo "  make linux USE_MPI=1"
	@echo "  make msys2"
	@echo "  make msys2 USE_MPI=1"
	@echo "  make html"
	@echo ""

linux:
	@$(MAKE) -C src -f Makefile.linux $(filter-out $@,$(MAKECMDGOALS))

msys2:
	@$(MAKE) -C src -f Makefile.msys2 $(filter-out $@,$(MAKECMDGOALS))

html:
	@$(MAKE) -C doc -f Makefile html

doc: html

clean-linux:
	@$(MAKE) -C src -f Makefile.linux clean

clean-msys2:
	@$(MAKE) -C src -f Makefile.msys2 clean

clean-doc:
	@$(MAKE) -C doc -f Makefile clean

clean-tests:
	@$(PYTHON) tests/run.py --clean
	@find tests/cases -type f -name 'log.*' -delete

clean-examples:
	@$(PYTHON) examples/run.py --clean
	@find examples -type f -name 'log.*' -delete

clean: clean-linux clean-msys2 clean-doc clean-tests clean-examples

distclean: clean
	@rm -rf build

%:
	@:
