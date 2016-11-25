# 2010-7-16

src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))
target = main

CC = gcc
CFLAGS = -Wall -c -g -I../jpeg-8 -I../freetype2
LDFLAGS = -ljpeg -L../jpeg-8/.libs -lpthread -L../usr/local/include -lfreetype

all :$(target) 
$(target): $(obj)
	gcc -o $@ $^ $(LDFLAGS)
%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	-rm -f *.o
	-rm -f $(target)
	@-rm -f *~ *~~
.PHONY: clean

