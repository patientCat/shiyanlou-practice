SOURCES := $(wildcard *.c)
OBJS := $(SOURCES:.c=.o)
DEPS := $(SOURCES:.c=.d)
EXCE := lru
CC := gcc
CFLAGS := 
$(EXCE) : $(OBJS)
	$(CC) -o $@ $^
$(OBJS) : %.o : %.c
	$(CC) -o $@ -c $<
sinclude $(DEPS)
$(DEPS) : %.d : %.c
	$(CC) -o $@ -MM $<
.Phony:clean
clean:
	rm -rf $(DEPS) $(EXCE) $(OBJS)
