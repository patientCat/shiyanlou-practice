SOURCES := $(wildcard *.cpp)
OBJS := $(SOURCES:.cpp=.o)
DEPS := $(SOURCES:.cpp=.d)
EXCE := cmsys
CC := g++
FLAG := -c --std=c++11
$(EXCE):$(OBJS)
	$(CC) $^ -o $@
$(OBJS):%.o:%.cpp
	$(CC) $(FLAG) $< -o $@
.PHONY:clean
clean:
	rm -rf $(OBJS) $(DEPS) $(EXCE)
sinclude $(DEPS)
$(DEPS):%.d:%.cpp
	$(CC) -MM $< > $@
