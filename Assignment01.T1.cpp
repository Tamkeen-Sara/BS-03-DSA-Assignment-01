#include <iostream>
#include <string>
using namespace std;

class Process {
public:
    int process_id;
    int execution_time;
    int remaining_time;
    Process* next;

    Process(int id, int time) : 
    process_id(id), execution_time(time), remaining_time(time), next(nullptr) {}
};

class ProcessScheduler {
private:
    Process* head;
    const int cpu_time_per_cycle = 3;

public:
    ProcessScheduler() : 
    head(nullptr) {

    }

    void addProcess(int process_id, int execution_time) {
        Process* newProcess = new Process(process_id, execution_time);
        if (!head) {
            head = newProcess;
            newProcess->next = head;
        } else {
            Process* current = head;
            while (current->next != head) {
                current = current->next;
            }
            current->next = newProcess;
            newProcess->next = head;
        }
    }

    void runCycle(int& cycleCount) {
        if (!head) {
            return;
        }

        cout << "Cycle " << cycleCount << ":" <<  endl;
        cout << "Before Execution: ";
        
        displayState();

        Process* current = head;
        Process* prev = nullptr;
        Process* completedHead = nullptr;

        do {
            current->remaining_time -= cpu_time_per_cycle;
            if (current->remaining_time <= 0) {
                Process* completed = new Process(current->process_id, 0);
                completed->next = completedHead;
                completedHead = completed;

                if (current->next == current) { 
                    delete current;
                    head = nullptr;
                } else {
                    if (prev) {
                        prev->next = current->next;
                    } else {
                        head = current->next;
                    }
                    Process* temp = current;
                    current = current->next;
                    delete temp;
                    continue;
                }
            } else {
                prev = current;
            }

            if (head == nullptr) break;
            current = current->next;
        } while (current != head);

        cout << "After Completion Of Cycle" << cycleCount << " :  ";
        displayState();
        
        displayCompleted(completedHead);
        freeCompletedList(completedHead);
        
        cycleCount++;
        cout <<  endl;
    }

    void displayState() {
        if (!head) {
            cout << "No processes left in the system." <<  endl;
            return;
        }

        Process* current = head;
        do {
             cout << "P" << current->process_id << " (Remaining Time: " << current->remaining_time << ") ";
            current = current->next;
        } while (current != head);
         cout <<  endl;
    }

    void displayCompleted(Process* completed) {
        if (!completed) {
            return;
        }
         cout << "\n\tCompleted Processes: ";
        Process* current = completed;
        while (current) {
             cout << "P" << current->process_id;
            if (current->next)  cout << ", ";
            current = current->next;
        }
         cout <<  endl;
    }

    void freeCompletedList(Process* completedHead) {
        while (completedHead) {
            Process* temp = completedHead;
            completedHead = completedHead->next;
            delete temp;
        }
    }

    void simulate() {
        int cycleCount = 1;
        while (head) {
            runCycle(cycleCount);
        }
    }

    ~ProcessScheduler() {
        if (!head) return;
        Process* current = head->next;
        while (current != head) {
            Process* temp = current;
            current = current->next;
            delete temp;
        }
        delete head;
    }
};

int main() {
    ProcessScheduler scheduler;
    cout << "\n\t\t\tWelcome To Process Scheduling System!" << endl;
    scheduler.addProcess(1, 10);
    scheduler.addProcess(2, 5);
    scheduler.addProcess(3, 8);

    cout << "\n\tInitial simulation:\n" << endl;
    scheduler.simulate();
    cout << "\t\t\tInitial Simulation Completed!" << endl;
    scheduler.addProcess(4, 9);
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    cout << "\n\tNew process P4 added!" << endl;
    cout << "\n\t2nd Simulation:\n" << endl;
    scheduler.simulate();
    cout << "\t\t\t2nd Simulation Completed!" << endl;
    cout << "\n\t\t\tThank You For Using Our System! Exiting...." << endl;
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;
   return 0;
}