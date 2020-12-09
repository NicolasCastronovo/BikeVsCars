CC = gcc
CFLAGS = -Wall -std=c99
FLAGGLUT = -lglut -lGL -lGLU
SRCS = main.c game.c draw.c structure.c display.c
OBJET = $(SRCS:.c = .o)
EXEC = bvc

all: $(EXEC)

$(EXEC): $(SRCS)
		$(CC) -g -c $(SRCS) $(FLAG) $(FLAGGLUT)
		$(CC) -g -o $(EXEC) $(OBJET) $(FLAG) $(FLAGGLUT)

clean:
		rm -rf $(EXEC)
		rm -rf *.o