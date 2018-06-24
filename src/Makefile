CC = clang
CFLAGS = -fno-stack-protector
TARGET = virtualife
OBJECTS = main.o zero.o lea.o mov.o add.o sub.o syscall.o cmp.o jmp.o xor.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	rm *.o
