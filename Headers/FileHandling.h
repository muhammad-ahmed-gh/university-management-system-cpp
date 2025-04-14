#ifndef FileHandling_H
#define FileHandling_H
#include <iostream>
#include <fstream>
#include <string>
#include "BST.h"
#include "SLL.h"
#include "Log.h"
#include "Queue.h"
using namespace std;

void loadCoursesFromFile(courseRecords &Courses)
{
    ifstream InfoFile("Data/Courses.txt");
    if (!InfoFile.is_open())
    {
        cerr << "Error opening file" << endl;
        PrintLog("Error opening courses file for reading\n");
        return;
    }

    string line;
    while (getline(InfoFile, line))
    {
        if (line == "")
        {
            string info[5]; // {CourseName, CourseId, CourseCredits, CourseInstructor, CourseSeats}
            for (short i = 0; i < 5; i++)
                getline(InfoFile, info[i]);

            BSTNode *Node = new BSTNode(info); // Prerequisites are added here
            Courses.addCourse(Node);
        }
    }
    InfoFile.close();
    PrintLog("Courses loaded from file\n");
}

void saveCoursesInfoToFile(BSTNode *Node) // Breadth first
{
    if (!Node)
        return;
    ofstream CoursesFile("Data/Courses.txt", ios::app);
    if (!CoursesFile.is_open())
    {
        cerr << "Error opening file for writing" << endl;
        PrintLog("Error opening courses file for writing\n");
        return;
    }

    Queue<BSTNode*> q;
    q.enqueue(Node);
    while(!q.isEmpty())
    {
        BSTNode* Node = q.frontValue();
        q.dequeue();

        CoursesFile << "\n";
        CoursesFile << Node->courseName << endl;
        CoursesFile << Node->courseId << endl;
        CoursesFile << Node->courseCredits << endl;
        CoursesFile << Node->courseInstructor << endl;
        CoursesFile << Node->remainingSeats << endl;

        if(Node->left != NULL)
            q.enqueue(Node->left);
        if(Node->right != NULL)
            q.enqueue(Node->right);
    }
}

void savePrereqToFile(BSTNode *Node)
{
    if (!Node)
        return;
    ofstream PrereqFile("Data/Prerequisites.txt", ios::app);
    savePrereqToFile(Node->left);
    if (!PrereqFile.is_open())
    {
        cerr << "Error opening file for writing" << endl;
        PrintLog("Error opening prerequisites file for writing\n");
        return;
    }

    PrereqFile << "\n";
    PrereqFile << Node->courseName << endl;

    if (Node->prerequisites.empty())
        PrereqFile << 0 << endl;
    else
        for (int prereq : Node->prerequisites)
            PrereqFile << prereq << endl;

    savePrereqToFile(Node->right);
}

void saveCoursesToFile(courseRecords &Courses)
{
    ofstream CoursesFile("Data/Courses.txt"); // clear
    CoursesFile.close();
    saveCoursesInfoToFile(Courses.getRoot());

    ofstream PrereqFile("Data/Prerequisites.txt"); // clear
    PrereqFile.close();
    savePrereqToFile(Courses.getRoot());
    PrintLog("Courses and prerequisites saved to file\n");
}

void loadStudentsFromFile(studentRecordsSLL &StudentRecords)
{
    ifstream inFile("Data/Students.txt");
    if (!inFile.is_open())
    {
        cerr << "Error opening file for reading" << endl;
        PrintLog("Error opening students file for reading\n");
        return;
    }

    while (!inFile.eof())
    {
        Student student;
        inFile >> student.studentId;
        inFile.ignore();
        getline(inFile, student.name);
        getline(inFile, student.email);
        getline(inFile, student.phone);
        getline(inFile, student.address);
        getline(inFile, student.password);

        if (student.studentId > 0) // Check if the student ID is valid
        {
            StudentRecords.addStudent(student);
        }
    }
    inFile.close();
    PrintLog("Students loaded from file\n");
}

