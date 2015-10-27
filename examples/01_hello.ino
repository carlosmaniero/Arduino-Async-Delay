#include <Task.h>
#include <Scheduler.h>

// Initialize Scheduler
Scheduler scheduler;

// Declare the tasks
class HelloTask : public Task {
    public:
        virtual void run(){
            Serial.println("Hello, world!");

            // Use Scheduler.delay to stop a execution
            scheduler.delay(1000);
            Serial.println("How are you?");
        };
};

class ByeTask : public Task {
    public:
        virtual void run(){
            Serial.println("Bye, world!");

            // Use interval to set when the task will be executed again.
            setInterval(200);                   
        };
};

// Initialize tasks
HelloTask hello_task;
ByeTask bye_task;

void setup() {
    Serial.begin(9600);

    // Add task to scheduler
    scheduler.addTask(&hello_task);
    scheduler.addTask(&bye_task);
}

void loop() {
    // Run tasks
    scheduler.run();
}
