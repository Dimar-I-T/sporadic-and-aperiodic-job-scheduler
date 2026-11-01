#ifndef CYCLIC_SCHEDULER_H
#define CYCLIC_SCHEDULER_H

#include <iostream>
#include <vector>

#include "Task.h"
#include "Frame.h"
#include "SporadicJob.h"      
#include "AperiodicJob.h"     
#include "SporadicQueue.h"    
#include "DeferrableServer.h"

class CyclicScheduler {
    public:
         CyclicScheduler(vector<Task> tasks, double serverPeriod = 0, double serverBudget = 0);
        int getHyperPeriod();
        int getFrameSize();
        bool isFeasible();
        vector<Task> getTasks();
        vector<Frame> getFrames();
        DeferrableServer& getServer();
        void simulate(vector<SporadicJob> sporadicJobs, vector<AperiodicJob> aperiodicJobs);
    
    private:
        vector<Task> tasks;
        vector<Frame> frames;
        int hyperPeriod;
        int frameSize;
        bool feasible;
        DeferrableServer server;
        int serverFrames; 
        int LCM(int a, int b);
        int getHyperPeriodValue(vector<Task>& tasks);
        int getMaxPeriod(vector<Task>& tasks);
        vector<int> getCandidates(int hyperPeriod, int maxPeriod);
        bool isValidInequalityOne(int f, Task task);
        bool isValidInequalityAll(int f, vector<Task>& tasks);
        int bestCandidate(vector<int> candidates, vector<Task>& tasks);
        void setFrameValues(int frameSize, int hyperPeriod);
        bool arrangeTasksPerHyperPeriod(int hyperPeriod, vector<Task>& tasks, vector<Frame>& frames);
        bool allocateServerBudget(double serverPeriod, double requestedBudget);
};

#endif