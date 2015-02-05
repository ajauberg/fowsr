CC = gcc
#CFLAGS += -std=c99

NAME = fowsr
SRC = fowsr.c
LDFLAGS += -lm -lusb

all:
#in the hope PKG_SOURCE is only defined when compiling for openwrt
ifdef PKG_SOURCE
	@echo compiling for openwrt
	$(CC) $(CFLAGS) -DIS_OPENWRT -o $(NAME) $(SRC) $(LDFLAGS) -g
else
	@echo compiling for all others
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) $(LDFLAGS) -g
endif

clean:
	-rm -f $(NAME) *.d *.o
