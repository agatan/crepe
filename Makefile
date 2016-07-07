SRCS := crepe.cpp printer.cpp matcher.cpp walker.cpp file.cpp
OBJS := $(subst .cpp,.o,$(SRCS))
DEPS := $(subst .cpp,.d,$(SRCS))

COMPILER := g++
CPPFLAGS := -std=c++11 -O3 -MMD -MP
LDFLAGS := -lboost_filesystem -lboost_system

TARGET := crepe

.PHONY: all clean

all: crepe

crepe: $(OBJS)
	$(COMPILER) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(COMPILER) $(CPPFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS)

-include $(DEPS)
