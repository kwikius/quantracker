
#include <cstdlib>
#include <stm32f4xx.h>
#include <misc.h>

extern "C" void __cxa_pure_virtual()
{
     while (1);
}

void *__dso_handle;

extern "C" void   vPortFree( void *pv );
extern "C" void * pvPortMalloc(size_t n);

void operator delete (void* pv){ vPortFree(pv);}
void* operator new (unsigned int n){ return pvPortMalloc(n);}
