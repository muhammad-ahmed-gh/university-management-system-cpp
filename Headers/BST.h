#ifndef BST_H
#define BST_H
#include <fstream>
#include <vector>
#include "SLL.h"
#include "Log.h"
#include "Queue.h"
using namespace std;

vector<int> getPrerequisite(string courseName)
{
    vector<int> prerequisitesList;

    ifstream prereqFile("Data/Prerequisites.txt");
    if (!prereqFile.is_open())
    {
        cerr << "Error opening file" << endl;
        PrintLog("Error opening file\n");
        return prerequisitesList;
    }

    string line;
    while (getline(prereqFile, line))
    {
        if (line == courseName)
        {
            string prereq;
            while (getline(prereqFile, prereq))
            {
                if (prereq == "")
                {
                    prereqFile.close();
                    return prerequisitesList;
                }
                else if (stoi(prereq) > 0)
                    prerequisitesList.push_back(stoi(prereq));
            }
        }
    }
    return prerequisitesList;
}

class BSTNode
{
public:
    int courseId;
    string courseName;
    int courseCredits;
    string courseInstructor;
    vector<int> prerequisites;
    studentNode *registeredStudents;
    BSTNode *left;
    BSTNode *right;
    int remainingSeats;
    Queue<int> waitlist;

    BSTNode()
    {
        courseId = courseCredits = 0;
        courseName = courseInstructor = "Unknown";
        registeredStudents = NULL;
        left = right = NULL;
        remainingSeats = 3;
    }

    BSTNode(int id, string name, int credits, string instructor, int seats, vector<int> prereq)
    {
        courseId = id;
        courseName = name;
        courseCredits = credits;
        courseInstructor = instructor;
        registeredStudents = NULL;
        left = right = NULL;
        remainingSeats = seats;
        prerequisites = prereq;
    }

    BSTNode(string info[]) // {CourseName, CourseId, CourseCredits, CourseInstructor, CourseSeats, Prerequisites}
    {
        courseName = info[0];
        courseId = stoi(info[1]);
        courseCredits = stoi(info[2]);
        courseInstructor = info[3];
        remainingSeats = stoi(info[4]);
        prerequisites = getPrerequisite(info[0]);
        registeredStudents = NULL;
        left = right = NULL;
    }

    BSTNode(int id, string name, int credits, string instructor)
    {
        courseId = id;
        courseName = name;
        courseCredits = credits;
        courseInstructor = instructor;
        registeredStudents = NULL;
        left = right = NULL;
        remainingSeats = 3;
    }

    void showData()
    {
        cout << "Course Name: " << courseName << endl;
        cout << "Course Id: " << courseId << endl;
        cout << "Course Credits: " << courseCredits << endl;
        cout << "Course Instructor: " << courseInstructor << endl;
        cout << "Course Remaining Seats: " << remainingSeats << endl;
        cout << "Course Name: " << courseName << endl;
    }
};

class courseRecords
{
private:
    BSTNode *root;

    BSTNode *addCourse(BSTNode *current, BSTNode *Node)
    {
        if (current == NULL)
        {
            cout << "The course was added successfully.\n\n";
            PrintLog("A course was added successfully\n");
            return Node;
        }

        else if (Node->courseId < current->courseId)
            current->left = addCourse(current->left, Node);

        else if (Node->courseId > current->courseId)
            current->right = addCourse(current->right, Node);

        else
        {
            cout << "There is already a course with this ID.\n\n";
            PrintLog("Trying to register an existing course\n");
        }
        return current;
    }

    void removeCourseFromStudents(int courseId, studentRecordsSLL &StudentRecords)
    {
        studentNode *current = StudentRecords.head;
        while (current)
        {
            current->getValue().enrollmentHistory->drop(courseId);
            current = current->getNext();
        }
    }

    void removeCourseFromPrerequisites(int courseId)
    {
        removeCourseFromPrerequisites(root, courseId);
    }

