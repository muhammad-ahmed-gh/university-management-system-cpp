#ifndef ManagementSystem_H
#define ManagementSystem_H
#include "SearchOptimization.h"
#include "SortOperations.h"
#include "FileHandling.h"
#include "HashTable.h"
#include <iostream>
#include <fstream>
#include "Stack.h"
#include "Queue.h"
#include "SLL.h"
#include "DLL.h"
#include "BST.h"
#include "Log.h"

enum enAction
{
    StudentsData = 1, // 1
    CoursesData,      // 2
    Exit,             // 3
};

enum enStudentsChoices
{
    AddStudent = 1,     // 1
    RemoveStudent,      // 2
    FindStudent,        // 3
    SortStudentsByName, // 4
    SortStudentsById,   // 5
    DisplayAllStudents, // 6
    UpdateStudentData,  // 7
    UpdateContactInfo,  // 8
    EnrollInCourse,     // 9
    DropCourse,         // 10
    PutGrade,           // 11
    ReturnToMainMenu    // 12
};

enum enCoursesChoices
{
    AddCourse = 1,     // 1
    RemoveCourse,      // 2
    FindCourse,        // 3
    DisplayAllCourses, // 4
    DisplayWaitlist,   // 5
    PrintBST,          // 6
    ReturnMainMenu     // 7
};

enum enPrintBSTChoices
{
    BreadthFirst = 1,  // 1
    InOrder,           // 2
    PreOrder,          // 3
    PostOrder,         // 4
    PrintAll,          // 5
    ReturnToCourseMenu // 6
};

enum enColor
{
    ResetColor          = 0,
    Red                 = 31,
    Green               = 32,
    Yellow              = 33,
    Blue                = 34,
    Magenta             = 35,
    Cyan                = 36,
    BrightRed           = 91,
    BrightGreen         = 92,
    BrightYellow        = 93,
    BrightBlue          = 94,
    BrightMagenta       = 95,
    BrightCyan          = 96
};

void setColor(enColor color)
{
    string strColor = "\033[";
    strColor += to_string(color);
    strColor += "m";
    cout << strColor;
}

void waitForEnter()
{
    cout << "Press Enter to return to the main menu...";
    cin.get();
}

void Clear()
{
    cout << "\033[2J\033[1;1H";
}

class ManagementSystem
{
private:
    void dropAllCourses(int studentId)
    {
        studentNode *student = StudentRecords.findStudent(studentId);
        enrollmentRecord *p = student->getValue().enrollmentHistory->getHead();
        while (p)
        {
            enrollmentRecord *p2 = p->next;
            dropCourse(student, Courses.findCourse(p->courseId));
            p = p2;
        }
    }

public:
    studentRecordsSLL StudentRecords;
    courseRecords Courses;
    HashTable courseHashTable;

    void showMainUI()
    {
        setColor(enColor::BrightYellow);
        cout << "*********************************************************\n";
        cout << "*                                                       *\n";
        cout << "*                     WELCOME to NU                     *\n";
        cout << "*                   Management System                   *\n";
        cout << "*                                                       *\n";
        cout << "*********************************************************\n";

        setColor(enColor::ResetColor);
        cout << "- What do you want to manage?\n";
        cout << "1. Students data                2. Courses Data\n";
        cout << "3. Exit" << endl;
    }

    void showStudentsUI()
    {
        setColor(enColor::Yellow);
        cout << "1. Add a student                2. Remove a student\n";
        cout << "3. Search for a student         4. Sort students by names\n";
        cout << "5. Sort students by Ids         6. Display all students\n";
        cout << "7. Update student data          8. Update contact info\n";
        cout << "9. Enroll in a course           10. Drop a course\n";
        cout << "11. Put Grade                   12. Return to Main Menu" << endl;
    }

    void showCoursesUI()
    {
        setColor(enColor::Green);
        cout << "1. Add a course                 2. Remove a course\n";
        cout << "3. Search for a course          4. Display all courses\n";
        cout << "5. Display waitlists            6. Print BST\n";
        cout << "7. Return to Main Menu" << endl;
    }

    void showPrintBSTUI()
    {
        setColor(enColor::Cyan);
        cout << "1. Breadth-First Traversal      2. In-Order Traversal\n";
        cout << "3. Pre-Order Traversal          4. Post-Order Traversal\n";
        cout << "5. Print All Traversals         6. Return to Main Menu" << endl;
    }

    BSTNode *searchWithHashing(int courseId)
    {
        return courseHashTable.search(courseId);
    }

