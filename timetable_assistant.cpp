
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std;

struct Task {
    string name;
    int hour, minute;
    int duration;
};

vector<Task> tasks;

void saveTasks(string filename = "tasks.txt") {
    ofstream f(filename);
    for (auto &t : tasks) {
        f << t.name << ";" << t.hour << ";" << t.minute << ";" << t.duration << "\n";
    }
    f.close();
}

void loadTasks(string filename = "tasks.txt") {
    tasks.clear();
    ifstream f(filename);
    string line;
    while (getline(f, line)) {
        stringstream ss(line);
        string n; int h, m, d;
        getline(ss, n, ';'); ss >> h; ss.ignore(); ss >> m; ss.ignore(); ss >> d;
        tasks.push_back({n, h, m, d});
    }
    f.close();
}

void addTask() {
    Task t;
    cout << "Task name: ";
    cin.ignore();
    getline(cin, t.name);
    cout << "Start hour (0-23): "; cin >> t.hour;
    cout << "Start minute (0-59): "; cin >> t.minute;
    cout << "Duration (in minutes): "; cin >> t.duration;
    tasks.push_back(t);
    cout << "Task added.\n";
}

void viewTasks() {
    if (tasks.empty()) {
        cout << "No tasks scheduled.\n"; return;
    }
    cout << "\n--- Scheduled Tasks ---\n";
    for (auto &t : tasks) {
        cout << setw(2) << setfill('0') << t.hour << ":" << setw(2) << t.minute
             << " - " << t.name << " (" << t.duration << " min)\n";
    }
}

void editTask() {
    int h, m;
    cout << "Enter task time to edit (HH MM): ";
    cin >> h >> m;
    for (auto &t : tasks) {
        if (t.hour == h && t.minute == m) {
            cout << "Editing " << t.name << ". Enter new details:\n";
            cin.ignore();
            getline(cin, t.name);
            cout << "Start hour: "; cin >> t.hour;
            cout << "Start min: "; cin >> t.minute;
            cout << "Duration: "; cin >> t.duration;
            cout << "Updated.\n";
            return;
        }
    }
    cout << "Task not found.\n";
}

void deleteTask() {
    int h, m;
    cout << "Enter task time to delete (HH MM): ";
    cin >> h >> m;
    for (size_t i = 0; i < tasks.size(); i++) {
        if (tasks[i].hour == h && tasks[i].minute == m) {
            cout << "Deleted: " << tasks[i].name << "\n";
            tasks.erase(tasks.begin() + i);
            return;
        }
    }
    cout << "Task not found.\n";
}

void nextTask() {
    time_t now = time(0);
    tm *lt = localtime(&now);
    int currMin = lt->tm_hour * 60 + lt->tm_min;
    int soon = 1e9;
    Task *nxt = nullptr;
    for (auto &t : tasks) {
        int tmin = t.hour * 60 + t.minute;
        if (tmin > currMin && tmin < soon) {
            soon = tmin;
            nxt = &t;
        }
    }
    if (nxt) {
        cout << "Next task at " << setw(2) << setfill('0') << nxt->hour << ":"
             << setw(2) << nxt->minute << " - " << nxt->name << "\n";
    } else {
        cout << "No upcoming task.\n";
    }
}

void remindMe() {
    string what;
    int mins;
    cout << "Remind me to: ";
    cin.ignore();
    getline(cin, what);
    cout << "How many minutes before? ";
    cin >> mins;
    for (auto &t : tasks) {
        if (t.name == what) {
            int remindAt = t.hour * 60 + t.minute - mins;
            cout << "Reminder set for " << remindAt / 60 << ":" << setw(2) << setfill('0') << (remindAt % 60) << "\n";
            return;
        }
    }
    cout << "Task not found.\n";
}

int main() {
    loadTasks();
    int ch;
    while (true) {
        cout << "\n1. Add Task\n2. View Tasks\n3. Edit Task\n4. Delete Task\n5. Next Task\n6. Remind Me\n7. Save & Exit\n> ";
        cin >> ch;
        if (ch == 1) addTask();
        else if (ch == 2) viewTasks();
        else if (ch == 3) editTask();
        else if (ch == 4) deleteTask();
        else if (ch == 5) nextTask();
        else if (ch == 6) remindMe();
        else if (ch == 7) {
            saveTasks();
            break;
        }
    }
    return 0;
}
