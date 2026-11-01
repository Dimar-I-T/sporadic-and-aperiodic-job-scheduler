#include <bits/stdc++.h>
#include "Task.h"
#include "CyclicScheduler.h"
using namespace std;

void printInitialTasks(vector<Task>& tasks) {
    for (Task task : tasks) {
        cout << task.getId() << ":\n";
        cout << "Period: " << task.getPeriod() << "\n";
        cout << "Execution Time: " << task.getExecutionTime() << "\n";
        cout << "Deadline: " << task.getDeadline() << "\n\n";
    }
}

int main() {
    vector<Task> tasks;
    bool storingTasks = 1;
    int i = 1;
    while (storingTasks) {
        cout << "Task " << i << "\n";
        cout << "Enter period: ";
        double p;
        cin >> p;
        cout << "Enter execution time: ";
        double e;
        cin >> e;
        cout << "Enter deadline: ";
        double d;
        cin >> d;
        
        string task_id = "T#" + to_string(i);
        Task task = Task(task_id, p, e, d);
        tasks.push_back(task);

        cout << "Do you want to enter more tasks? (y/n): ";
        char q;
        cin >> q;
        switch (q) {
            case 'y':
                i++;
                // do nothing
                break;
            case 'n':
                cout << "\n";
                storingTasks = 0;
                break;
            default:
                cout << "Invalid input";
                break;
        }
    }

    printInitialTasks(tasks);
    cout << "Enter deferrable server period (0 = without server): ";
    double serverPeriod;
    cin >> serverPeriod;
    double serverBudget = 0;
    if (serverPeriod > 0) {
        cout << "Enter deferrable server budget: ";
        cin >> serverBudget;
    }

    CyclicScheduler cyclicScheduler = CyclicScheduler(tasks, serverPeriod);
    cout << "hyper period = " << cyclicScheduler.getHyperPeriod() << "\n";
    cout << "frame size = " << cyclicScheduler.getFrameSize() << "\n";
    cout << "server budget = " << cyclicScheduler.getServer().getBudget() << " per " << cyclicScheduler.getServer().getPeriod() << "\n";
    if (!cyclicScheduler.isFeasible()) {
        cout << "Schedule not feasible\n";
        return 0;
    }

    vector<Frame> frames = cyclicScheduler.getFrames();
    int fs = cyclicScheduler.getFrameSize();
    for (int f = 0; f < (int) frames.size(); f++) {
        cout << "Frame " << f << " [" << f * fs << ", " << (f + 1) * fs << "): ";
        for (string id : frames[f].jobs) {
            cout << id << " ";
        }
        cout << ": server = " << frames[f].serverSlot << ": slack = " << frames[f].slack << "\n";
    }

    vector<SporadicJob> sporadicJobs;
    int ns;
    cout << "\nNumber of sporadic jobs: ";
    cin >> ns;
    for (int s = 1; s <= ns; s++) {
        double r, e, d;
        cout << "S" << s << " release time: ";
        cin >> r;
        cout << "S" << s << " execution time: ";
        cin >> e;
        cout << "S" << s << " deadline: ";
        cin >> d;
        sporadicJobs.push_back(SporadicJob("S" + to_string(s), r, e, d));
    }

    vector<AperiodicJob> aperiodicJobs;
    int na;
    cout << "Number of aperiodic jobs: ";
    cin >> na;
    for (int a = 1; a <= na; a++) {
        double r, e;
        cout << "A" << a << " release time: ";
        cin >> r;
        cout << "A" << a << " execution time: ";
        cin >> e;
        aperiodicJobs.push_back(AperiodicJob("A" + to_string(a), r, e));
    }

    cout << "\n";
    cyclicScheduler.simulate(sporadicJobs, aperiodicJobs);
}