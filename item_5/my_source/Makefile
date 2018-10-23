source := $(wildcard *.c)
target := $(source:.c=)
all: $(target)
fileinfo: fileinfo.c
	gcc -o $@ $^
ls1: ls1.c
	gcc -o $@ $^
ls: ls.c
	gcc -o $@ $^
clean:
	rm -rf $(target)
