.PHONY: all help linux msys2 clean clean-linux clean-msys2

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
	@echo "  make clean-linux      remove Linux build artifacts"
	@echo "  make clean-msys2      remove MSYS2 build artifacts"
	@echo "  make clean            remove both build trees"
	@echo ""
	@echo "Examples:"
	@echo "  make linux"
	@echo "  make linux USE_MPI=1"
	@echo "  make msys2"
	@echo "  make msys2 USE_MPI=1"
	@echo ""

linux:
	@$(MAKE) -C src -f Makefile.linux $(filter-out $@,$(MAKECMDGOALS))

msys2:
	@$(MAKE) -C src -f Makefile.msys2 $(filter-out $@,$(MAKECMDGOALS))

clean-linux:
	@$(MAKE) -C src -f Makefile.linux clean

clean-msys2:
	@$(MAKE) -C src -f Makefile.msys2 clean

clean: clean-linux clean-msys2

%:
	@:
