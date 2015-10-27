#include <stdio.h>
#include <iostream>
#include <Task.h>
#include <Scheduler.h>

#ifdef __unix
    #include <Arduino_local.h>
#else
    #include <Arduino.h>
#endif

// Initialize Scheduler
Scheduler scheduler;

// Declare the tasks
class HelloTask : public Task {
    public:
        virtual void run(){
            std::cout << "Hello, world!\n" << std::flush;

            // Use Scheduler.delay to stop a execution
            scheduler.delay(1000);
            std::cout << "How are you?\n" << std::flush;
        };
};

class ByeTask : public Task {
    public:
        virtual void run(){
            std::cout << "Bye, world!\n" << std::flush;

            // Use interval to set when the task will be executed again.
            setInterval(200);                   
        };
};

// Initialize tasks
HelloTask hello_task;
ByeTask bye_task;

int main(){
    scheduler.addTask(&hello_task);
    scheduler.addTask(&bye_task);
    while(1){
        scheduler.run();
    }
}
