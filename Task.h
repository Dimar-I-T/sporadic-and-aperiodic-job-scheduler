#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>

using namespace std;

class Task {
    public:
        Task(string s_id, double p, double e, double d, double releaseTime = 0);
        string getId();
        double getPeriod();
        double getExecutionTime();
        double getDeadline();
        double getReleaseTime();

    private:
        string id;
        double releaseTime;
        double period;
        double execution_time;
        double deadline;
};

#endif