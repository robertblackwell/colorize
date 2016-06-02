# hellomake: fe.c pipe_reader.h pipe_reader.c
# 	gcc -o fe fe.c -I.
	
HEADERS = pipe_reader.h monitor.h runner.h color.h formatter.h
OBJECTS = pipe_reader.o colorize.o monitor.o runner.o color.o formatter.o
TARGET=colorize

default: $(TARGET)

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

$(TARGET): $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)	