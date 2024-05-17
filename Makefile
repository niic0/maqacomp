# path icc: /opt/intel/oneapi/compiler/2024.1/bin/icx
CC=gcc
CFLAGS=-Ofast -g -Wall
LDFLAGS=-lm
OPTFLAGS=-O2 -g -Wall
OBJS_COMMON=kernel.o rdtsc.o perf_event.c

all: measure

measure: $(OBJS_COMMON) main.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

kernel.o: kernel.c
	$(CC) $(OPTFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_COMMON) main.o measure
