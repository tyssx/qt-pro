#ifndef QUEUE_H
#define QUEUE_H

template<typename T> class Queue
{
public:
    Queue(int max = 10);
    ~Queue();

    void push(const T &data);
    void pop();

    T &front();
    T &rear();

    bool isEmpty();
    int size();

private:
    T *m_queue;
    int m_max;
    int m_front; //队首标号
    int m_rear;  //队尾标号
};

#endif // QUEUE_H
