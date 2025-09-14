//
// Created by shangeyun on 2024/2/24.
//

//#ifndef MTHREAD_MMTHREAD_H
//#define MTHREAD_MMTHREAD_H
#pragma once

#include <thread>

class MMThread {

public:
    virtual void run() = 0;

    int start();//用来启动线程
    int Stop();

public:
    std::thread * t = nullptr;

    int stopFlag = 0;
};


//#endif //MTHREAD_MMTHREAD_H
