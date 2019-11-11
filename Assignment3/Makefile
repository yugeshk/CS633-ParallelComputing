CC		= mpicc
COPT	= -g -03

LD		= $(CC)
LDFLAGS	= $(COPT)

all: src.x

hpc: 
	mpiicc src.c -o src.x

src.x:
	$(CC) src.c -o src.x

clean:
	rm src.x
