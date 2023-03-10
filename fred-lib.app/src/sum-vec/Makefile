BIN = fred-test-cli
LIBFRED = fred
FRED_PATH ?= /opt/fredsys

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

CFLAGS += -std=gnu99 -g -Wall -Werror -fpic -I $(FRED_PATH)/include
LDFLAGS += -lpthread 

$(BIN): $(OBJS) 
	$(CC) $(OBJS) -o $@ $(LDFLAGS) -L $(FRED_PATH)/lib -l$(LIBFRED)

# include all dep makefiles generated using the next rule
-include $(DEPS)

# Pattern rule for generating makefiles rules based
# on headers includes dependencies using the C preprocessor
%.d: %.c
	$(CPP) $< -MM -MT $(@:.d=.o) > $@

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJS) $(DEPS)

install:
	mkdir -p ${FRED_PATH}/examples
	cp ${BIN} ${FRED_PATH}/examples
