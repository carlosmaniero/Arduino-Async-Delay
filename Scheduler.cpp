#include <stdlib.h>
#include <Scheduler.h>

#ifdef __unix
    #include <Arduino_local.h>
#else
    #include <Arduino.h>
#endif

typedef struct process {
    unsigned int id;
    Task *task;
    unsigned long int avg_execution;
    process *next;
} process;

void set_empty_process(process **p) {
    process *tmp = (process *) malloc(sizeof(process));
    tmp->next = NULL;
    tmp->task = NULL;
    tmp->id = 0;
    tmp->avg_execution = 0;
    *p = tmp;
}

Scheduler::Scheduler() {
    set_empty_process(&processes);
    last_process = processes;

    set_empty_process(&priority_processes);
    last_priority_process = priority_processes;

    set_empty_process(&selected_process);
    execute_priority = false;
    priority_control = 0;
    last_pid = 0;
}

void Scheduler::updateQueue() {
    if (selected_process != NULL && selected_process->task != NULL) {
        if (!selected_process->task->isTerminated()) {
            // Change process of queue if necessary
            if (selected_process->task->isPriority()) {
                last_priority_process->next = selected_process;
                last_priority_process = selected_process;
            }
            else {
                last_process->next = selected_process;
                last_process = selected_process;
            }
        }
        if (execute_priority) {
            priority_processes->next = selected_process->next;
        }
        else {
            processes->next = selected_process->next;
        }
        selected_process->next = NULL;
    }

    
    // Invert priority
    if (priority_processes->next != NULL){
        priority_control = ++priority_control % 3;
        execute_priority = priority_control != 0;
    }else{
        execute_priority = false;
    }

    if (execute_priority) {
        selected_process = priority_processes->next;
    }
    else {
        selected_process = processes->next;
    }
}

int Scheduler::addTask(Task *async_task) {
    process *new_process;
    set_empty_process(&new_process);
    new_process->id = ++last_pid;
    new_process->task = async_task;

    // Put task in correct queue
    if (async_task->isPriority()) {
        last_priority_process->next = new_process;
        last_priority_process = new_process;
    }
    else {
        last_process->next = new_process;    
        last_process = new_process;    
    }
}

bool Scheduler::kill(int pid) {
    process *tmp = processes;
    while (tmp) {
        if (tmp->next->id == pid) {
            tmp->next = tmp->next->next;
            return true;
        }     
        tmp = tmp->next;
    }
    tmp = priority_processes;
    while (tmp) {
        if (tmp->next->id == pid) {
            tmp->next = tmp->next->next;
            return true;
        }     
        tmp = tmp->next;
    }
    return false;
}

void Scheduler::run() {
    unsigned long start;
    if (selected_process != NULL && selected_process->task != NULL) {
        if(selected_process->task->isReady()) {
            start = millis();
            selected_process->task->run(); 
            selected_process->task->postRun(); 
            selected_process->avg_execution = (
                selected_process->avg_execution + (start - millis()) 
            ) / 2;
        }
    }
    updateQueue();
}

void Scheduler::run(unsigned long int max_time) {
    unsigned long start;
    if (selected_process != NULL && selected_process->task != NULL) {
        if(selected_process->task->isReady() && max_time > selected_process->avg_execution) {
            start = millis();
            selected_process->task->run();
            selected_process->task->postRun();
            selected_process->avg_execution = (
                selected_process->avg_execution + (start - millis()) 
            ) / 2;
        }
    }
    updateQueue();
}

void Scheduler::delay(unsigned long int milliseconds) {
    unsigned long int start = millis();
    unsigned long int end = start;
    selected_process->task->wait();

    while (start + milliseconds > end) {
        run(end - (start + milliseconds)); 
        end = millis();
    }

    selected_process->task->ready();
}