    void insertCoursesIntoHashTable(BSTNode *node)
    {
        if (!node)
            return;
        courseHashTable.insert(node);
        insertCoursesIntoHashTable(node->left);
        insertCoursesIntoHashTable(node->right);
    }

    void loadData()
    {
        ::loadCoursesFromFile(Courses);
        insertCoursesIntoHashTable(Courses.getRoot());

        ::loadStudentsFromFile(StudentRecords);
        ::loadWaitlistFromFile(Courses);
        ::loadEnrollmentHistoryFromFile(StudentRecords);
        ::loadRegisteredStudentsFromFile(Courses, StudentRecords);
        ::loadGPADataFromFile(StudentRecords);
        ::loadGradesFromFile(StudentRecords);
    }

    void saveData()
    {
        ::saveCoursesToFile(Courses);
        ::saveStudentsToFile(StudentRecords);
        ::saveWaitlistToFile(Courses);
        ::saveEnrollmentHistoryToFile(StudentRecords);
        ::saveRegisteredStudentsToFile(Courses);
        ::saveGPADataToFile(StudentRecords);
        ::saveGradesToFile(StudentRecords);
    }

    void enrollInCourse()
    {
        int courseId = ReadNumber("Enter Course Id: ");
        BSTNode *course = searchWithHashing(courseId);
        if (!course)
        {
            cout << "Course not found." << endl;
            PrintLog("Trying to register a student in a non-existing course\n");
            return;
        }

        int studentId = ReadNumber("Enter Student Id: ");
        studentNode *student = StudentRecords.findStudent(studentId);
        if (!student)
        {
            cout << "Student not found." << endl;
            PrintLog("Trying to register a non-existing student in a course\n");
            return;
        }

        if (validatePrerequisites(Courses, StudentRecords, courseId, studentId))
        {
            if (!student->getValue().enrollmentHistory->contains(courseId))
            {
                if (Courses.registerStudent(courseId, studentId))
                    student->getValue().enrollmentHistory->add(courseId, course->courseName);
            }
            else
            {
                cout << "The student has already registered this course." << endl;
                PrintLog("Trying to register a student in a course that they already registered\n");
            }
        }
    }

    void dropCourse(studentNode *student, BSTNode *course)
    {
        int studentId = student->getValue().studentId;
        int courseId = course->courseId;

        student->getValue().enrollmentHistory->drop(courseId);
        Courses.dropCourse(courseId, studentId);

        if (!course->waitlist.isEmpty())
        {
            int nextStudentId = course->waitlist.dequeue();
            while (!StudentRecords.findStudent(nextStudentId) && !course->waitlist.isEmpty())
            {
                nextStudentId = course->waitlist.dequeue();
            }
            if (StudentRecords.findStudent(nextStudentId))
            {
                Courses.registerStudent(courseId, nextStudentId);
                StudentRecords.findStudent(nextStudentId)->getValue().enrollmentHistory->add(courseId, course->courseName);
            }
        }
    }

    void dropCourse()
    {
        int courseId = ReadNumber("Enter Course Id: ");
        BSTNode *course = searchWithHashing(courseId);
        if (!course)
        {
            cout << "Course not found." << endl;
            PrintLog("Trying to drop a non-existing course\n");
            return;
        }

        int studentId = ReadNumber("Enter Student Id: ");
        studentNode *student = StudentRecords.findStudent(studentId);
        if (!student)
        {
            cout << "Student not found." << endl;
            PrintLog("Trying to drop a course for a non-existing student\n");
            return;
        }

        if (!student->getValue().enrollmentHistory->contains(courseId))
        {
            cout << "The student of Id " << studentId << " didn\'t add the course " << courseId << endl;
            PrintLog("A student trying to drop a non-added course\n");
            return;
        }
        if (student->getValue().enrollmentHistory->getRecord(courseId)->grade != -1)
        {
            cout << "The student already has a final grade." << endl;
            PrintLog("Trying to drop a course that has a final grade\n");
            return;
        }
        dropCourse(student, course);
    }

    void displayAllCourses()
    {
        cout << "*****************************************\n";
        cout << "*              All Courses              *\n";
        cout << "*****************************************\n";
        Courses.displayCourses();
        cout << "*****************************************\n";
    }

