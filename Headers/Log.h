#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

void PrintLog(string Message)
{
    fstream inFile("Data/Log.txt", ios::app);
    if (!inFile.is_open())
    {
        cout << "Error opening log file." << endl;
        return;
    }
    time_t now = time(0);
    tm *ltm = localtime(&now);
    inFile << "[" << 1900 + ltm->tm_year << "-"
           << ltm->tm_mon << "-"
           << ltm->tm_mday << " "
           << ltm->tm_hour << ":"
           << ltm->tm_min << ":"
           << ltm->tm_sec << "] "
           << Message;
    inFile.close();
}
#endif