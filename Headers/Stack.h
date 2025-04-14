#ifndef STACK_H
#define STACK_H
#include "SLL.h"
#include "BST.h"
#include "Log.h"
#include <vector>
using namespace std;
const int stackMaxSize = 5;

class Stack
{
private:
    int elements[stackMaxSize];
    int stackTop;

public:
    Stack()
    {
        stackTop = -1;
    }

    bool isEmpty()
    {
        return stackTop == -1;
    }

    bool isFull()
    {
        return stackTop == (stackMaxSize - 1);
    }

    int peek()
    {
        if (isEmpty())
        {
            PrintLog("Attempted to peek value from empty stack\n");
            return -1;
        }
        return elements[stackTop];
    }

    void push(int value)
    {
        if (!isFull())
        {
            stackTop++;
            elements[stackTop] = value;
            PrintLog("Pushed value onto stack: " + to_string(value) + "\n");
        }
        else
        {
            PrintLog("Attempted to push value onto full stack: " + to_string(value) + "\n");
        }
    }

    int pop()
    {
        if (!isEmpty())
        {
            int value = peek();
            stackTop--;
            PrintLog("Popped value from stack: " + to_string(value) + "\n");
            return value;
        }
        PrintLog("Attempted to pop value from empty stack\n");
        return -1;
    }
};

bool validatePrerequisites(courseRecords &courses, studentRecordsSLL &students, int courseId, int studentId)
{
    BSTNode *course = courses.findCourse(courseId);
    studentNode *student = students.findStudent(studentId);

    Stack prereqStack;
    for (int prereq : course->prerequisites)
        prereqStack.push(prereq);

    vector<int> remainingPrereqs;
    while (!prereqStack.isEmpty())
    {
        int prereqId = prereqStack.pop();
        if (!student->getValue().enrollmentHistory->contains(prereqId))
            remainingPrereqs.push_back(prereqId);
        else
        {
            int grade = student->getValue().enrollmentHistory->getRecord(prereqId)->grade;
            if (grade == -1 || grade < 50)
                remainingPrereqs.push_back(prereqId);
        }
    }

    if (!remainingPrereqs.empty())
    {
        cout << "You cannot enroll in " << course->courseName << " yet.\n";
        cout << "You still need to complete the following prerequisites:\n";
        PrintLog("Trying to register a student in a not eligible course\n");
        for (int prereqId : remainingPrereqs)
        {
            cout << "- Course " << prereqId << endl;
        }
        return false;
    }
    return true;
}
#endif