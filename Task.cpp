#include <iostream>
#include <string>

#include "Task.h"

using namespace std;

Task::Task(string s_id, double p, double e, double d, double releaseTime) {
    id = s_id;
    period = p;
    execution_time = e;
    deadline = d;
    Task::releaseTime = releaseTime;
}

string Task::getId() {
    return id;
}

double Task::getPeriod() {
    return period;
}

double Task::getExecutionTime() {
    return execution_time;
}

double Task::getDeadline() {
    return deadline;
}

double Task::getReleaseTime() {
    return releaseTime;
}