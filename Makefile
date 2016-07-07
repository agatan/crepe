SRCS := crepe.cpp printer.cpp matcher.cpp
OBJS := $(subst .cpp,.o,$(SRCS))

COMPILER := g++
CPPFLAGS := -std=c++11 -O3

TARGET := crepe

.PHONY: all clean

all: crepe

crepe: $(OBJS)
	$(COMPILER) $(CPPFLAGS) -o $@ $^

%.o: %.cpp
	$(COMPILER) $(CPPFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS)
