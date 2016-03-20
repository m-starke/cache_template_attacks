all: spy
spy: spy.c ../../cacheutils.h
	clang -std=gnu11 -O2 -o $@ $<
	#gcc -std=gnu11 -O2 -o $@ $< -lrt -pthread

