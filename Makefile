CC=g++
CFLAGS=-Wall -O3 #-pg

BINARIES=kmerwrite kmerlookup readcorrector readcounts correctread

all: $(BINARIES)

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

KMERWRITE_O = kmer_writer.o kmer_record.o packed_kmer.o packed_sequence.o
kmerwrite: $(KMERWRITE_O)
	$(CC) $(CFLAGS) $(KMERWRITE_O) -o kmerwrite

KMERLOOKUP_O = kmer_lookup.o mmap_file.o mmap_kmer_lookup.o packed_kmer.o packed_sequence.o kmer_record.o
kmerlookup: $(KMERLOOKUP_O)
	$(CC) $(CFLAGS) $(KMERLOOKUP_O) -o kmerlookup


READCORRECTOR_O = read_corrector.o fastq_record.o het_corrector.o abstract_kmer_lookup.o mmap_file.o mmap_kmer_lookup.o kmer_creator.o packed_kmer.o packed_sequence.o list_backed_kmer.o kmer_record.o coverage_segment.o
readcorrector: $(READCORRECTOR_O)
	$(CC) -lpthread $(CFLAGS) $(READCORRECTOR_O) -o readcorrector


READCOUNTS_O = fastq_record.o abstract_kmer_lookup.o mmap_file.o mmap_kmer_lookup.o kmer_creator.o packed_kmer.o packed_sequence.o list_backed_kmer.o kmer_record.o coverage_segment.o read_counts.cc
readcounts: $(READCOUNTS_O)
	$(CC) -lpthread $(CFLAGS) $(READCOUNTS_O) -o readcounts

CORRECTREAD_O = correct_read_simple.o fastq_record.o het_corrector.o abstract_kmer_lookup.o mmap_file.o mmap_kmer_lookup.o kmer_creator.o packed_kmer.o packed_sequence.o list_backed_kmer.o kmer_record.o coverage_segment.o
correctread: $(CORRECTREAD_O)
	$(CC) -lpthread $(CFLAGS) $(CORRECTREAD_O) -o correctread

clean:
	rm *.o $(BINARIES)
