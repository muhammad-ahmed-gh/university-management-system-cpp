#ifndef SEARCHOPTIMIZATION_H
#define SEARCHOPTIMIZATION_H
#include <vector>
#include <algorithm>
#include "SLL.h"
#include "BST.h"
#include "Log.h"
using namespace std;

Student *searchStudentById(studentRecordsSLL &StudentRecords, int studentId)
{
    studentNode *current = StudentRecords.head;
    while (current)
    {
        PrintLog("Checking student with ID: " + to_string(current->getValue().studentId));
        if (current->getValue().studentId == studentId)
        {
            PrintLog("Student found with ID: " + to_string(studentId));
            return &current->getValue();
        }
        current = current->getNext();
    }
    PrintLog("Student not found with ID: " + to_string(studentId));
    return NULL;
}

BSTNode *searchCourseById(courseRecords &Courses, int courseId)
{
    BSTNode *current = Courses.getRoot();
    while (current)
    {
        PrintLog("Checking course with ID: " + to_string(current->courseId));
        if (courseId == current->courseId)
        {
            PrintLog("Course found with ID: " + to_string(courseId));
            return current;
        }
        else if (courseId < current->courseId)
        {
            PrintLog("Course ID is less than current node, moving left");
            current = current->left;
        }
        else
        {
            PrintLog("Course ID is greater than current node, moving right");
            current = current->right;
        }
    }
    PrintLog("Course not found with ID: " + to_string(courseId));
    return NULL;
}

#endif
