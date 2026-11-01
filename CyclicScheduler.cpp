#include <bits/stdc++.h>
#include "CyclicScheduler.h"

CyclicScheduler::CyclicScheduler(vector<Task> tasks, double serverPeriod, double serverBudget) {
    CyclicScheduler::tasks = tasks;
    int maxPeriod = CyclicScheduler::getMaxPeriod(tasks);
    CyclicScheduler::serverFrames = 1;
    CyclicScheduler::hyperPeriod = getHyperPeriodValue(tasks);
    vector<int> candidates = CyclicScheduler::getCandidates(CyclicScheduler::hyperPeriod, maxPeriod);
    sort(candidates.rbegin(), candidates.rend());
    CyclicScheduler::frameSize = CyclicScheduler::bestCandidate(candidates, tasks);
    if (CyclicScheduler::frameSize == 0) {
        CyclicScheduler::feasible = 0;
        return;
    }

    CyclicScheduler::setFrameValues(CyclicScheduler::frameSize, CyclicScheduler::hyperPeriod);
    CyclicScheduler::feasible = CyclicScheduler::arrangeTasksPerHyperPeriod(CyclicScheduler::hyperPeriod, tasks, CyclicScheduler::frames);

    if (CyclicScheduler::feasible) {
        CyclicScheduler::feasible = CyclicScheduler::allocateServerBudget(serverPeriod, serverBudget);
    }
}

bool CyclicScheduler::isFeasible() {
    return CyclicScheduler::feasible;
}

int CyclicScheduler::getHyperPeriod() {
    return CyclicScheduler::hyperPeriod;
}

int CyclicScheduler::getFrameSize() {
    return CyclicScheduler::frameSize;
}

vector<Task> CyclicScheduler::getTasks() {
    return CyclicScheduler::tasks;
}

vector<Frame> CyclicScheduler::getFrames() {
    return CyclicScheduler::frames;
}

int CyclicScheduler::LCM(int a, int b) {
    return a * b / __gcd(a, b);
}

int CyclicScheduler::getHyperPeriodValue(vector<Task>& tasks) {
    int n = tasks.size();
    if (n == 1) {
        return tasks[0].getPeriod();
    }

    // 3, 6, 14, 20
    // kpk(3, 6, 14, 20) = kpk(kpk(3, 6), 14, 20) = kpk(kpk(kpk(3, 6), 14), 20) -> O(n)
    // kpk(a, b) = a * b / fpb(a, b)
    int lcm = LCM(tasks[0].getPeriod(), tasks[1].getPeriod());
    for (int x = 2; x < n; x++) {
        lcm = LCM(lcm, tasks[x].getPeriod());
    }

    return lcm;
}

int CyclicScheduler::getMaxPeriod(vector<Task>& tasks) {
    int res = 0;
    for (Task task : tasks) {
        res = max(res, (int) task.getPeriod());
    }

    return res;
}   

vector<int> CyclicScheduler::getCandidates(int hyperPeriod, int maxPeriod) {
    unordered_map<int, bool> exists;
    vector<int> candidates;
    for (int x = 1; x * x <= hyperPeriod && x <= maxPeriod; x++) {
        int c1 = x;
        int c2 = hyperPeriod / x;
        if (hyperPeriod % x == 0) {
            if (!exists[c1]) {
                candidates.push_back(c1);
                exists[c1] = 1;
            }
    
            if (!exists[c2] && c2 <= maxPeriod) {
                candidates.push_back(c2);
                exists[c2] = 1;
            }
        }
    }

    return candidates;
}

bool CyclicScheduler::isValidInequalityOne(int f, Task task) {
    return (2 * f - __gcd((int) task.getPeriod(), f) <= (int) task.getDeadline()) && (f >= task.getExecutionTime());
}

bool CyclicScheduler::isValidInequalityAll(int f, vector<Task>& tasks) {
    bool valid = 1;
    for (Task task : tasks) {
        valid = isValidInequalityOne(f, task);
        if (!valid) {
            break;
        }
    }

    return valid;
}

int CyclicScheduler::bestCandidate(vector<int> candidates, vector<Task>& tasks) {
    bool valid = 0;
    int res = 0;
    for (int candidate : candidates) {
        valid = isValidInequalityAll(candidate, tasks);
        if (valid) {
            res = candidate;
            break;
        }
    }

    return res;
}

void CyclicScheduler::setFrameValues(int frameSize, int hyperPeriod) {
    int n = hyperPeriod / frameSize;
    vector<Frame> frames;
    frames.resize(n, Frame(frameSize));
    CyclicScheduler::frames = frames;
}

struct Job {
    string id;
    double release;
    double deadline; 
    double execution;
};

bool compareJob(const Job& a, const Job& b) {
    if (a.deadline != b.deadline) {
        return a.deadline < b.deadline;
    }

    return a.execution > b.execution;
}

