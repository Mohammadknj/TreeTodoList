#include <iostream>
#include <string>
#include <vector>
using namespace std;
int currentYear, currentMonth, currentDay;
int monthsDays[] = {0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};
int leftDaysCalculator(int year, int month, int day) {
    int result = 0;
    if (year > currentYear + 1) {
        result += (year - (currentYear + 1)) * 365;
        year = currentYear + 1;
    }
    while (year > currentYear || month > currentMonth) {
        result += monthsDays[--month];
        if (month == 0) {
            month = 13;
            year--;
        }
    }
    result += day - currentDay;
    return result;
}
int isTaskOrSubtask = 1;
bool checkCorrectAddingCommand(int i, string str) {
    if (isTaskOrSubtask == 1) {
        string s = "add task ";
        for (int j = 0; j < i; ++j) {
            if (s[j] != str[j]) {
                isTaskOrSubtask = 0;
                return false;
            }
        }
    } else {
        string s = "add subtask ";
        for (int j = 0; j < i; ++j) {
            if (s[j] != str[j]) {
                isTaskOrSubtask = 0;
                return false;
            }
        }
    }
    return true;
}
vector<string> addTaskExtractInformations(string str) {
    vector<string> result;
    string word;
    int i = 9;
    if (isTaskOrSubtask == 2) {
        i += 3;
    }
    if (!checkCorrectAddingCommand(i, str)) {
        cout << "Error! You Entered a wrong command\n";
        return {};
    }
    while (i < int(str.length())) {
        if (str[i] == ',') {
            result.push_back(word);
            word.clear();
            i += 2;
            continue;
        }
        word += str[i];
        i++;
    }
    result.push_back(word);
    return result;
}
class Task;

