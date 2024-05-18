CC = gcc
CFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11
SRCS = main.c actions.c bot.c globals.c moves.c possible.c render.c
EXECUTABLE = chess 
all: $(EXECUTABLE)

$(EXECUTABLE):$(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(EXECUTABLE)
