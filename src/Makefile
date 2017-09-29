#
# make file
#
DIR_CUR := $(shell pwd)
CC = gcc
CFLAGS = -Wall -Wno-error -g -O2 -I. -I./include/ -I./include/protob
LDFLAGS = -lpthread -lsqlite3

SRCS = main.c net.c Encoder.c Decoder.c Control.c datacache.c
#SRCS += protob/pb_common.c protob/pb_decode.c protob/pb_encode.c protob/MsgUtil.c \
protob/ConnectReq.pb.c
SRCS += $(wildcard protob/*.c)

OBJS = $(patsubst %.c, %.o, $(SRCS))
TARGET = client

all:$(TARGET) ECHO_OK

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

ECHO_OK:
	@echo "#"
	@echo "# make successfully"
	@echo "#"

clean:
	-rm -rf $(OBJS) $(TARGET)

.PHONY:all clean $(TARGET) ECHO_OK
