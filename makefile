# hellomake: fe.c pipe_reader.h pipe_reader.c
# 	gcc -o fe fe.c -I.
	
HEADERS = pipe_reader.h monitor.h
OBJECTS = pipe_reader.o fe.o monitor.o

default: fe

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

fe: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f fe	