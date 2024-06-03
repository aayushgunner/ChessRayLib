CC = gcc
CFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11
SRCS = main.c src/actions.c src/bot.c src/globals.c src/moves.c src/possible.c src/render.c src/server.c src/client.c
EXECUTABLE = chess 
all: $(EXECUTABLE) 

$(EXECUTABLE):$(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(EXECUTABLE)
