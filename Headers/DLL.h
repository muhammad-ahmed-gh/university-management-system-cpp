#ifndef DLL_H
#define DLL_H
#include <iostream>
#include <string>
#include "Log.h"
using namespace std;

class enrollmentRecord
{
public:
    int courseId;
    string courseName;
    int grade;
    enrollmentRecord *prev;
    enrollmentRecord *next;

    enrollmentRecord(int id, string name, int grd = -1) // Default grade to -1 (not assigned)
    {
        courseId = id;
        courseName = name;
        grade = grd;
        prev = next = NULL;
    }
};

class enrollmentHistoryDLL
{
private:
    enrollmentRecord *head;
    enrollmentRecord *tail;

public:
    enrollmentHistoryDLL()
    {
        head = tail = NULL;
    }

    void add(int courseId, string courseName, int grade = -1) // Default grade to -1 (not assigned)
    {
        enrollmentRecord *newRecord = new enrollmentRecord(courseId, courseName, grade);
        if (!head)
            head = tail = newRecord;
        else
        {
            tail->next = newRecord;
            newRecord->prev = tail;
            tail = newRecord;
        }
        PrintLog("Added course to enrollment history: " + courseName + " (ID: " + to_string(courseId) + ")\n");
    }

    void updateGrade(int courseId, int grade)
    {
        enrollmentRecord *current = head;
        while (current)
        {
            if (current->courseId == courseId)
            {
                current->grade = grade;
                PrintLog("Updated grade for course ID: " + to_string(courseId) + " to " + to_string(grade) + "\n");
                return;
            }
            current = current->next;
        }
    }

    void deleteNode(enrollmentRecord *Node)
    {
        if (!Node)
            return;

        if (Node == head)
        {
            head = head->next;
            if (head)
                head->prev = NULL;
            else
                tail = NULL;
        }
        else if (Node == tail)
        {
            tail = tail->prev;
            if (tail)
                tail->next = NULL;
        }
        else
        {
            Node->prev->next = Node->next;
            Node->next->prev = Node->prev;
        }
        delete Node;
    }

    void drop(int courseId)
    {
        enrollmentRecord *record = head;
        while (record)
        {
            if (record->courseId == courseId)
            {
                deleteNode(record);
                PrintLog("Dropped course from enrollment history: " + to_string(courseId) + "\n");
                return;
            }
            record = record->next;
        }
    }

    void display()
    {
        if (!head)
        {
            cout << "No enrollment history available." << endl;
            return;
        }

        cout << "Enrollment History:" << endl;
        enrollmentRecord *current = head;
        while (current)
        {
            cout << "Course ID: " << current->courseId << ", Course Name: " << current->courseName;
            if (current->grade != -1)
                cout << ", Grade: " << current->grade;
            cout << endl;
            current = current->next;
        }
    }

    bool contains(int courseId)
    {
        enrollmentRecord *current = head;
        while (current)
        {
            if (current->courseId == courseId)
                return true;
            current = current->next;
        }
        return false;
    }

    enrollmentRecord *getHead()
    {
        return head;
    }

    enrollmentRecord *getRecord(int courseId)
    {
        enrollmentRecord *p = head;
        while (p)
        {
            if (p->courseId == courseId)
                return p;
            p = p->next;
        }
        return NULL;
    }

    ~enrollmentHistoryDLL()
    {
        while (head)
        {
            enrollmentRecord *temp = head;
            head = head->next;
            delete temp;
        }
    }
};
#endif