void saveStudentsToFile(studentRecordsSLL &StudentRecords)
{
    ofstream outFile("Data/Students.txt");
    if (!outFile.is_open())
    {
        cerr << "Error opening file for writing" << endl;
        PrintLog("Error opening students file for writing\n");
        return;
    }

    studentNode *current = StudentRecords.head;
    while (current)
    {
        Student &student = current->getValue();
        outFile << "\n";
        outFile << student.studentId << endl;
        outFile << student.name << endl;
        outFile << student.email << endl;
        outFile << student.phone << endl;
        outFile << student.address << endl;
        outFile << student.password << endl;

        current = current->getNext();
    }
    outFile.close();
    PrintLog("Students saved to file\n");
}

void saveWaitlistHelper(BSTNode *node, ofstream &outFile)
{
    if (!node)
        return;

    saveWaitlistHelper(node->left, outFile);

    outFile << node->courseId << endl;
    Queue<int> tempQueue;
    while (!node->waitlist.isEmpty())
    {
        int studentId = node->waitlist.dequeue();
        outFile << studentId << endl;
        tempQueue.enqueue(studentId);
    }
    outFile << endl; // Separate each course's waitlist by an empty line

    while (!tempQueue.isEmpty())
    {
        node->waitlist.enqueue(tempQueue.dequeue());
    }

    saveWaitlistHelper(node->right, outFile);
}

void saveWaitlistToFile(courseRecords &Courses)
{
    ofstream outFile("Data/Waitlist.txt");
    if (!outFile.is_open())
    {
        cerr << "Error opening file for writing" << endl;
        PrintLog("Error opening waitlist file for writing\n");
        return;
    }

    saveWaitlistHelper(Courses.getRoot(), outFile);
    outFile.close();
    PrintLog("Waitlist saved to file\n");
}

void loadWaitlistFromFile(courseRecords &Courses)
{
    ifstream inFile("Data/Waitlist.txt");
    if (!inFile.is_open())
    {
        cerr << "Error opening file for reading" << endl;
        PrintLog("Error opening waitlist file for reading\n");
        return;
    }

    int courseId, studentId;
    string line;
    while (getline(inFile, line))
    {
        courseId = stoi(line);
        BSTNode *course = Courses.findCourse(courseId);
        if (course)
        {
            while (getline(inFile, line) && line != "")
            {
                studentId = stoi(line);
                course->waitlist.enqueue(studentId);
            }
        }
    }
    inFile.close();
    PrintLog("Waitlist loaded from file\n");
}

void saveEnrollmentHistoryToFile(studentRecordsSLL &StudentRecords)
{
    ofstream outFile("Data/EnrollmentHistory.txt");
    if (!outFile.is_open())
    {
        cerr << "Error opening file for writing" << endl;
        PrintLog("Error opening enrollment history file for writing\n");
        return;
    }

    studentNode *current = StudentRecords.head;
    while (current)
    {
        Student &student = current->getValue();
        outFile << student.studentId << endl;

        // Save enrollment history
        enrollmentRecord *record = student.enrollmentHistory->getHead();
        while (record)
        {
            outFile << record->courseId << endl;
            outFile << record->courseName << endl;
            outFile << record->grade << endl; // Save grade
            record = record->next;
        }
        outFile << endl; // Mark the end of enrollment history for this student
        current = current->getNext();
    }
    outFile.close();
    PrintLog("Enrollment history saved to file\n");
}

void loadEnrollmentHistoryFromFile(studentRecordsSLL &StudentRecords)
{
    ifstream inFile("Data/EnrollmentHistory.txt");
    if (!inFile.is_open())
    {
        cerr << "Error opening file for reading" << endl;
        PrintLog("Error opening enrollment history file for reading\n");
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        if(line == "")
            break;
        studentNode *student = StudentRecords.findStudent(stoi(line));
        while(getline(inFile, line))
        {
            if(line == "")
                break;
            string courseIdStr = line, courseName, gradeStr;
            getline(inFile, courseName);
            getline(inFile, gradeStr);

            student->getValue().enrollmentHistory->add(stoi(courseIdStr), courseName, stoi(gradeStr));
        }
    }
    inFile.close();
    PrintLog("Enrollment history loaded from file\n");
}

void saveGradesToFile(studentRecordsSLL &StudentRecords)
{
    ofstream outFile("Data/Grades.txt");
    if (!outFile.is_open())
    {
        cerr << "Error opening file for saving grades." << endl;
        PrintLog("Error opening grades file for writing\n");
        return;
    }

    studentNode *current = StudentRecords.head;
    while (current)
    {
        Student &student = current->getValue();
        outFile << student.studentId << endl;

        enrollmentRecord *record = student.enrollmentHistory->getHead();
        while (record)
        {
            outFile << record->courseId << endl;
            outFile << record->grade << endl;
            record = record->next;
        }
        outFile << endl; // Separate each student's grades by an empty line

        current = current->getNext();
    }
    outFile.close();
    PrintLog("Grades saved to file\n");
}

