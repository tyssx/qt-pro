#include "queue.h"

template<typename T> Queue<T>::Queue(int max)
{
    m_max   = max;
    m_front = 0;
    m_rear  = 0;

    m_queue = new T[max];
}

template<typename T> Queue<T>::~Queue()
{
}

template<typename T> void Queue<T>::push(const T &data)
{
    //    if (m_rear-m_front )
}
