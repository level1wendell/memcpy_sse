
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <stdint.h>
    #include <mm_malloc.h>
    #include "memcpy_sse.h"

typedef struct { unsigned char dummy [32]; } DT;
//I found this on stack overflow. It's crap for our purposes...
// so not actually used, but you could try it too instead of memcpy() 
	void fastcpy(unsigned char* dst, unsigned char* src, size_t s) 
	{
	         unsigned char* sa = src+s;
	         DT *d1 = (DT*)dst - 1;
	         DT *s1 = (DT*)src - 1;
	         size_t si = s / sizeof(DT);
	 
	         si = (si + 7) / 8;
	         switch(si % 8) 
	         {
	                 case 0: do { *++d1 = *++s1;
	                 case 7:      *++d1 = *++s1;
	                 case 6:      *++d1 = *++s1;
	                 case 5:      *++d1 = *++s1;
	                 case 4:      *++d1 = *++s1;
	                 case 3:      *++d1 = *++s1;
	                 case 2:      *++d1 = *++s1;
	                 case 1:      *++d1 = *++s1;
	                             } while(--si > 0);
	         }
	 
	         dst = (unsigned char*)d1;
	         src = (unsigned char*)s1;
	         while(src < sa) 
	         {
	                 *++dst = *++src;
	         }
	}


    static inline uint64_t nanotime()
    {
	
      struct timespec time;
      clock_gettime(CLOCK_MONOTONIC_RAW, &time);
      return ((uint64_t)time.tv_sec * 1e9) + time.tv_nsec;
    }

    int main(int argc, char * argv[])
    {
    const int s = atoi(argv[1]) + 0 ;
    int size = s * 1024 * 1024;
    //int size = 32 * 1024 * 1024;
    char * buffer1 = malloc(size);
    char * buffer2 = malloc(size);

// prevent -O3 from optimizing away the buffer ... 
for(int i = 0; i < s; i++) {
    sprintf(buffer1 + i, "%x", rand() % 16);
}


    uint64_t t = nanotime();  
    void *(*memcpy_ptr)(void *, const void *, size_t) = memcpy;    
    for(volatile int i = 0; i < 1000; ++i)
       memcpy_ptr(buffer1, buffer2, size );
    printf("%2u MB = %f ms\n", s, ((float)(nanotime() - t) / 1000.0f) / 1000000.0f);
    printf("-Compare match (should be zero): %2u \n\n", memcmp(buffer1,buffer2,size)) ;
    free(buffer1);
    free(buffer2);
    return 0;
    }

