

#include <stddef.h>
#include <stdint.h>
#include <emmintrin.h>


    #define INLINE         __inline__ __attribute__((always_inline))



static INLINE void* memcpy_sse(void *dst, const void *src, size_t size)
{
	size_t pad; 

	// this might not matter for powers of two, but it was
	// a bug in testing random buffer sizes. TODO: think about this. 
	pad = (16 - (((size_t)dst) & 15)) & 15;

	if (pad > 0) {
		// TODO: I am not sure this is bug free... or the right way to do this. 
		__m128i top = _mm_loadu_si128((const __m128i*)src);
		
		_mm_storeu_si128((__m128i*)dst, top);
		src += pad;
		dst += pad;
		
		size -= pad;
	}

	_mm_prefetch((const char*)(src), _MM_HINT_NTA);

		for (; size >= 128; size -= 128) {
			__m128i w0, w1, w2, w3, w4, w5, w6, w7;
			w0 = _mm_load_si128(((const __m128i*)src) + 0);
			w1 = _mm_load_si128(((const __m128i*)src) + 1);
			w2 = _mm_load_si128(((const __m128i*)src) + 2);
			w3 = _mm_load_si128(((const __m128i*)src) + 3);
			w4 = _mm_load_si128(((const __m128i*)src) + 4);
			w5 = _mm_load_si128(((const __m128i*)src) + 5);
			w6 = _mm_load_si128(((const __m128i*)src) + 6);
			w7 = _mm_load_si128(((const __m128i*)src) + 7);
			_mm_prefetch((const char*)(src + 256), _MM_HINT_NTA);
			src += 128;
			_mm_stream_si128((((__m128i*)dst) + 0), w0);
			_mm_stream_si128((((__m128i*)dst) + 1), w1);
			_mm_stream_si128((((__m128i*)dst) + 2), w2);
			_mm_stream_si128((((__m128i*)dst) + 3), w3);
			_mm_stream_si128((((__m128i*)dst) + 4), w4);
			_mm_stream_si128((((__m128i*)dst) + 5), w5);
			_mm_stream_si128((((__m128i*)dst) + 6), w6);
			_mm_stream_si128((((__m128i*)dst) + 7), w7);
			dst += 128;
		}
	
	// TODO: Last 128 bytes matter when not aligned? 
	// Docs said we need this? 
	_mm_sfence();
	
	return dst;
}


