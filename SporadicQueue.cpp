#include <bits/stdc++.h>
#include "SporadicQueue.h"

double SporadicQueue::totalSlack(int from, int to, vector<Frame>& frames) {
    double sum = 0;
    for (int g = from; g <= to; g++) {
        sum += frames[g % frames.size()].slack;
    }

    return sum;
}

bool SporadicQueue::acceptanceTest(SporadicJob job, int currentFrame, vector<Frame>& frames, int frameSize) {
    int pos = 0;
    while (pos < (int) jobs.size() && jobs[pos].getDeadline() <= job.getDeadline()) {
        pos++;
    }

    vector<SporadicJob> temp = jobs;
    temp.insert(temp.begin() + pos, job);

    double sumRemaining = 0;
    for (int i = 0; i < (int) temp.size(); i++) {
        sumRemaining += temp[i].getRemaining();
        int lastFrame = (int) floor(temp[i].getDeadline() / frameSize + 1e-9) - 1;
        double sigma = totalSlack(currentFrame, lastFrame, frames) - sumRemaining;
        if (sigma < -1e-9) {
            return 0; 
        }
    }

    jobs = temp; 
    return 1;
}

bool SporadicQueue::empty() {
    return jobs.empty();
}

SporadicJob& SporadicQueue::front() {
    return jobs.front();
}

void SporadicQueue::popFront() {
    jobs.erase(jobs.begin());
}