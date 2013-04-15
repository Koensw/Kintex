VPATH = src:src/core:src/core/tokens/operators:src/core/tokens/operands/:src/core/tokens/names/:src/core/tokens/control:src/tokens:external:lib
DEPDIR = build

ifneq ($(OS),Windows_NT)
	CXXFLAGS = -Wall -pedantic -g -std=c++0x
	df = $(DEPDIR)/$(*F)
else
	CXXFLAGS = -Wall -pedantic -g -std=c++0x
	df = $(DEPDIR)/$(*F)
endif

OPERATORS = add.o multiply.o subtract.o divide.o exponent.o set.o equality.o inequality.o
OPERANDS = integer.o floating-point.o void.o
NAMES = variable.o variable_creator.o function.o function_creator.o instantiated_function.o
CONTROL = parentheses.o statement.o brackets.o
BASE = operand.o operator.o kintex.o level.o processor.o interpreter.o program.o token.o exception.o
LIBRARY = default_library.o interactive_library.o
EXTERNAL = getopt_pp.o

SRCS = ${OPERATORS} ${OPERANDS} ${NAMES} ${CONTROL} ${BASE} ${LIBRARY} ${EXTERNAL}

#build
kintex : $(addprefix build/,${SRCS})
	g++ ${CXXFLAGS} -o bin/kintex $(addprefix build/,${SRCS})

build/%.o : %.cpp
	g++ ${CXXFLAGS} -c -MD -o $@ $<
#ifneq ($(OS),Windows_NT)
	@cp $(df).d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; \
	rm -f $(df).d
#else
#	@cp $(df).d $(df).P > NUL 
#	@sed -e "s/#.*//" -e "s/^[^:]*: *//" -e "s/ *\\$$//" -e "/^$$/ d" -e "s/$$/ :/" < $(df).d >> $(df).P 
#	@rm $(df).d
#endif

-include $(SRCS:%.o=$(DEPDIR)/%.P)

# PHONY's now only for linux!
#ifneq ($(OS),Windows_NT)
.PHONY: clean
.PHONY: install
.PHONY: test
# clean operation
clean :
	-rm -rf build/*
	find ./ -name '*~' | xargs rm

# install operation
install:
	@bash install.sh

# install operation
uninstall:
	@bash install.sh uninstallr

#execute tests
test:
	@bash test/tests.sh
	
#endif
