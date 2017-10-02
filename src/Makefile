#
# make file
#
DIR_CUR := $(shell pwd)
CC = gcc
CFLAGS = -Wall -Wno-error -g -O2 -I. -I./include/ -I./include/protob
LDFLAGS = -lpthread -lsqlite3

SRCS = main.c net.c Encoder.c Decoder.c Control.c datacache.c
SRCS_PROTOB = $(wildcard protob/*.c)

OBJS = $(patsubst %.c, %.o, $(SRCS))
OBJS_PROTOB = $(patsubst %.c, %.o, $(SRCS_PROTOB))

TARGET = client

all:$(TARGET) ECHO_OK

$(TARGET):$(OBJS) $(OBJS_PROTOB)
	$(CC) -o $@ $(OBJS) $(OBJS_PROTOB) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

ECHO_OK:
	@echo "#"
	@echo "# make successfully"
	@echo "#"

clean:
	-rm -rf $(OBJS) $(TARGET)

dist-clean:
	-rm -rf $(OBJS_PROTOB)

.PHONY:all clean dist-clean $(TARGET) ECHO_OK
