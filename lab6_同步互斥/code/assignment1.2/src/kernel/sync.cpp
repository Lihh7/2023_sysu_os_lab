#include "sync.h"
#include "asm_utils.h"
#include "stdio.h"
#include "os_modules.h"

SpinLock::SpinLock()
{
    initialize();
}

void SpinLock::initialize()
{
  
}

void SpinLock::lock()
{
    asm_my_lock();
/*
    uint32 key = 1;

    do
    {
        asm_atomic_exchange(&key, &bolt);
        //printf("pid: %d\n", programManager.running->pid);
    } while (key);
*/
}

void SpinLock::unlock()
{
    asm_my_unlock();
}
