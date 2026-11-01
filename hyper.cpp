#include <bits/stdc++.h>
using namespace std;

struct Task {
    int p;
    int d;
};

int LCM(int a, int b) {
    return a * b / __gcd(a, b);
}

int HyperPeriod(vector<Task> v) {
    int n = v.size();
    if (n == 1) {
        return v[0].p;
    }

    // 3, 6, 14, 20
    // kpk(3, 6, 14, 20) = kpk(kpk(3, 6), 14, 20) = kpk(kpk(kpk(3, 6), 14), 20) -> O(n)
    // kpk(a, b) = a * b / fpb(a, b)
    int lcm = LCM(v[0].p, v[1].p);
    for (int x = 2; x < n; x++) {
        lcm = LCM(lcm, v[x].p);
    }

    return lcm;
}

vector<int> getCandidates(int hyperPeriod, int maxPeriod) {
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

bool isValidInequalityOne(int f, Task task) {
    return (2 * f - __gcd(task.p, f) <= task.d);
}

bool isValidInequalityAll(int f, vector<Task> tasks) {
    bool valid = 1;
    for (Task task : tasks) {
        valid = isValidInequalityOne(f, task);
        if (!valid) {
            break;
        }
    }

    return valid;
}

int bestCandidate(vector<int> candidates, vector<Task> tasks) {
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

int main() {
    int n;
    cin >> n; 
    vector<Task> A(n);
    int maxPeriod = 0;
    for (int x = 0; x < n; x++) {
        int p, d;
        cin >> p >> d;
        Task task = {p, d};
        A[x] = task;
        maxPeriod = max(maxPeriod, A[x].p);
    }

    int H = HyperPeriod(A);
    cout << "hyper: " << H << "\n";
    cout << "candidates: ";
    vector<int> candidates = getCandidates(H, maxPeriod);
    sort(candidates.rbegin(), candidates.rend());
    for (int d : candidates) {
        cout << d << " ";
    }

    int f = bestCandidate(candidates, A);
    cout << "\nf = " << f;
}