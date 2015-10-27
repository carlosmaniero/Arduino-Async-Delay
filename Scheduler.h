#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <Task.h>

typedef struct process process;

void set_empty_process(process *p);


class Scheduler {
    private:
        process *processes;
        process *last_process;
        process *selected_process;
        process *priority_processes;
        process *last_priority_process;
        int priority_control;
        bool execute_priority;
        int last_pid;

        void updateQueue();

    public:
        int addTask(Task *async_task);
        bool kill(int pid);
        void run();
        void run(unsigned long int max_time);
        void delay(unsigned long int milliseconds);
        Scheduler();
};
#endif
