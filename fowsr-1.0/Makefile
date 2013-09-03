CC = gcc
NAME = fowsr
SRC = fowsr.c
OBJ = $(subst .c,.o, $(SRC))
LDFLAGS += -lusb -lm

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDLIBS) $(LDFLAGS)

clean:
	-rm -f $(NAME) *.d *.o
