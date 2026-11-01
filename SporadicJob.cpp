#include <iostream>
#include <string>

#include "SporadicJob.h"

using namespace std;

SporadicJob::SporadicJob(string s_id, double r, double e, double d) {
    id = s_id;
    releaseTime = r;
    execution_time = e;
    deadline = d;
    executed = 0;
}

string SporadicJob::getId() const {
    return id;
}

double SporadicJob::getReleaseTime() const {
    return releaseTime;
}

double SporadicJob::getExecutionTime() const {
    return execution_time;
}

double SporadicJob::getDeadline() const {
    return deadline;
}

double SporadicJob::getRemaining() const {
    return execution_time - executed;
}

void SporadicJob::execute(double amount) {
    executed += amount;
}

bool SporadicJob::isDone() const {
    return getRemaining() <= 1e-9;
}