    void displayWaitlist()
    {
        int courseId = ReadNumber("Enter Course Id: ");
        BSTNode *course = searchWithHashing(courseId);
        if (!course)
        {
            cout << "Course not found." << endl;
            PrintLog("Trying to display waitlist for a non-existing course\n");
            return;
        }

        Queue<int> &waitlist = course->waitlist;
        if (waitlist.isEmpty())
        {
            cout << "Waitlist is empty for course " << courseId << endl;
            return;
        }

        cout << "*****************************************\n";
        cout << "*          Waitlist for Course          *\n";
        cout << "*****************************************\n";
        cout << "Course ID: " << courseId << endl;
        cout << "*****************************************\n";
        Queue<int> tempQueue;
        while (!waitlist.isEmpty())
        {
            int studentId = waitlist.dequeue();
            cout << studentId << endl;
            tempQueue.enqueue(studentId);
        }

        while (!tempQueue.isEmpty())
        {
            waitlist.enqueue(tempQueue.dequeue());
        }
        cout << "*****************************************\n";
    }

    void putGrade()
    {
        int studentId = ReadNumber("Enter Student Id: ");
        studentNode *student = StudentRecords.findStudent(studentId);
        if (!student)
        {
            cout << "Student not found." << endl;
            PrintLog("Trying to put grade for a non-existing student\n");
            return;
        }

        if (!student->getValue().enrollmentHistory->getHead())
        {
            cout << "No enrollment history found for this student." << endl;
            PrintLog("No enrollment history found for student ID: " + to_string(studentId) + "\n");
            return;
        }

        cout << "Courses enrolled by the student:" << endl;
        student->getValue().enrollmentHistory->display();

        int courseId = ReadNumber("Enter Course Id: ");
        BSTNode *course = Courses.findCourse(courseId);
        if (!course)
        {
            cout << "Course not found." << endl;
            PrintLog("Trying to put grade for a non-existing course\n");
            return;
        }

        // Validate if the student is enrolled in the course
        if (!student->getValue().enrollmentHistory->contains(courseId))
        {
            cout << "The student is not enrolled in this course." << endl;
            PrintLog("Trying to put grade for a course the student is not enrolled in\n");
            return;
        }

        int grade = ReadNumber("Enter Numeric Grade (0-100): ");
        StudentRecords.updateStudentGPA(studentId, grade, course->courseCredits);
        student->getValue().enrollmentHistory->updateGrade(courseId, grade); // Save grade in enrollment history
    }

    void findCourse()
    {
        int courseId = ReadNumber("Enter Course Id: ");
        BSTNode *cNode = searchCourseById(Courses, courseId);

        if (cNode)
        {
            cout << "The course was found\n";
            cNode->showData();
            cout << endl;
        }
        else
        {
            cout << "Course not found\n\n";
            PrintLog("Trying to search for a non-existing course\n");
        }
    }

    void findStudent()
    {
        int studentId = ReadNumber("Enter Student Id: ");
        Student *student = searchStudentById(StudentRecords, studentId);
        if (student)
        {
            cout << "The student was found" << endl;
            student->showData();
            cout << endl;
        }
        else
        {
            cout << "Student not found\n\n";
            PrintLog("Trying to search for a non-existing student\n");
        }
    }

    void updateStudentData()
    {
        int studentId = ReadNumber("Enter Student Id: ");
        studentNode *student = StudentRecords.findStudent(studentId);
        if (!student)
        {
            cout << "Student not found.\n\n";
            PrintLog("Trying to update data for a non-existing student\n");
            return;
        }

        string studentName, studentEmail, studentPhone, studentAddress, studentPassword;

        do
        {
            cout << "Enter new name: ";
            getline(cin, studentName);
            if (!student->getValue().isValidName(studentName))
                cout << "Invalid name. Please try again." << endl;
        } while (!student->getValue().isValidName(studentName));

        do
        {
            cout << "Enter new email: ";
            getline(cin, studentEmail);
            if (!student->getValue().isValidEmail(studentEmail))
                cout << "Invalid email. Please try again. Example: example@domain.com" << endl;
        } while (!student->getValue().isValidEmail(studentEmail));

        do
        {
            cout << "Enter new phone: ";
            getline(cin, studentPhone);
            if (!student->getValue().isValidPhone(studentPhone))
                cout << "Invalid phone number. Please try again. Example: 12345678901" << endl;
        } while (!student->getValue().isValidPhone(studentPhone));

        do
        {
            cout << "Enter new address: ";
            getline(cin, studentAddress);
            if (!student->getValue().isValidAddress(studentAddress))
                cout << "Invalid address. Please try again." << endl;
        } while (!student->getValue().isValidAddress(studentAddress));

        do
        {
            cout << "Enter new password: ";
            getline(cin, studentPassword);
            if (!student->getValue().isValidPassword(studentPassword))
                cout << "Invalid password. Please try again. Password must be at least 6 characters long." << endl;
        } while (!student->getValue().isValidPassword(studentPassword));

        StudentRecords.updateStudentData(studentId, studentName, studentEmail, studentPhone, studentAddress, studentPassword);
    }

