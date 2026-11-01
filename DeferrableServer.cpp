#include <bits/stdc++.h>
#include "DeferrableServer.h"

DeferrableServer::DeferrableServer(double period, double budget) {
    DeferrableServer::period = period;
    DeferrableServer::budget = budget;
    DeferrableServer::remaining = 0;
}

double DeferrableServer::getPeriod() const {
    return period;
}

double DeferrableServer::getBudget() const {
    return budget;
}

double DeferrableServer::getRemaining() const {
    return remaining;
}

void DeferrableServer::replenish() {
    remaining = budget;
}

void DeferrableServer::addAperiodic(AperiodicJob job) {
    jobs.push_back(job);
    stable_sort(jobs.begin(), jobs.end(), [](const AperiodicJob& a, const AperiodicJob& b) {
        return a.getReleaseTime() < b.getReleaseTime();
    });
}

void DeferrableServer::serve(double now, double finishTime, double slot) {
    double allowed = min(remaining, slot);
    for (AperiodicJob& job : jobs) {
        if (allowed <= 1e-9) {
            break;
        }

        if (job.isDone() || job.getReleaseTime() > now + 1e-9) {
            continue;
        }

        double used = min(allowed, job.getRemaining());
        job.execute(used);
        allowed -= used;
        remaining -= used; 
        if (job.isDone()) {
            job.setFinishTime(finishTime); 
        }
    }
}

vector<AperiodicJob>& DeferrableServer::getJobs() {
    return jobs;
}