class Deadline {
public:
    int year, month, day, leftDays, hour, minute;
    string date;
    void makeDeadline(string str) {
        int arr[] = {0, 0, 0};
        int i = 0, j = 0;
        while (str[i] != ' ') {
            if (str[i] == '/') {
                i++;
                arr[j] /= 10;
                j++;
                continue;
            }
            arr[j] += str[i] - 48;
            arr[j] *= 10;
            i++;
        }
        arr[2] /= 10;
        i++;
        j = 0;
        while (str[i] != ':') {
            j += str[i] - 48;
            j *= 10;
            i++;
        }
        hour = j / 10;
        i++;
        minute = (str[i++] - 48) * 10 + (str[i] - 48);
        year = arr[0];
        month = arr[1];
        day = arr[2];
        leftDays = leftDaysCalculator(year, month, day);
        string min = minute < 10 ? "0" + to_string(minute) : to_string(minute);
        string hr = hour < 10 ? "0" + to_string(hour) : to_string(hour);
        hr += ":";
        date = to_string(year) + '/' + to_string(month) + '/' + to_string(day) +
               " " + hr + min;
    }
};
class Subtask {
public:
    string name, description, status, taskName;
    Subtask *next;
    Subtask *previous;
    Task *parent;
    Deadline deadline;
    Subtask() {
        string str;
        cout << "Enter task: ";
        cin.ignore();
        getline(cin, str);
        vector<string> ss = addTaskExtractInformations(str);
        if (isTaskOrSubtask == 0)
            return;
        name = ss[0];
        description = ss[1];
        status = ss[3];
        taskName = ss[4];
        next = nullptr;
        previous = nullptr;
        parent = nullptr;
        deadline.makeDeadline(ss[2]);
    }
};
class Linklist {
public:
    Subtask *head;
    Subtask *tail;
    int size;
    Linklist() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
    void insert(Subtask *sub, Task *t) {
        sub->parent = t;
        sub->previous = head;
        if (head == nullptr) {
            head = sub;
            tail = sub;
        } else {
            head->next = sub;
            head = sub;
        }
        size++;
    }
    void delNode(Subtask *subtask) {
        if (subtask == nullptr)
            return;
        if (subtask->previous != nullptr)
            subtask->previous->next = subtask->next;
        else
            tail = subtask->next;
        if (subtask->next != nullptr)
            subtask->next->previous = subtask->previous;
        else
            head = subtask->previous;
        size--;
        delete subtask;
    }
};
class Task {
public:
    string name, description, status;
    Task *parent;
    Task *rightSibling;
    Subtask *leftChild;
    Linklist subtasks;
    Deadline deadline;
    Task() {
        parent = nullptr;
        rightSibling = nullptr;
        leftChild = nullptr;
        string str;
        cout << "Enter task: ";
        cin.ignore();
        getline(cin, str);
        vector<string> ss = addTaskExtractInformations(str);
        if (isTaskOrSubtask == 0)
            return;
        name = ss[0];
        description = ss[1];
        status = ss[3];
        deadline.makeDeadline(ss[2]);
    }
    void addSubtask(Subtask *sub, Task *t) {
        subtasks.insert(sub, t);
        if (t->subtasks.size == 1)
            t->leftChild = sub;
        sub->parent = t;
    }
};
Task *closerTask(Task *T, Task *T1) {
    if (T->deadline.leftDays == T1->deadline.leftDays) {
        if (T->deadline.hour > T1->deadline.hour)
            return T1;
        else if (T->deadline.hour < T1->deadline.hour)
            return T;
        else
            return T->deadline.minute > T1->deadline.minute ? T1 : T;
    }
    if (T->deadline.leftDays > T1->deadline.leftDays)
        return T1;
    return T;
}
class TreeRoot {
public:
    Task *itself;
    Task *leftChild;
    int size;
    TreeRoot() {
        itself = nullptr;
        leftChild = nullptr;
        size = 0;
    }
    void addTask(Task *t) {
        t->parent = itself;
        size++;
        if (leftChild == nullptr) {
            leftChild = t;
        } else {
            // Task *x = leftChild;
            // while (x->rightSibling != nullptr) {
            //     x = x->rightSibling;
            // }
            // x->rightSibling = t;
            SortTasks(t);
        }

    }
    void SortTasks(Task *T) {
        Task *t = leftChild,*t1;
        while (t != nullptr) {
            if(T == closerTask(t,T)){
                insert(t,T);
                // if(t==leftChild) leftChild = T;
                return;
            }
            t1 = t;
            t = t->rightSibling;
        }
        t1->rightSibling = T;
        //     Tn = T->rightSibling;
        //     while (Tn != nullptr) {
        //         t = closerTask(T, Tn);
        //         if (t == Tn)
        //             switching(T, Tn);
        //         Tn = Tn->rightSibling;
        //     }
        //     T = T->rightSibling;
        // }

    }
    void insert(Task*T,Task*Tn){
        Tn->rightSibling = T;
        if(T==leftChild){
            leftChild = Tn;
            return;
        }
        Task*t=leftChild;
        while(t->rightSibling!=T)
            t=t->rightSibling;
        t->rightSibling=Tn;
        // t=Tn;
        // while(t->rightSibling!=Tn) t=t->rightSibling;
        // t->rightSibling=nullptr;
    }
    // void switching(Task *T, Task *Tn) {
    //     Task *Tpre, *Trs = T->rightSibling, *Tnpre, *Tnrs = Tn->rightSibling;
    //     Task *t = leftChild;
    //     if (T == leftChild)
    //         Tpre = nullptr;
    //     else
    //         while (t->rightSibling != T)
    //             t = t->rightSibling;
    //     if (Tpre != nullptr)
    //         Tpre = t;
    //     while (t->rightSibling != Tn)
    //         t = t->rightSibling;
    //     Tnpre = t;
    //     if (Tpre != nullptr)
    //         Tpre->rightSibling = Tn;
    //     Tnpre->rightSibling = T;
    //     // T->rightSibling = Tnrs;
    //     // Tn->rightSibling = Trs;
    //     Task* temp = T->rightSibling;
    //     T->rightSibling = Tn->rightSibling;
    //     Tn->rightSibling = temp;
    // }
};
void showCloserTask(Task *T) {
    cout << "Task:\n    Name: " << T->name << ", Description: " << T->description
         << ", Status: " << T->status << endl;
    cout << "    It's " << T->deadline.leftDays
         << " days left. Deadline: " << T->deadline.date << endl;
    if (T->subtasks.size == 0)
        return;
    cout << "Subtasks:\n";
    Subtask *sub = T->subtasks.tail;
    while (sub != nullptr) {
        cout << "    " << sub->name << ", " << sub->description << ", "
             << sub->status << ", " << sub->deadline.date << endl;
        sub = sub->next;
    }
}
void menu(TreeRoot *Root) {
    int choice;
    do {
        cout << "menu:" << endl;
        cout << "1.Add task" << endl;
        cout << "2.Add subtask" << endl;
        cout << "3.Change subtask status" << endl;
        cout << "4.Show task with higher priority" << endl;
        // cout << "5.sabte etelaat dar file matni" << endl;
        // cout << "6.bazyabie etelaat az file matni" << endl;
        cout << "0.Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            isTaskOrSubtask = 1;
            Task *t = new Task;
            if (isTaskOrSubtask == 1) {
                Root->addTask(t);
                cout << "Task successfully added :)\n";
            } else {
                delete t;
                t = nullptr;
            }
            break;
        }
        case 2: {
            isTaskOrSubtask = 2;
            Subtask *sub = new Subtask();
            Task *T = Root->leftChild;
            string currentTaskName = sub->taskName;
            while (T != nullptr && T->name != currentTaskName)
                T = T->rightSibling;
            if (T != nullptr) {
                T->addSubtask(sub, T);
                cout << "Subtask successfully added :)\n";
            } else {
                if (isTaskOrSubtask == 2)
                    cout << "Oops! Task wasn't found\n";
                delete sub;
                sub = nullptr;
            }
            break;
        }
        // case 3:
        //    bazyabi_etelaat();
        //    break;
        case 4:
            showCloserTask(Root->leftChild);
            break;
        // case 5:
        //    sabt_dar_file();
        //    break;
        // case 6:
        //    bazyabi_az_file();
        //    break;
        case 0:
            break;
        default:
            cout << "Invalid choice! try again.\n";
        }
    } while (choice != 0);
}
int main() {
    cout << "Hi! At first please tell me today's date: ";
    cin >> currentYear >> currentMonth >> currentDay;
    cout << "First add root task\n";
    TreeRoot R;
    R.itself = new Task;
    menu(&R);
    return 0;
}