    void updateContactInfo()
    {
        int studentId = ReadNumber("Enter Student Id: ");
        studentNode *student = StudentRecords.findStudent(studentId);
        if (!student)
        {
            cout << "Student not found." << endl;
            PrintLog("Trying to update contact info for a non-existing student\n");
            return;
        }

        string newEmail, newPhone, newAddress;

        do
        {
            cout << "Enter new email: ";
            getline(cin, newEmail);
            if (!student->getValue().isValidEmail(newEmail))
                cout << "Invalid email. Please try again. Example: example@domain.com" << endl;
        } while (!student->getValue().isValidEmail(newEmail));

        do
        {
            cout << "Enter new phone: ";
            getline(cin, newPhone);
            if (!student->getValue().isValidPhone(newPhone))
                cout << "Invalid phone number. Please try again. Example: 12345678901" << endl;
        } while (!student->getValue().isValidPhone(newPhone));

        do
        {
            cout << "Enter new address: ";
            getline(cin, newAddress);
            if (!student->getValue().isValidAddress(newAddress))
                cout << "Invalid address. Please try again." << endl;
        } while (!student->getValue().isValidAddress(newAddress));

        student->getValue().updateContactInfo(newEmail, newPhone, newAddress);
        cout << "Contact information updated successfully.\n\n";
        PrintLog("Contact information updated successfully\n");
    }

    void calculateGPA(studentRecordsSLL &StudentRecords)
    {
        studentNode *current = StudentRecords.head;
        while (current)
        {
            Student &student = current->getValue();
            double totalPoints = 0.0;
            int totalCredits = 0;
            enrollmentRecord *record = student.enrollmentHistory->getHead();
            while (record)
            {
                if (record->grade != -1)
                {
                    double points = student.convertGradeToPoint(record->grade);
                    totalPoints += points * student.totalCredits;
                    totalCredits += student.totalCredits;
                }
                record = record->next;
            }
            if (totalCredits > 0)
                student.gpa = totalPoints / totalCredits;
            else
                student.gpa = 0.0;
            current = current->getNext();
        }
        PrintLog("GPA calculated for all students\n");
    }

    void studentsOperations(int choice)
    {
        switch (choice)
        {
        case enStudentsChoices::AddStudent:
            setColor(enColor::Green);
            PrintLog("Adding a new student\n");
            StudentRecords.addStudent();
            break;

        case enStudentsChoices::RemoveStudent:
        {
            setColor(enColor::Red);
            PrintLog("Removing a student\n");
            if (StudentRecords.length == 0)
            {
                cout << "There are no registered students\n";
                PrintLog("Trying to remove a student from an empty list\n");
                return;
            }

            int studentId = ReadNumber("Student Id: ");
            if (StudentRecords.findStudent(studentId))
            {
                dropAllCourses(studentId);
                StudentRecords.removeStudent(studentId);
            }
            else
            {
                cout << "The student does not exist\n";
                PrintLog("Trying to remove a non-existing student\n");
            }
            break;
        }

        case enStudentsChoices::FindStudent:
            setColor(enColor::Yellow);
            PrintLog("Finding a student\n");
            findStudent();
            break;

        case enStudentsChoices::SortStudentsByName:
            setColor(enColor::Blue);
            PrintLog("Sorting students by name\n");
            quickSortByName(StudentRecords);
            break;

        case enStudentsChoices::SortStudentsById:
            setColor(enColor::Blue);
            PrintLog("Sorting students by ID\n");
            quickSortById(StudentRecords);
            break;

        case enStudentsChoices::DisplayAllStudents:
            setColor(enColor::Blue);
            PrintLog("Displaying all students\n");
            StudentRecords.displayStudents();
            break;

        case enStudentsChoices::UpdateStudentData:
            setColor(enColor::BrightMagenta);
            PrintLog("Updating student data\n");
            updateStudentData();
            break;

        case enStudentsChoices::UpdateContactInfo:
            setColor(enColor::BrightMagenta);
            PrintLog("Updating contact info\n");
            updateContactInfo();
            break;

        case enStudentsChoices::EnrollInCourse:
            setColor(enColor::BrightRed);
            PrintLog("Enrolling in a course\n");
            enrollInCourse();
            break;

        case enStudentsChoices::DropCourse:
            setColor(enColor::BrightRed);
            PrintLog("Dropping a course\n");
            dropCourse();
            calculateGPA(StudentRecords);
            break;

        case enStudentsChoices::PutGrade:
            setColor(enColor::BrightGreen);
            PrintLog("Putting a grade\n");
            putGrade();
            break;

        case enStudentsChoices::ReturnToMainMenu:
            setColor(enColor::BrightCyan);
            PrintLog("Returning to main menu\n");
            break;

        default:
            setColor(enColor::BrightCyan);
            cout << "Invalid input\n";
        }
    }

