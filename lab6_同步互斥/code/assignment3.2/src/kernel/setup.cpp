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
Semaphore fork[5];
int eating[5]={0,0,0,0,0};
int use[5][2]={{0,0},{0,0},{0,0},{0,0},{0,0}};

void philosopher0(void *arg){
    int i=0;
    while(1){
	printf("philosopher %d is thinking\n",i);
	for(int j=0;j<200380013;j++){}
	fork[i].P();
	use[i][0]=1;
	fork[(i+1)%5].P();
	use[i][1]=1;
	eating[i]=1;
	printf("philosopher %d is eating\n",i);
	for(int j=0;j<1032132;j++){}
	eating[i]=0;
	fork[(i+1)%5].V();
	use[i][1]=0;
	fork[i].V();
	use[i][0]=0;
    }
}

void philosopher1(void *arg){
    int i=1;
    while(1){
	printf("philosopher %d is thinking\n",i);
	for(int j=0;j<1320005;j++){}
	fork[i].P();
        use[i][0]=1;
	fork[(i+1)%5].P();
	use[i][1]=1;
	eating[i]=1;
	printf("philosopher %d is eating\n",i);
	for(int j=0;j<346754;j++){}
	eating[i]=0;
	fork[(i+1)%5].V();
	use[i][1]=0;
	fork[i].V();
	use[i][0]=0;
    }
}

void philosopher2(void *arg){
    int i=2;
    while(1){
	printf("philosopher %d is thinking\n",i);
	for(int j=0;j<106547580;j++){}
	fork[i].P();
        use[i][0]=1;
	fork[(i+1)%5].P();
	use[i][1]=1;
	eating[i]=1;
	printf("philosopher %d is eating\n",i);
	for(int j=0;j<150;j++){}
	eating[i]=0;
	fork[(i+1)%5].V();
	use[i][1]=0;
	fork[i].V();
	use[i][0]=0;
    }
}

void philosopher3(void *arg){
    int i=3;
    while(1){
	printf("philosopher %d is thinking\n",i);
	for(int j=0;j<63543650;j++){}
	fork[i].P();
        use[i][0]=1;
	fork[(i+1)%5].P();
	use[i][1]=1;
	eating[i]=1;
	printf("philosopher %d is eating\n",i);
	for(int j=0;j<103750;j++){}
	eating[i]=0;
	fork[(i+1)%5].V();
	use[i][1]=0;
	fork[i].V();
	use[i][0]=0;
    }
}

void philosopher4(void *arg){
    int i=4;
    while(1){
	printf("philosopher %d is thinking\n",i);
	for(int j=0;j<106585000;j++){}
	fork[(i+1)%5].P();
	use[i][1]=1;
	fork[i].P();
        use[i][0]=1;
	eating[i]=1;
	printf("philosopher %d is eating\n",i);
	for(int j=0;j<103550;j++){}
	eating[i]=0;
	fork[i].V();
	use[i][0]=0;
	fork[(i+1)%5].V();
	use[i][1]=0;
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

    for(int i=0;i<5;i++){
        fork[i].initialize(1);
    }

    programManager.executeThread(philosopher0, nullptr, "second thread", 1);
    programManager.executeThread(philosopher1, nullptr, "third thread", 1);
    programManager.executeThread(philosopher2, nullptr, "forth thread", 1);
    programManager.executeThread(philosopher3, nullptr, "fifth thread", 1);
    programManager.executeThread(philosopher4, nullptr, "sixth thread", 1);
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
