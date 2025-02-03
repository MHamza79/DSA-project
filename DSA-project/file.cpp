#include <iostream>
#include <queue>
#include <stack>
#include <string>
using namespace std;

class CourseNode {
public:
    string courseName;
    CourseNode* left;
    CourseNode* right;

    CourseNode(string name) : courseName(name), left(nullptr), right(nullptr) {}
};

class CourseTree {
public:
    CourseNode* root;

    CourseTree() : root(nullptr) {}

    void insert(string courseName) {
        root = insertRec(root, courseName);
    }

    void display() {
        inOrder(root);
    }

private:
    CourseNode* insertRec(CourseNode* node, string courseName) {
        if (node == nullptr) {
            return new CourseNode(courseName);
        }
        if (courseName < node->courseName) {
            node->left = insertRec(node->left, courseName);
        } else if (courseName > node->courseName) {
            node->right = insertRec(node->right, courseName);
        }
        return node;
    }

    void inOrder(CourseNode* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << "- " << node->courseName << endl;
        inOrder(node->right);
    }
};

class RegistrationSystem {
    queue<string> waitlist;
    stack<pair<string, bool>> undoStack;
    int seatsAvailable;
    int coursesRegistered;
    const int MAX_SEATS = 1;
    const int MAX_COURSES = 6;

public:
    RegistrationSystem() : seatsAvailable(MAX_SEATS), coursesRegistered(0) {}

    void registerCourse(string student, string course) {
        if (coursesRegistered >= MAX_COURSES) {
            cout << "Student " << student << " cannot register for more than " << MAX_COURSES << " courses.\n";
            return;
        }
        if (seatsAvailable > 0) {
            seatsAvailable--;
            coursesRegistered++;
            undoStack.push({student, true});
            cout << "Student " << student << " registered for " << course << " successfully!\n";
        } else {
            waitlist.push(student);
            undoStack.push({student, false});
            cout << "No seats available for " << course << ". Student " << student << " added to the waitlist.\n";
        }
    }

    void dropCourse(string student, string course) {
        if (seatsAvailable < MAX_SEATS) {
            seatsAvailable++;
            coursesRegistered--;
            cout << "Student " << student << " dropped " << course << " successfully!\n";
            if (!waitlist.empty()) {
                string nextStudent = waitlist.front();
                waitlist.pop();
                seatsAvailable--;
                coursesRegistered++;
                undoStack.push({nextStudent, true});
                cout << "Student " << nextStudent << " has been enrolled from the waitlist for " << course << ".\n";
            }
        } else {
            cout << "Student " << student << " was not registered for " << course << "!\n";
        }
    }

    void undo(string course) {
        if (undoStack.empty()) {
            cout << "No actions to undo for " << course << "!\n";
            return;
        }
        auto lastAction = undoStack.top();
        undoStack.pop();
        string student = lastAction.first;
        bool wasRegistered = lastAction.second;
        if (wasRegistered) {
            seatsAvailable++;
            coursesRegistered--;
            cout << "Undo: Student " << student << " removed from " << course << ".\n";
            waitlist.push(student);
            cout << "Student " << student << " added back to the waitlist for " << course << ".\n";
        } else {
            queue<string> tempQueue;
            bool found = false;
            while (!waitlist.empty()) {
                if (waitlist.front() == student && !found) {
                    found = true;
                } else {
                    tempQueue.push(waitlist.front());
                }
                waitlist.pop();
            }
            waitlist = tempQueue;
            cout << "Undo: Student " << student << " removed from the waitlist for " << course << ".\n";
        }
    }

    void displayWaitlist(string course) {
        if (waitlist.empty()) {
            cout << "No students in the waitlist for " << course << ".\n";
        } else {
            cout << "Waitlist for " << course << ": ";
            queue<string> tempQueue = waitlist;
            while (!tempQueue.empty()) {
                cout << tempQueue.front() << " ";
                tempQueue.pop();
            }
            cout << endl;
        }
    }
};

int main() {
    CourseTree courseTree;
    courseTree.insert("CS101");
    courseTree.insert("CS102");
    courseTree.insert("CS201");
    courseTree.insert("CS202");

    RegistrationSystem regSystem;

    cout << "Welcome to the Course Registration System!\n";
    int choice;
    string student, course;

    do {
        cout << "\nMenu:\n";
        cout << "1. Display Course Tree\n";
        cout << "2. Register for a Course\n";
        cout << "3. Drop a Course\n";
        cout << "4. Undo Last Action\n";
        cout << "5. Display Waitlist\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            courseTree.display();
            break;
        case 2:
            cout << "Enter your name: ";
            cin >> student;
            cout << "Enter course code: ";
            cin >> course;
            regSystem.registerCourse(student, course);
            break;
        case 3:
            cout << "Enter your name: ";
            cin >> student;
            cout << "Enter course code: ";
            cin >> course;
            regSystem.dropCourse(student, course);
            break;
        case 4:
            cout << "Enter course code: ";
            cin >> course;
            regSystem.undo(course);
            break;
        case 5:
            cout << "Enter course code: ";
            cin >> course;
            regSystem.displayWaitlist(course);
            break;
        case 6:
            cout << "Exiting... Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}

