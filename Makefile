BUILD_DIR=build

all: $(BUILD_DIR)/Makefile
	$(MAKE) -C "$(BUILD_DIR)" all

benchmark: all
	"$(BUILD_DIR)"/bin/benchmark_nucleotide_count

clean:
	if test -d "$(BUILD_DIR)"; then $(MAKE) -C "$(BUILD_DIR)" clean; fi

veryclean:
	rm -r "$(BUILD_DIR)"

$(BUILD_DIR)/Makefile:
	test -d "$(BUILD_DIR)" || mkdir "$(BUILD_DIR)"
	WD=`pwd`; cd "$(BUILD_DIR)" && cmake -DCMAKE_BUILD_TYPE=Release "$$WD"

.PHONY: all test sb clean veryclean
