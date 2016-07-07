SRCS := crepe.cpp printer.cpp matcher.cpp walker.cpp
OBJS := $(subst .cpp,.o,$(SRCS))

COMPILER := g++
CPPFLAGS := -std=c++11 -O3
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
