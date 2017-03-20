
#include <cstdlib>
#include <cstring>
#include <stm32f4xx.h>
#include <misc.h>

extern "C" void __cxa_pure_virtual()
{
     while (1);
}

//void *__dso_handle;

extern "C" void   vPortFree( void *pv );
extern "C" void * pvPortMalloc(size_t n);

void operator delete (void* pv)
{ 
   if (pv) {
      vPortFree(pv);
   } 
}
void* operator new (size_t n_in){ 
   size_t const n = (n_in > 1) ? n_in : 1;
   void * result = pvPortMalloc(n );
   if ( result != nullptr){
      memset(result,0,n);
   }
   return result;
}


// should prrob init to 0 for ardupilot
void * operator new[](size_t n_in)
{
   size_t const n = (n_in > 1) ? n_in : 1;
   void * result = pvPortMalloc(n );
   if ( result != nullptr){
      memset(result,0,n);
   }
   return result;
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
