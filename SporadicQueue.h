#ifndef SPORADIC_QUEUE_H
#define SPORADIC_QUEUE_H

#include <iostream>
#include <vector>

#include "SporadicJob.h"
#include "Frame.h"

using namespace std;

class SporadicQueue {
    public:
        bool acceptanceTest(SporadicJob job, int currentFrame, vector<Frame>& frames, int frameSize);
        bool empty();
        SporadicJob& front();
        void popFront();

    private:
        vector<SporadicJob> jobs; 
        double totalSlack(int from, int to, vector<Frame>& frames);
};

#endif