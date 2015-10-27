#include <stdlib.h>
#include <Task.h>

#ifdef __unix
    #include <Arduino_local.h>
#else
    #include <Arduino.h>
#endif

Task::Task() {
    is_ready = true;
    is_terminated = false;
    is_priority = false;
    interval = 0;
    last_execution = 0;
    stop_until = 0;
}

/**
 * Check if task is ready to execute
 */
bool Task::isReady() {
    unsigned long now = millis();
    return is_ready && (stop_until <= now) && ((last_execution + interval) <= now);
}

void Task::postRun() {
    last_execution = millis();
    times++;
}

/**
 * Check if task is terminated
 * In true case, TaskManager remove this from queue
 */
bool Task::isTerminated() {
    return is_terminated;
}

/**
 * Check if is a periodic task
 */
bool Task::isInterval() {
    return interval > 0;    
}

/**
 * Check if task should be in priority task
 */
bool Task::isPriority() {
    return is_priority;    
}

/**
 * Return the interval between executions
 */
unsigned long Task::getInterval() {
    return interval;  
}

/**
 * Define the ready state of task
 */
void Task::setReady(bool value) {
    is_ready = value;    
}

/**
 * Set the interval
 */
void Task::setInterval(unsigned long value) {
    interval = value;
}

/**
 * Define task ready to be executed
 */
void Task::ready() {
    is_ready = true; 
}

/**
 * Put task to sleep
 */
void Task::wait() {
    is_ready = false; 
}

/**
 * Put task to waiting milliseconds
 */
void Task::wait(unsigned long milliseconds) {
    unsigned long now = millis();
    stop_until = now + milliseconds;
}

/**
 * Set task finished
 */
void Task::finish() {
    is_terminated = true;
}

/**
 * Set task priority
 */
void Task::priority() {
    is_priority = true;    
}

/**
 * Remove priority of task
 */
void Task::normal() {
    is_priority = false;    
}

/**
 * What the task will do?
 */
void Task::run() {
    // Nothing to do...
    finish();
}

// Times Task
TimesTask::TimesTask(unsigned int _times) : Task() {
    times = _times;
    loops = 0;
}

void TimesTask::inc() {
    times++; 
}

void TimesTask::dec() {
    times--;
}

unsigned int TimesTask::remainder() {
    return times - loops; 
}

unsigned int TimesTask::currentLoop() {
    return loops;
}

void TimesTask::postRun() {
    Task::postRun();
    loops++;
    if (remainder() <= 0) {
        finish(); 
    }
}
