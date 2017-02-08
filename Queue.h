/*
 * Queue.h
 *
 *  Created on: Feb 20, 2015
 *      Author: rmin
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class Queue {
private:
    std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cv;
public:
    Queue();
    ~Queue();
    T pop();
    void push(T value);
};

#endif /* QUEUE_H_ */
