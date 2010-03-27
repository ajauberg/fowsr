CC = gcc
NAME = fowsr
SRC = fowsr.c
OBJ = $(subst .c,.o, $(SRC))
LDFLAGS += -lusb

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

clean:
	-rm -f $(NAME) *.d *.o
