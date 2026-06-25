#include <bits/stdc++.h>
using namespace std;


struct Task {
    string id;
    int burst;
    queue<string> mem_requests;
};


class CacheLevel {
public:
    int capacity;
    int latency;
    queue<string> q;
    unordered_set<string> s;

    CacheLevel(int cap, int lat) : capacity(cap), latency(lat) {}

    bool contains(string block) {
        return s.count(block);
    }

    void insert(string block) {
        if (s.count(block)) return;

        if (q.size() == capacity) {
            string evicted = q.front();
            q.pop();
            s.erase(evicted);
        }

        q.push(block);
        s.insert(block);
    }

    void print(string name) {
        cout << name << ": [";
        queue<string> temp = q;
        while (!temp.empty()) {
            cout << temp.front();
            temp.pop();
            if (!temp.empty()) cout << ", ";
        }
        cout << "]";
    }
};


class CacheSystem {
public:
    CacheLevel L1 = CacheLevel(32, 4);
    CacheLevel L2 = CacheLevel(128, 12);
    CacheLevel L3 = CacheLevel(512, 40);

    int RAM_latency = 200;
    int ram_accesses = 0;

    int access(string block) {
        // L1 HIT
        if (L1.contains(block)) {
            cout << ">> L1 HIT ";
            return L1.latency;
        }

        // L2 HIT
        if (L2.contains(block)) {
            cout << ">> L2 HIT ";
            L1.insert(block);
            return L2.latency;
        }

        // L3 HIT
        if (L3.contains(block)) {
            cout << ">> L3 HIT ";
            L2.insert(block);
            L1.insert(block);
            return L3.latency;
        }

        // RAM MISS
        cout << ">> MISS (RAM FETCH) ";
        ram_accesses++;

     
        L3.insert(block);
        L2.insert(block);
        L1.insert(block);

        return RAM_latency;
    }

    void print() {
        L1.print("L1"); cout << " ";
        L2.print("L2"); cout << " ";
        L3.print("L3"); cout << endl;
    }
};


vector<Task> parseInput(string filename) {
    ifstream file(filename);
    vector<Task> tasks;

    string word;
    while (file >> word) {
        if (word == "TASK") {
            Task t;
            file >> t.id >> word >> t.burst >> word;

            string mem;
            while (file >> mem) {
                if (mem == "TASK") {
                    file.seekg(-((int)mem.size()), ios_base::cur);
                    break;
                }
                t.mem_requests.push(mem);
            }
            tasks.push_back(t);
        }
    }

    return tasks;
}


class Scheduler {
public:
    int quantum = 2;

    void run(vector<Task> tasks) {
        CacheSystem cache;
        queue<Task> q;

        for (auto &t : tasks) q.push(t);

        int cycle = 0;
        int completed = 0;

        while (!q.empty()) {
            Task t = q.front(); q.pop();

            int time = min(quantum, t.burst);

            for (int i = 0; i < time; i++) {
                cycle++;

                if (!t.mem_requests.empty()) {
                    string block = t.mem_requests.front();
                    t.mem_requests.pop();

                    cout << "\nCycle " << cycle
                         << " - Running: " << t.id
                         << " Requesting: " << block << endl;

                    int latency = cache.access(block);
                    cout << "Latency: " << latency << endl;
                } else {
                    cout << "\nCycle " << cycle
                         << " - Running: " << t.id
                         << " (no memory request)" << endl;
                }

                cache.print();
                t.burst--;
            }

            if (t.burst > 0) {
                q.push(t);
            } else {
                completed++;
            }
        }

        cout << "\n=== Final Results ===" << endl;
        cout << "Total Cycles: " << cycle << endl;
        cout << "Tasks Completed: " << completed << endl;
        cout << "Scheduler: Round Robin (q=2)" << endl;
        cout << "RAM Accesses: " << cache.ram_accesses << endl;
    }
};


int main() {
    vector<Task> tasks = parseInput("input.txt");

    if (tasks.empty()) {
        cout << "ERROR: No tasks loaded. Check input.txt\n";
        return 0;
    }

    Scheduler scheduler;
    scheduler.run(tasks);

    return 0;
}
