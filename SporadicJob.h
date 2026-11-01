#ifndef SPORADIC_JOB_H
#define SPORADIC_JOB_H

#include <iostream>
#include <string>

using namespace std;

class SporadicJob {
    public:
        SporadicJob(string s_id, double r, double e, double d);
        string getId() const;
        double getReleaseTime() const;
        double getExecutionTime() const;
        double getDeadline() const;     
        double getRemaining() const;    
        void execute(double amount);    
        bool isDone() const;

    private:
        string id;
        double releaseTime;
        double execution_time;
        double deadline;
        double executed;              
};

#endif