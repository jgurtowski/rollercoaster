CC=g++
CFLAGS=-Wall -O3 -ggdb #-pg


BINARIES=kmerwrite kmerlookup readcounts #hetcorrector correctread

all: $(BINARIES)

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

KMERWRITE_O = kmer_writer.o kmer_record.o packed_kmer.o packed_sequence.o
kmerwrite: $(KMERWRITE_O)
	$(CC) $(CFLAGS) $(KMERWRITE_O) -o kmerwrite

KMERLOOKUP_O = kmer_lookup.o mmap_file.o mmap_kmer_lookup.o packed_kmer.o packed_sequence.o kmer_record.o
kmerlookup: $(KMERLOOKUP_O)
	$(CC) $(CFLAGS) $(KMERLOOKUP_O) -o kmerlookup


HETCORRECTOR_O = het_corrector_main.o fastq_record.o het_corrector.o abstract_kmer_lookup.o mmap_file.o mmap_kmer_lookup.o kmer_creator.o packed_kmer.o packed_sequence.o list_backed_kmer.o kmer_record.o coverage_segment.o
hetcorrector: $(HETCORRECTOR_O)
	$(CC) -lpthread $(CFLAGS) $(HETCORRECTOR_O) -o hetcorrector

DISTSAMPLER_O = dist_sampler.o fastq_record.o het_corrector.o abstract_kmer_lookup.o mmap_file.o mmap_kmer_lookup.o kmer_creator.o packed_kmer.o packed_sequence.o list_backed_kmer.o kmer_record.o coverage_segment.o
distsampler: $(DISTSAMPLER_O)
	$(CC) -lpthread $(CFLAGS) $(DISTSAMPLER_O) -o distsampler


READCOUNTS_O = fastq_record.o abstract_kmer_lookup.o mmap_file.o mmap_kmer_lookup.o kmer_creator.o packed_kmer.o packed_sequence.o list_backed_kmer.o kmer_record.o coverage_segment.o read_counts.cc
readcounts: $(READCOUNTS_O)
	$(CC) -lpthread $(CFLAGS) $(READCOUNTS_O) -o readcounts

CORRECTREAD_O = correct_read_simple.o fastq_record.o het_corrector.o abstract_kmer_lookup.o mmap_file.o mmap_kmer_lookup.o kmer_creator.o packed_kmer.o packed_sequence.o list_backed_kmer.o kmer_record.o coverage_segment.o
correctread: $(CORRECTREAD_O)
	$(CC) -lpthread $(CFLAGS) $(CORRECTREAD_O) -o correctread

clean:
	rm *.o $(BINARIES)
