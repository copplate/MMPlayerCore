//
// Created by shangeyun on 2024/2/24.
//

#include "MMThread.h"
#include <thread>

int MMThread::start(){
    if (t == nullptr) {//如果t为nullptr，说明线程不存在
        stopFlag = 0;
        t = new std::thread(&MMThread::run, this);
    }



    //目前我们自己写的MMThread也还有很多可以完善的地方，比如detach出去就和主线程完全没有关系了，就不太合适，
    //也就是说，我们怎么方便的控制，让它正常的结束。
    //还有一些别的东西：外部怎么判断这个线程是否在运行，线程回收时如何去回收等。
    /*std::thread t(&MMThread::run, this);
    t.detach();*/
    return 0;
}

int MMThread::Stop() {
    if (t != nullptr) {
        stopFlag = 1;
        t->join();//阻塞，等线程结束再进行下一步
        delete t;
        t = nullptr;
    }
    return 0;
}
