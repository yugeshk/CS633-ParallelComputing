CC		= mpicc
COPT	= -g -03

LD		= $(CC)
LDFLAGS = $(COPT)

all: src.x

%.x: %.o src.o
	$(LD) $< -o $@

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm *.o *.x > /dev/null
