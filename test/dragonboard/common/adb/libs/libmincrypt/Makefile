TARGET          = libmincrypt.a
INCLUDES        += -I.

SRCS := rsa.c rsa_e_3.c rsa_e_f4.c sha.c
OBJS    = $(SRCS:.c=.o)
CC := arm-linux-gnueabi-gcc
%.o: %.c
	@echo "  LN\t$^ -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(TARGET): $(OBJS)
	@echo "  AR\t$^ -> $@"
	@ar r $@ $(OBJS)

all:$(TARGET)

clean:
	rm -rf $(TARGET) *.o *.a *~

