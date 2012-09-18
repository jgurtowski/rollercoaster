CC=g++
CFLAGS=-Wall -O3 -ggdb

BINARIES=kmerwrite kmerlookup

all: $(BINARIES)

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

KMERWRITE_O = kmer_writer.o kmer_record.o packed_kmer.o packed_sequence.o
kmerwrite: $(KMERWRITE_O)
	$(CC) $(CFLAGS) $(KMERWRITE_O) -o kmerwrite



KMERLOOKUP_O = kmer_lookup.o mmap_file.o mmap_kmer_lookup.o packed_kmer.o packed_sequence.o kmer_record.o
kmerlookup: $(KMERLOOKUP_O)
	$(CC) $(CFLAGS) $(KMERLOOKUP_O) -o kmerlookup


clean:
	rm *.o $(BINARIES)
