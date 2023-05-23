BUILD := bin
TARGET := mera
LIBNAME := lib$(TARGET).a
TEST_TARGET := $(BUILD)/$(TARGET)_tests

CC := gcc
CPPFLAGS = -MMD $(INDIRS)
CFLAGS = -Wall -Wextra `pkg-config --cflags freetype2`
TESTFLAGS = -O2 -DCRIT_TEST `pkg-config --cflags criterion`
TESTLD = `pkg-config --libs criterion`
LDFLAGS := `pkg-config --libs freetype2`
LDLIBS := 

RELEASE := 0
ifneq ($(RELEASE), 0)
	CFLAGS += -O2
else
	CFLAGS += -Og -g -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
	CPPFLAGS += -DDEBUG
endif

TEST := 0
ifneq ($(TEST), 0)
	TEST_FILES_OPTS = -not -name "main.c"
else
	TEST_FILES_OPTS = -not -name "test[0-9]*.c"
endif

INDIRS := $(addprefix -I, \
	src \
)

TESTFILES := $(shell find tests -name "*.c")
TESTOFILES := $(TESTFILES:%.c=$(BUILD)/%.o)
MAINS := $(BUILD)/src/main.o $(BUILD)/src/test[0_9]*.o

CFILES := $(shell find src -name "*.c" $(TEST_FILES_OPTS))
OFILES := $(CFILES:%.c=$(BUILD)/%.o)
SRCFILES := $(filter-out $(MAINS),$(OFILES))
DFILES := $(OFILES:%.o=%.d)

SRCDIRS := $(shell find . -type d -not -path "*$(BUILD)*")
$(shell mkdir -p $(SRCDIRS:%=$(BUILD)/%))

all: $(TARGET)

lib: $(BUILD)/$(LIBNAME)

$(BUILD)/$(LIBNAME): $(SRCFILES)
	ar -rc $@ $^

$(TARGET): $(OFILES)
	$(V)$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(BUILD)/tests/%.o: tests/%.c
	$(V)$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS) $(TESTFLAGS)

$(BUILD)/%.o: %.c
	$(V)$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

clean:
	rm -rf $(BUILD) $(TARGET) $(GENERATED_FILES)

$(TEST_TARGET): $(SRCFILES) $(TESTOFILES)
	$(V)$(CC) -o $(TEST_TARGET) $^ $(LDFLAGS) $(LDLIBS) $(TESTLD)

test: $(TEST_TARGET)
	@./$(TEST_TARGET)

.PHONY: all clean test lib

-include $(DFILES)
