
SOURCE = $(wildcard *.c)
OBJECTS = $(SOURCE:.c=.o)

all: $(OBJECTS)
%.o: %.c
	gcc -c -g $<

clean:
	rm *.o
