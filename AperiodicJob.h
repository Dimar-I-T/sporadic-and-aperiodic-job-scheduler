#ifndef APERIODIC_JOB_H
#define APERIODIC_JOB_H

#include <iostream>
#include <string>

using namespace std;

class AperiodicJob {
    public:
        AperiodicJob(string s_id, double r, double e);
        string getId() const;
        double getReleaseTime() const;
        double getRemaining() const;
        double getFinishTime() const;
        void execute(double amount);
        void setFinishTime(double t);
        bool isDone() const;

    private:
        string id;
        double releaseTime;
        double execution_time;
        double executed;
        double finishTime; 
};

#endif