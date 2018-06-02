CC=gcc
32: testmem_modified.c
	$(info =========== BUILDING 32BIT EXAMPLE ===========)
	$(CC) -march=native -O3 -m32 testmem_modified.c -o tm32

64: testmem_modified.c
	$(info =========== BUILDING 64BIT EXAMPLE ===========)
	$(CC) -march=native -O3 -m64 testmem_modified.c -o tm64