    void coursesOperations(int choice)
    {
        switch (choice)
        {
        case enCoursesChoices::AddCourse:
            setColor(enColor::Magenta);
            PrintLog("Adding a new course\n");
            Courses.addCourse(Courses.ReadCourse());
            break;

        case enCoursesChoices::RemoveCourse:
            setColor(enColor::Cyan);
            PrintLog("Removing a course\n");
            Courses.removeCourse(ReadNumber("Enter Course Id: "), StudentRecords);
            calculateGPA(StudentRecords);
            saveData();
            savePrereqToFile(Courses.getRoot()); // Save updated prerequisites list
            break;

        case enCoursesChoices::FindCourse:
            setColor(enColor::BrightGreen);
            PrintLog("Finding a course\n");
            findCourse();
            break;

        case enCoursesChoices::DisplayAllCourses:
            setColor(enColor::BrightYellow);
            PrintLog("Displaying all courses\n");
            displayAllCourses();
            break;

        case enCoursesChoices::DisplayWaitlist:
            setColor(enColor::BrightBlue);
            PrintLog("Displaying waitlist\n");
            displayWaitlist();
            break;

        case enCoursesChoices::PrintBST:
            showPrintBSTUI();
            choice = ReadNumber();

            switch (choice)
            {
            case enPrintBSTChoices::BreadthFirst:
                setColor(enColor::BrightYellow);
                PrintLog("Breadth-First Traversal\n");
                Courses.breadthFirstTraversal(Courses.getRoot());
                break;

            case enPrintBSTChoices::InOrder:
                setColor(enColor::BrightYellow);
                PrintLog("In-Order Traversal\n");
                Courses.inOrderTraversal(Courses.getRoot());
                cout << endl;
                break;

            case enPrintBSTChoices::PreOrder:
                setColor(enColor::BrightYellow);
                PrintLog("Pre-Order Traversal\n");
                Courses.preOrderTraversal(Courses.getRoot());
                cout << endl;
                break;

            case enPrintBSTChoices::PostOrder:
                setColor(enColor::BrightYellow);
                PrintLog("Post-Order Traversal\n");
                Courses.postOrderTraversal(Courses.getRoot());
                cout << endl;
                break;

            case enPrintBSTChoices::PrintAll:
                setColor(enColor::BrightYellow);
                PrintLog("Printing all traversals\n");
                cout << "Breadth-First Traversal: ";
                Courses.breadthFirstTraversal(Courses.getRoot());
                cout << "In-Order Traversal: ";
                Courses.inOrderTraversal(Courses.getRoot());
                cout << endl;
                cout << "Pre-Order Traversal: ";
                Courses.preOrderTraversal(Courses.getRoot());
                cout << endl;
                cout << "Post-Order Traversal: ";
                Courses.postOrderTraversal(Courses.getRoot());
                cout << endl;
                break;

            case enPrintBSTChoices::ReturnToCourseMenu:
                setColor(enColor::BrightCyan);
                PrintLog("Returning to courses menu\n");
                break;

            default:
                setColor(enColor::BrightCyan);
                cout << "Invalid input\n";
            }
            break;

        case enCoursesChoices::ReturnMainMenu:
            setColor(enColor::BrightCyan);
            PrintLog("Returning to main menu\n");
            break;

        default:
            setColor(enColor::BrightCyan);
            cout << "Invalid input\n";
        }
    }

    void RunProgram()
    {
        while (true)
        {
            Clear();
            showMainUI();
            int choice = ReadNumber();

            switch (choice)
            {
            case enAction::StudentsData:
                showStudentsUI();
                studentsOperations(ReadNumber());
                break;
            case enAction::CoursesData:
                showCoursesUI();
                coursesOperations(ReadNumber());
                break;

            case enAction::Exit:
                setColor(enColor::BrightMagenta);
                cout << "Program has terminated." << endl;
                PrintLog("Program has terminated\n");
                saveData();
                return;

            default:
                setColor(enColor::BrightCyan);
                cout << "Invalid input\n";
            }
            saveData();
            waitForEnter();
        }
    }
};
#endif