void loadGradesFromFile(studentRecordsSLL &StudentRecords)
{
    ifstream inFile("Data/Grades.txt");
    if (!inFile.is_open())
    {
        cerr << "Error opening file for loading grades." << endl;
        PrintLog("Error opening grades file for reading\n");
        return;
    }

    int studentId, courseId, grade;
    string line;
    while (getline(inFile, line))
    {
        studentId = stoi(line);
        studentNode *student = StudentRecords.findStudent(studentId);
        if (student)
        {
            while (getline(inFile, line) && line != "")
            {
                courseId = stoi(line);
                inFile >> grade;
                inFile.ignore();
                student->getValue().enrollmentHistory->updateGrade(courseId, grade);
            }
        }
    }
    inFile.close();
    PrintLog("Grades loaded from file\n");
}

void saveRegisteredStudentsHelper(BSTNode *node, ofstream &outFile)
{
    if (!node)
        return;

    saveRegisteredStudentsHelper(node->left, outFile);

    outFile << node->courseId << endl;
    studentNode *current = node->registeredStudents;
    while (current)
    {
        outFile << current->getValue().getStudentId() << endl;
        current = current->getNext();
    }
    outFile << endl; // Separate each course's registered students by an empty line

    saveRegisteredStudentsHelper(node->right, outFile);
}

void saveRegisteredStudentsToFile(courseRecords &Courses)
{
    ofstream outFile("Data/RegisteredStudents.txt");
    if (!outFile.is_open())
    {
        cerr << "Error opening file for writing" << endl;
        PrintLog("Error opening registered students file for writing\n");
        return;
    }

    saveRegisteredStudentsHelper(Courses.getRoot(), outFile);
    outFile.close();
    PrintLog("Registered students saved to file\n");
}

void loadRegisteredStudentsFromFile(courseRecords &Courses, studentRecordsSLL &StudentRecords)
{
    ifstream inFile("Data/RegisteredStudents.txt");
    if (!inFile.is_open())
    {
        cerr << "Error opening file for reading" << endl;
        PrintLog("Error opening registered students file for reading\n");
        return;
    }

    int courseId, studentId;
    string line;
    while (getline(inFile, line))
    {
        courseId = stoi(line);
        BSTNode *course = Courses.findCourse(courseId);
        if (course)
        {
            while (getline(inFile, line) && line != "")
            {
                studentId = stoi(line);
                studentNode *student = StudentRecords.findStudent(studentId);
                if (student)
                {
                    studentNode *newStudent = new studentNode(student->getValue());
                    newStudent->setNext(course->registeredStudents);
                    course->registeredStudents = newStudent;
                }
            }
        }
    }
    inFile.close();
    PrintLog("Registered students loaded from file\n");
}

void saveGPADataToFile(studentRecordsSLL &StudentRecords)
{
    ofstream outFile("Data/StudentGPA.txt");
    if (!outFile.is_open())
    {
        cerr << "Error opening file for saving GPA data." << endl;
        PrintLog("Error opening GPA data file for writing\n");
        return;
    }

    studentNode *current = StudentRecords.head;
    while (current)
    {
        outFile << current->getValue().studentId << " " << current->getValue().gpa << endl;
        current = current->getNext();
    }
    outFile.close();
    PrintLog("GPA data saved to file\n");
}

void loadGPADataFromFile(studentRecordsSLL &StudentRecords)
{
    ifstream inFile("Data/StudentGPA.txt");
    if (!inFile.is_open())
    {
        cerr << "Error opening file for loading GPA data." << endl;
        PrintLog("Error opening GPA data file for reading\n");
        return;
    }

    int studentId;
    double gpa;
    while (inFile >> studentId >> gpa)
    {
        studentNode *student = StudentRecords.findStudent(studentId);
        if (student)
        {
            student->getValue().gpa = gpa;
        }
    }
    inFile.close();
    PrintLog("GPA data loaded from file\n");
}
#endif