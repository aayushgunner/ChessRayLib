CC = gcc
CFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11
SRCS = first.c
EXECUTABLE = chess 
all: $(EXECUTABLE)

$(EXECUTABLE):$(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(EXECUTABLE)