    void removeCourseFromPrerequisites(BSTNode *node, int courseId)
    {
        if (!node)
            return;

        // Remove the course from the prerequisites list
        node->prerequisites.erase(
            remove(node->prerequisites.begin(), node->prerequisites.end(), courseId),
            node->prerequisites.end()
        );

        removeCourseFromPrerequisites(node->left, courseId);
        removeCourseFromPrerequisites(node->right, courseId);
    }

    BSTNode *removeCourse(BSTNode *node, int courseId, studentRecordsSLL &StudentRecords)
    {
        if (!node)
        {
            cout << "Course Not found." << endl;
            PrintLog("Trying to remove a non-existing course\n");
            return NULL;
        }
        if (courseId < node->courseId)
            node->left = removeCourse(node->left, courseId, StudentRecords);

        else if (courseId > node->courseId)
            node->right = removeCourse(node->right, courseId, StudentRecords);

        else // found
        {
            // Remove the course from all students' enrollment histories
            removeCourseFromStudents(courseId, StudentRecords);

            // Remove the course from the prerequisites list of all other courses
            removeCourseFromPrerequisites(courseId);

            if (!node->left && !node->right)
            {
                delete node;
                return NULL;
            }
            else if (!node->left)
            {
                BSTNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                BSTNode *temp = node->left;
                delete node;
                return temp;
            }
            else
            {
                BSTNode *tempParent = node;
                BSTNode *temp = node->right;
                while (temp->left)
                {
                    tempParent = temp;
                    temp = temp->left;
                }
                if (tempParent != node)
                    tempParent->left = temp->right;
                else
                    tempParent->right = temp->right;

                node->courseId = temp->courseId;
                node->courseName = temp->courseName;
                node->courseCredits = temp->courseCredits;
                node->courseInstructor = temp->courseInstructor;
                node->prerequisites = temp->prerequisites;
                node->registeredStudents = temp->registeredStudents;
                node->remainingSeats = temp->remainingSeats;
                delete temp;
            }

            cout << "Course " << courseId << " has been removed." << endl;
            PrintLog("A course was removed\n");
        }
        return node;
    }

    void traverse(BSTNode *node)
    {
        if (!node)
            return;
        traverse(node->left);

        cout << "Course name: "       << node->courseName << endl;
        cout << "Course ID: "         << node->courseId << endl;
        cout << "Course credits: "    << node->courseCredits << endl;
        cout << "Course instructor: " << node->courseInstructor << endl;
        cout << "Remaining seats: "   << node->remainingSeats << endl;
        cout << "Prerequisites:";
        if(node->prerequisites.empty())
            cout << " No prerequisites";
        else
            for (int prereq : node->prerequisites)
                cout << ' ' << prereq;

        cout << "\nRegistered Students:";
        studentNode *current = node->registeredStudents;
        if(!current)
            cout << " No registered students";
        else
        {
            while (current)
            {
                cout << ' ' << current->getValue().getStudentId();
                current = current->getNext();
            }
        }
        cout << endl<< endl;

        traverse(node->right);
    }

public:
    courseRecords()
    {
        root = NULL;
    }

    BSTNode *getRoot()
    {
        return root;
    }

    void DestroyTree(BSTNode *Node)
    {
        if (Node == NULL)
            return;

        DestroyTree(Node->left);
        DestroyTree(Node->right);
        delete Node;
    }

    ~courseRecords()
    {
        DestroyTree(root);
    }

    BSTNode *ReadCourse()
    {
        BSTNode *course = new BSTNode;
        cout << "Enter Course Id: ";
        cin >> course->courseId;
        cin.ignore();

        cout << "Enter Course Name: ";
        getline(cin, course->courseName);

        cout << "Enter Course Credits: ";
        cin >> course->courseCredits;
        cin.ignore();

        cout << "Enter Course Instructor: ";
        getline(cin, course->courseInstructor);

        cout << "Enter Course Seats: ";
        cin >> course->remainingSeats;
        cin.ignore();

        string choice = "N";
        do
        {
            int prerequisites;
            cout << "Enter Prerequisites: ";
            cin >> prerequisites;
            if (prerequisites == 0)
                break;

            if (findCourse(prerequisites))
            {
                course->prerequisites.push_back(prerequisites);
            }
            else
            {
                cout << "Prerequisite course ID " << prerequisites << " does not exist. Please enter a valid course ID." << endl;
            }
            cout << "Do You Want To Add More? (Y/N): ";
            cin >> choice;
        } while (choice != "N" && choice != "n");

        return course;
    }

