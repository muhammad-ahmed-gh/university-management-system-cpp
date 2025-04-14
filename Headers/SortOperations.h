#ifndef SORTOPERATIONS_H
#define SORTOPERATIONS_H
#include <algorithm>
#include <vector>
#include "SLL.h"
#include "Log.h"
using namespace std;

void Swap(studentNode* x, studentNode* y)
{
    Student temp = x->getValue();
    x->setValue(y->getValue());
    y->setValue(temp);
}

void quickSortById(studentRecordsSLL &SLL, int start, int end)
{
    if(end <= start)
        return;
    int currentIndex, swapIndex = start-1;
    studentNode *pivot = SLL.traverse(end);
    for(currentIndex = start; currentIndex<=end; currentIndex++)
    {
        if(SLL.traverse(currentIndex)->getValue().studentId <= pivot->getValue().studentId)
        {
            swapIndex++;
            if(currentIndex>swapIndex)
                Swap(SLL.traverse(swapIndex), SLL.traverse(currentIndex));
        }
    }
    quickSortById(SLL, start, swapIndex-1);
    quickSortById(SLL, swapIndex+1, end);
}

void quickSortById(studentRecordsSLL &SLL)
{
    quickSortById(SLL, 0, SLL.length-1);
}

void quickSortByName(studentRecordsSLL &SLL, int start, int end)
{
    if(end <= start)
        return;
    int currentIndex, swapIndex = start-1;
    studentNode *pivot = SLL.traverse(end);
    for(currentIndex = start; currentIndex<=end; currentIndex++)
    {
        if(SLL.traverse(currentIndex)->getValue().name <= pivot->getValue().name)
        {
            swapIndex++;
            if(currentIndex>swapIndex)
                Swap(SLL.traverse(swapIndex), SLL.traverse(currentIndex));
        }
    }
    quickSortByName(SLL, start, swapIndex-1);
    quickSortByName(SLL, swapIndex+1, end);
}

void quickSortByName(studentRecordsSLL &SLL)
{
    quickSortByName(SLL, 0, SLL.length-1);
}

#endif