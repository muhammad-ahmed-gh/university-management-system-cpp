#ifndef SLL_H
#define SLL_H
#include <iostream>
#include <string>
#include <regex>
#include "Log.h"
#include "DLL.h"
using namespace std;

int ReadNumber(string Message = "")
{
    int Number;
    cout << Message;
    cin >> Number;
    cin.ignore();

    return Number;
}

class Student
{
public:
    int studentId;
    string name, email, phone, address, password;
    enrollmentHistoryDLL *enrollmentHistory;
    double gpa;
    double totalPoints;
    int totalCredits;

    Student()
    {
        studentId = 0;
        name = email = phone = address = password = "Unknown";
        gpa = 0.0;
        totalPoints = 0.0;
        totalCredits = 0;
        enrollmentHistory = new enrollmentHistoryDLL();
    }

    Student(int id, string studentName, string studentEmail, string studentPhone, string studentAddress, string studentPassword)
    {
        studentId = id;
        name = studentName;
        email = studentEmail;
        phone = studentPhone;
        address = studentAddress;
        password = studentPassword;
        gpa = 0.0;
        totalPoints = 0.0;
        totalCredits = 0;
    }

    bool isValidEmail(string &email)
    {
        regex pattern(R"(^[\w\.-]+@[\w\.-]+\.\w{2,4}$)");
        return regex_match(email, pattern);
    }

    bool isValidPhone(string &phone)
    {
        regex pattern("^01\\d{9}$"); // Updated pattern to ensure phone number starts with "01"
        return regex_match(phone, pattern);
    }

    bool isValidPassword(string &password)
    {
        return password.length() >= 6;
    }

    bool isValidId(int id)
    {
        return id > 0;
    }

    bool isValidName(string &name)
    {
        return !name.empty();
    }

    bool isValidAddress(string &address)
    {
        return !address.empty();
    }

    void enterData()
    {
        do
        {
            cout << "Name: ";
            getline(cin, name);
            if (!isValidName(name))
                cout << "Invalid name. Please try again." << endl;
        } while (!isValidName(name));

        do
        {
            cout << "ID: ";
            cin >> studentId;
            cin.ignore();
            if (!isValidId(studentId))
                cout << "Invalid ID. Please try again." << endl;
        } while (!isValidId(studentId));

        do
        {
            cout << "Email: ";
            getline(cin, email);
            if (!isValidEmail(email))
                cout << "Invalid email. Please try again." << endl;
        } while (!isValidEmail(email));

        do
        {
            cout << "Phone: ";
            getline(cin, phone);
            if (!isValidPhone(phone))
                cout << "Invalid phone number. Please try again." << endl;
        } while (!isValidPhone(phone));

        do
        {
            cout << "Address: ";
            getline(cin, address);
            if (!isValidAddress(address))
                cout << "Invalid address. Please try again." << endl;
        } while (!isValidAddress(address));

        do
        {
            cout << "Password: ";
            getline(cin, password);
            if (!isValidPassword(password))
                cout << "Invalid password. Please try again." << endl;
        } while (!isValidPassword(password));
        PrintLog("Student data entered: " + name + ", ID: " + to_string(studentId) + "\n");
    }

    void showData()
    {
        cout << "Name: " << name << endl;
        cout << "ID: " << studentId << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Address: " << address << endl;
        cout << "Password: " << password << endl;
        cout << "GPA: " << gpa << endl;
    }

    int getStudentId()
    {
        return studentId;
    }

    void setStudentId(int id)
    {
        studentId = id;
    }

    void updateContactInfo(string newEmail, string newPhone, string newAddress)
    {
        if (isValidEmail(newEmail))
            email = newEmail;
        if (isValidPhone(newPhone))
            phone = newPhone;
        if (isValidAddress(newAddress))
            address = newAddress;
        PrintLog("Contact information updated for student ID: " + to_string(studentId) + "\n");
    }

    double convertGradeToPoint(int grade)
    {
        if (grade >= 97)
            return 4.0;
        else if (grade >= 90)
            return 4.0;
        else if (grade >= 85)
            return 3.7;
        else if (grade >= 80)
            return 3.3;
        else if (grade >= 75)
            return 3.0;
        else if (grade >= 70)
            return 2.7;
        else if (grade >= 65)
            return 2.3;
        else if (grade >= 60)
            return 2.0;
        else if (grade >= 55)
            return 1.7;
        else if (grade >= 50)
            return 1.3;
        else if (grade >= 45)
            return 1.0;
        else
            return 0.0;
    }

    void updateGPA(int grade, int credits)
    {
        double points = convertGradeToPoint(grade);
        totalPoints += points * credits;
        totalCredits += credits;
        gpa = totalPoints / totalCredits;
        PrintLog("GPA updated for student ID: " + to_string(studentId) + "\n");
    }