    void addCourse(BSTNode *Node)
    {
        root = addCourse(root, Node);
    }

    bool registerStudent(int courseId, int studentId)
    {
        BSTNode *current = root;
        while (current)
        {
            if (courseId == current->courseId)
            {
                if (current->remainingSeats > 0)
                {
                    studentNode *newStudent = new studentNode();
                    newStudent->getValue().setStudentId(studentId);

                    studentNode *temp = current->registeredStudents;
                    current->registeredStudents = newStudent;
                    newStudent->setNext(temp);

                    current->remainingSeats--;
                    cout << "Student " << studentId << " was registered in course " << courseId << endl;
                    PrintLog("A student was registered in a course\n");
                    return true;
                }
                else
                {
                    current->waitlist.enqueue(studentId);
                    cout << "No remaining seats available for course " << courseId << ". Student " << studentId << " added to waitlist." << endl;
                    PrintLog("A student was added to the waitlist\n");
                    return false;
                }
            }
            else if (courseId < current->courseId)
                current = current->left;
            else
                current = current->right;
        }
        return false; // dummy
    }

    void dropCourse(int courseId, int studentId)
    {
        BSTNode *current = root;
        while (current)
        {
            if (courseId == current->courseId)
            {
                studentNode *p = current->registeredStudents;
                if (p->getValue().studentId == studentId) // checking the 1st node
                {
                    current->registeredStudents = p->getNext();
                    delete p;
                    cout << "Student " << studentId << " has dropped the course " << courseId << endl;
                    PrintLog("A student has dropped a course\n");
                    current->remainingSeats++;
                    return;
                }
                while (p->getNext())
                {
                    if (p->getNext()->getValue().studentId == studentId)
                    {
                        studentNode *temp = p->getNext();
                        p->setNext(p->getNext()->getNext());
                        delete temp;
                        cout << "Student " << studentId << " has dropped the course " << courseId << endl;
                        PrintLog("A student has dropped a course\n");
                        current->remainingSeats++;
                        return;
                    }
                    p = p->getNext();
                }
                return;
            }
            else if (courseId < current->courseId)
                current = current->left;
            else
                current = current->right;
        }
    }

    void removeCourse(int courseId, studentRecordsSLL &StudentRecords)
    {
        root = removeCourse(root, courseId, StudentRecords);
    }

    void displayCourses()
    {
        traverse(root);
    }

    BSTNode *findCourse(int courseId)
    {
        BSTNode *current = root;
        while (current)
        {
            if (courseId == current->courseId)
                return current;
            else if (courseId < current->courseId)
                current = current->left;
            else
                current = current->right;
        }
        return NULL;
    }

    void breadthFirstTraversal(BSTNode *root)
    {
        if (!root)
        {
            cout << "BST is empty. No data to display." << endl;
            return;
        }

        Queue<BSTNode *> queue;
        queue.enqueue(root);
        bool first = true;

        while (!queue.isEmpty())
        {
            BSTNode *current = queue.dequeue();
            if (!first)
                cout << " -> ";
            cout << current->courseId;
            first = false;

            if (current->left)
                queue.enqueue(current->left);
            if (current->right)
                queue.enqueue(current->right);
        }
        cout << endl;
    }

    void inOrderTraversal(BSTNode *node)
    {
        if (!node)
            return;

        static bool first = true;
        inOrderTraversal(node->left);
        if (!first)
            cout << " -> ";
        cout << node->courseId;
        first = false;
        inOrderTraversal(node->right);
    }

    void preOrderTraversal(BSTNode *node)
    {
        if (!node)
            return;

        static bool first = true;
        if (!first)
            cout << " -> ";
        cout << node->courseId;
        first = false;
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }

    void postOrderTraversal(BSTNode *node)
    {
        if (!node)
            return;

        static bool first = true;
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        if (!first)
            cout << " -> ";
        cout << node->courseId;
        first = false;
    }
};
#endif