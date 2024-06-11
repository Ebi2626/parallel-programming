CC=mpicc
LDFLAGS="-Wl,--copy-dt-needed-entries"
all: zad1 zad2 monte-carlo monte-carlo-seq
zad1:
	$(CC) zad-1.c -o zad-1

zad2:
	$(CC) zad-2.c -o zad-2

monte-carlo:
	$(CC) $(LDFLAGS) monte-carlo.c -o monte-carlo

monte-carlo-seq:
	$(CC) $(LDFLAGS) monte-carlo-seq.c -o monte-carlo-seq -lm

clean:
	rm -r zad-1 zad-2 monte-carlo monte-carlo-seq