    void updateData(string studentName, string studentEmail, string studentPhone, string studentAddress, string studentPassword)
    {
        if (isValidName(studentName))
            name = studentName;
        if (isValidEmail(studentEmail))
            email = studentEmail;
        if (isValidPhone(studentPhone))
            phone = studentPhone;
        if (isValidAddress(studentAddress))
            address = studentAddress;
        if (isValidPassword(studentPassword))
            password = studentPassword;
        PrintLog("Student data updated for student ID: " + to_string(studentId) + "\n");
    }
};

class studentNode
{
private:
    Student value;
    studentNode *next;

public:
    studentNode()
    {
        next = NULL;
    }

    studentNode(Student &studentValue)
    {
        value = studentValue;
        next = NULL;
    }

    studentNode(Student studentValue, studentNode *nextNode)
    {
        value = studentValue;
        next = nextNode;
    }

    Student &getValue()
    {
        return value;
    }

    studentNode *getNext()
    {
        return next;
    }

    void setValue(Student studentValue)
    {
        value = studentValue;
    }

    void setNext(studentNode *nextNode)
    {
        next = nextNode;
    }
};

class studentRecordsSLL
{
public:
    studentNode *head;
    studentNode *tail;
    int length;

    studentRecordsSLL()
    {
        head = tail = NULL;
        length = 0;
    }

    ~studentRecordsSLL()
    {
        while (head)
        {
            studentNode *temp = head;
            head = head->getNext();
            delete temp;
        }
    }

    void addStudent()
    {
        Student student;
        student.enterData();
        if (head == NULL)
        {
            head = tail = new studentNode(student);
            length++;
            cout << "The student was added successfully.\n\n";
            PrintLog("A student was added successfully\n");
            return;
        }
        if (!findStudent(student.studentId))
        {
            tail->setNext(new studentNode(student));
            tail = tail->getNext();
            length++;
            cout << "The student was added successfully.\n\n";
            PrintLog("A student was added successfully\n");
            return;
        }
        cout << "The student already exists.\n\n";
        PrintLog("Trying to add an existing student\n");
    }

    void addStudent(Student student)
    {
        if (head == NULL)
            head = tail = new studentNode(student);
        else
        {
            tail->setNext(new studentNode(student));
            tail = tail->getNext();
        }
        length++;
        cout << "The student was added successfully.\n\n";
        PrintLog("A student was added successfully\n");
    }

    void removeStudent(int studentId)
    {
        studentNode *p = head;
        studentNode *prev = NULL;

        while (p)
        {
            if (p->getValue().getStudentId() == studentId)
            {
                if (prev == NULL) // Node to be deleted is head
                {
                    head = p->getNext();
                    if (head == NULL) // If the list becomes empty, update the tail
                        tail = NULL;
                }
                else
                {
                    prev->setNext(p->getNext());
                    if (p == tail) // If the deleted node was the tail, update the tail
                        tail = prev;
                }

                // Clean up related data
                delete p;
                length--;
                cout << "The student was deleted successfully.\n\n";
                PrintLog("A student was deleted successfully\n");
                return;
            }
            prev = p;
            p = p->getNext();
        }
    }

    void displayStudents()
    {
        cout << "******************************************\n";
        cout << "*              All Students              *\n";
        cout << "******************************************\n";
        studentNode *p = head;
        while (p != NULL)
        {
            p->getValue().showData();
            p->getValue().enrollmentHistory->display();
            cout << "******************************************\n";
            p = p->getNext();
        }
        PrintLog("Displayed all students.\n");
    }

    studentNode *findStudent(int studentId)
    {
        studentNode *p = head;
        while (p)
        {
            if (p->getValue().studentId == studentId)
                return p;
            p = p->getNext();
        }
        return NULL;
    }

    void updateStudentGPA(int studentId, int grade, int credits)
    {
        studentNode *student = findStudent(studentId);
        if (student)
        {
            student->getValue().updateGPA(grade, credits);
            cout << "GPA updated successfully for student ID " << studentId << endl;
            PrintLog("GPA updated successfully for a student\n");
        }
        else
        {
            cout << "Student not found." << endl;
            PrintLog("Trying to update GPA for a non-existing student\n");
        }
    }

    void updateStudentData(int studentId, string studentName, string studentEmail, string studentPhone, string studentAddress, string studentPassword)
    {
        studentNode *student = findStudent(studentId);
        if (student)
        {
            student->getValue().updateData(studentName, studentEmail, studentPhone, studentAddress, studentPassword);
            cout << "Student data updated successfully.\n\n";
            PrintLog("Student data updated successfully\n");
        }
        else
        {
            cout << "Student not found.\n\n";
            PrintLog("Trying to update data for a non-existing student\n");
        }
    }

    studentNode* traverse(int Index)
    {
        studentNode* p = head;
        for(int i = 0; i<Index && p; i++)
            p = p->getNext();
        return p;
    }
};
#endif