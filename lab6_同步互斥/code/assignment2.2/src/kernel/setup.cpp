#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;

Semaphore full;
Semaphore empty;
Semaphore mutex;
int buffer[100];
int size=0;

void producer(void *arg)
{
    while(1){
	empty.P();
	mutex.P();
        if(size==100){
      	    printf("full_error\n");
        }
        buffer[size++]=7;
	full.V();
	mutex.V();
    }
}

void resumer(void *arg){
    while(1){
	full.P();
	mutex.P();
        if(size==0){
	    printf("empty_error\n");
        }
        printf("%d\n",buffer[size-1]);
        buffer[size-1]=0;
        size--;
	empty.V();
	mutex.V();
    }
}




void first_thread(void *arg)
{
    // 第1个线程不可以返回
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }
    stdio.moveCursor(0);

    full.initialize(0);
    empty.initialize(100);
    mutex.initialize(1);
    programManager.executeThread(producer, nullptr, "second thread", 1);
    programManager.executeThread(resumer, nullptr, "third thread", 1);

    asm_halt();
}

extern "C" void setup_kernel()
{

    // 中断管理器
    interruptManager.initialize();
    interruptManager.enableTimeInterrupt();
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler);

    // 输出管理器
    stdio.initialize();

    // 进程/线程管理器
    programManager.initialize();

    // 创建第一个线程
    int pid = programManager.executeThread(first_thread, nullptr, "first thread", 1);
    if (pid == -1)
    {
        printf("can not execute thread\n");
        asm_halt();
    }

    ListItem *item = programManager.readyPrograms.front();
    PCB *firstThread = ListItem2PCB(item, tagInGeneralList);
    firstThread->status = RUNNING;
    programManager.readyPrograms.pop_front();
    programManager.running = firstThread;
    asm_switch_thread(0, firstThread);

    asm_halt();
}
