#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include "Log.h"
using namespace std;

template <typename T>
class QueueNode
{
public:
    T value;
    QueueNode *next;

    QueueNode(T val)
    {
        value = val;
        next = NULL;
    }
};

template <typename T>
class Queue
{
private:
    QueueNode<T> *front;
    QueueNode<T> *rear;

public:
    Queue()
    {
        front = rear = NULL;
    }

    ~Queue()
    {
        while (front)
        {
            QueueNode<T> *temp = front;
            front = front->next;
            delete temp;
        }
    }

    T frontValue()
    {
        return front->value;
    }

    void enqueue(T value)
    {
        QueueNode<T> *newNode = new QueueNode<T>(value);
        if (rear)
        {
            rear->next = newNode;
        }
        else
        {
            front = newNode;
        }
        rear = newNode;
        PrintLog("A value was enqueued\n");
    }

    T dequeue()
    {
        if (!front)
        {
            PrintLog("Attempted to dequeue from empty queue\n");
            throw runtime_error("Queue is empty");
        }
        T value = front->value;
        QueueNode<T> *temp = front;
        front = front->next;
        if (!front)
        {
            rear = NULL;
        }
        delete temp;
        PrintLog("A value was dequeued\n");
        return value;
    }

    bool isEmpty()
    {
        return front == NULL;
    }
};

#endif