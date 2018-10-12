# 银行排队模拟系统makefile
# 源文件
SOURCES := $(wildcard *.cpp)
# 目标文件
OBJECTS := $(SOURCES:.cpp=.o)
# 依赖文件
DEPS := $(SOURCES:.cpp=.d)
# 可执行程序
EXEC := main
CC := g++
FLAGS := -c --std=c++11
$(EXEC) : $(OBJECTS)
	$(CC) -o $@ $^
$(OBJECTS):%.o:%.cpp
	$(CC) $(FLAGS) $< -o $@
.PHONY:clean
clean:
	rm -rf $(EXEC) $(OBJECTS) $(DEPS)

sinclude $(DEPS)
$(DEPS):%.d:%.cpp
	$(CC) -MM $< > $@
