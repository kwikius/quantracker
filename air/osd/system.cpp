
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

void * operator new[](size_t n)
{
    return pvPortMalloc( ((n>0)?n:1) );
}

void operator delete[](void * ptr)
{
    if (ptr) vPortFree(ptr);
}

// from Ardupilot/libraries/AP_Common/c++.cpp
__extension__ typedef int __guard __attribute__((mode (__DI__)));

int __cxa_guard_acquire(__guard *g)
{
    return !*(char *)(g);
};

void __cxa_guard_release (__guard *g)
{
    *(char *)g = 1;
};

void __cxa_guard_abort (__guard *) 
{
};
