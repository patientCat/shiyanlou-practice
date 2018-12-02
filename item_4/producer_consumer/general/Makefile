sources := $(wildcard *.c)
elf := $(sources:.c=)
ld := -lpthread
$(elf) : $(sources)
	gcc -o $@ $< $(ld) 