bool CyclicScheduler::arrangeTasksPerHyperPeriod(int hyperPeriod, vector<Task>& tasks, vector<Frame>& frames) {
    int frameSize = frames[0].getLength();

    vector<Job> jobs;
    for (Task task : tasks) {
        int k = 0;
        double r = task.getReleaseTime();
        while (r < hyperPeriod) {
            Job job;
            job.id = task.getId() + "(" + to_string(k + 1) + ")";
            job.release = r;
            job.deadline = r + task.getDeadline();
            job.execution = task.getExecutionTime();
            jobs.push_back(job);

            k++;
            r = task.getReleaseTime() + k * task.getPeriod();
        }
    }

    sort(jobs.begin(), jobs.end(), compareJob);

    for (Job job : jobs) {
        bool placed = 0;
        for (int f = 0; f < (int) frames.size(); f++) {
            double start = f * frameSize;
            double end = start + frameSize;

            bool inWindow = (start >= job.release && end <= job.deadline);
            bool fit = (frames[f].slack + 1e-9 >= job.execution);

            if (inWindow && fit) {
                frames[f].slack -= job.execution;
                frames[f].jobs.push_back(job.id);
                placed = 1;
                break;
            }
        }

        if (!placed) {
            cout << "Job " << job.id << " tidak bisa dimasukkan ke frame manapun\n";
            return 0;
        }
    }

    return 1;
}

DeferrableServer& CyclicScheduler::getServer() {
    return CyclicScheduler::server;
}

bool CyclicScheduler::allocateServerBudget(double serverPeriod, double requestedBudget) {
    if (serverPeriod <= 0 || requestedBudget <= 0) {
        CyclicScheduler::server = DeferrableServer(); 
        return 1;
    }

    int k = (int) round(serverPeriod / frameSize);
    if (k < 1 || k * frameSize != serverPeriod || hyperPeriod % (k * frameSize) != 0) {
        cout << "Periode server harus kelipatan frame size dan membagi hyper period\n";
        return 0;
    }

    CyclicScheduler::serverFrames = k;
    int windows = frames.size() / k;

    double budget = requestedBudget;
    for (int w = 0; w < windows; w++) {
        double freeSlack = 0;
        for (int g = w * k; g < (w + 1) * k; g++) {
            freeSlack += frames[g].slack;
        }

        budget = min(budget, freeSlack);
    }

    if (budget < requestedBudget) {
        cout << "Budget diminta " << requestedBudget << " tidak cukup, dialokasikan " << budget << "\n";
    }

    for (int w = 0; w < windows; w++) {
        double freeSlack = 0;
        for (int g = w * k; g < (w + 1) * k; g++) {
            freeSlack += frames[g].slack;
        }

        for (int g = w * k; g < (w + 1) * k; g++) {
            double part = (freeSlack > 0) ? budget * frames[g].slack / freeSlack : 0;
            frames[g].serverSlot = part;
            frames[g].slack -= part;
        }
    }

    CyclicScheduler::server = DeferrableServer(serverPeriod, budget);
    return 1;
}

void CyclicScheduler::simulate(vector<SporadicJob> sporadicJobs, vector<AperiodicJob> aperiodicJobs) {
    sort(sporadicJobs.begin(), sporadicJobs.end(), [](const SporadicJob& a, const SporadicJob& b) {
        return a.getReleaseTime() < b.getReleaseTime();
    });

    for (AperiodicJob job : aperiodicJobs) {
        server.addAperiodic(job);
    }

    double horizon = hyperPeriod;
    for (SporadicJob& s : sporadicJobs) {
        horizon = max(horizon, s.getDeadline());
    }

    for (AperiodicJob& a : aperiodicJobs) {
        horizon = max(horizon, a.getReleaseTime() + hyperPeriod);
    }

    SporadicQueue queue;
    int next = 0; 
    int totalFrames = (int) ceil(horizon / frameSize);

    for (int f = 0; f < totalFrames; f++) {
        double t0 = f * frameSize;
        double t1 = t0 + frameSize;
        Frame& frame = frames[f % frames.size()];
        cout << "Frame " << f << " [" << t0 << ", " << t1 << ")\n";

        if (f % serverFrames == 0) {
            server.replenish();
        }

        while (next < (int) sporadicJobs.size() && sporadicJobs[next].getReleaseTime() <= t0 + 1e-9) {
            SporadicJob job = sporadicJobs[next++];
            bool ok = queue.acceptanceTest(job, f, frames, frameSize);
            cout << "  Acceptance test " << job.getId() << ": " << (ok ? "DITERIMA" : "DITOLAK") << "\n";
        }

        double before = server.getRemaining();
        server.serve(t0, t1, frame.serverSlot);
        if (before - server.getRemaining() > 1e-9) {
            cout << "  Server memakai " << before - server.getRemaining() << ", sisa budget " << server.getRemaining() << "\n";
        }

        double slackLeft = frame.slack;
        while (!queue.empty() && slackLeft > 1e-9) {
            SporadicJob& job = queue.front();
            double used = min(slackLeft, job.getRemaining());
            job.execute(used);
            slackLeft -= used;
            if (job.isDone()) {
                cout << "  " << job.getId() << " selesai paling lambat t = " << t1 << " (deadline " << job.getDeadline() << ")\n";
                queue.popFront();
            }
        }
    }

    cout << "\nAperiodic jobs:\n";
    for (AperiodicJob& job : server.getJobs()) {
        cout << job.getId() << ": ";
        if (job.isDone()) {
            cout << "selesai paling lambat t = " << job.getFinishTime() << "\n";
        } else {
            cout << "belum selesai, sisa " << job.getRemaining() << "\n";
        }
    }
}