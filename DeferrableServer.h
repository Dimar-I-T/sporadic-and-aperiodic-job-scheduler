#ifndef DEFERRABLE_SERVER_H
#define DEFERRABLE_SERVER_H

#include <iostream>
#include <vector>

#include "AperiodicJob.h"

using namespace std;

class DeferrableServer {
    public:
        DeferrableServer(double period = 0, double budget = 0);
        double getPeriod() const;
        double getBudget() const;
        double getRemaining() const;
        void replenish();
        void addAperiodic(AperiodicJob job);
        void serve(double now, double finishTime, double slot);
        vector<AperiodicJob>& getJobs();

    private:
        double period;
        double budget;    
        double remaining; 
        vector<AperiodicJob> jobs; 
};

#endif