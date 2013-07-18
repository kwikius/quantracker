
#include <errno.h>
#include <stddef.h> /* where ptrdiff_t is defined */
/*
  required by sprintf when floating point arg is required
*/

extern "C"{
  // used by sprintf for static alloc
/* Extend heap space by size bytes.
   Return start of new space allocated, or -1 for errors 
   Error cases:
    1. Allocation is not within heap range */

	void * _sbrk (ptrdiff_t size)
	{
	  /*
	  * The following two memory locations should be defined in the linker script file
	  */
	  extern const char _HEAP_START; /* start of heap */
	  extern const char _HEAP_MAX;	 /* end of heap (maximum value of heap_ptr) */

	  static const char * heap_ptr = &_HEAP_START;  /* pointer to head of heap */
	 
     const char * new_heap_ptr = heap_ptr + size;
	  if (new_heap_ptr > &_HEAP_MAX)
	  { 
		 /* top of heap is bigger than _HEAP_MAX */
		 errno = ENOMEM;
		 return (void *) -1;
	  }
     const char * old_heap_ptr = heap_ptr;
	  /* success: update heap_ptr and return previous value */
	  heap_ptr = new_heap_ptr;
	  return (void *)old_heap_ptr;
	}
} // extern "C"