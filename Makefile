SRCS := crepe.cpp printer.cpp matcher.cpp walker.cpp file.cpp option.cpp color.cpp
OBJS := $(subst .cpp,.o,$(SRCS))
DEPS := $(subst .cpp,.d,$(SRCS))

COMPILER := g++
CPPFLAGS := -std=c++14 -O3 -MMD -MP
LDFLAGS := -lboost_filesystem -lboost_system -lboost_program_options

TARGET := crepe

.PHONY: all clean

all: crepe

crepe: $(OBJS)
	$(COMPILER) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(COMPILER) $(CPPFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
