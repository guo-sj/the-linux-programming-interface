TARGET = copy
SOURCE = $(wildcard *.c)
OBJECTS = $(SOURCE:.c=.o)


$(TARGET): $(OBJECTS)
	cc copy.o ../../lib/*.o -o $@

%.o: %.c
	cc -g -c -I../../lib $<

clean:
	rm $(OBJECTS) $(TARGET)
