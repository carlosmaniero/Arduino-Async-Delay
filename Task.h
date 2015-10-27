#ifndef TASK_H
#define TASK_H


class Task {
    private:
        bool is_ready;
        bool is_terminated;
        bool is_priority;
        unsigned long interval;
        unsigned long last_execution;
        unsigned long times;
        unsigned long stop_until;

    public:
        Task();

        virtual bool isReady();
        virtual bool isTerminated();
        virtual bool isInterval();
        virtual bool isPriority();

        virtual unsigned long getInterval();

        virtual void setReady(bool value);
        virtual void setInterval(unsigned long value);

        virtual void ready();
        virtual void wait();
        virtual void wait(unsigned long milliseconds);
        virtual void finish();
        virtual void priority();
        virtual void normal();

        virtual void run();
        virtual void postRun();
};


class TimesTask : public Task {
    private:
        unsigned int times;
        unsigned int loops;
    public:
        TimesTask(unsigned int _times);
        void inc();
        void dec();
        unsigned int currentLoop();
        unsigned int remainder();
        virtual void postRun();
};
#endif
