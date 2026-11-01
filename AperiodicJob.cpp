#include <iostream>
#include <string>

#include "AperiodicJob.h"

using namespace std;

AperiodicJob::AperiodicJob(string s_id, double r, double e) {
    id = s_id;
    releaseTime = r;
    execution_time = e;
    executed = 0;
    finishTime = -1;
}

string AperiodicJob::getId() const {
    return id;
}

double AperiodicJob::getReleaseTime() const {
    return releaseTime;
}

double AperiodicJob::getRemaining() const {
    return execution_time - executed;
}

double AperiodicJob::getFinishTime() const {
    return finishTime;
}

void AperiodicJob::execute(double amount) {
    executed += amount;
}

void AperiodicJob::setFinishTime(double t) {
    finishTime = t;
}

bool AperiodicJob::isDone() const {
    return getRemaining() <= 1e-9;
}