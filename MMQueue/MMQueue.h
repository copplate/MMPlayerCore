#pragma once

//先不实现自己的容器，先用std库提供的queue
#include <queue>

/*
MMQueue<MMAVFrame> queue
*/

//1、因为想塞一些自己的数据，所以涉及到一个泛型的问题，
//泛型在c++中是用template实现的
//2、为什么不直接用std提供的队列，因为不是线程安全的，
//在我们自己的队列中，要加上线程锁，然后就可以多线程修改队列，
//因为写播放器的过程中，读和解码都不是在一个线程里
template<typename T>
/*  1、MMQueue是在std的队列上又封装了一层，用起来更方便
	2、因为用了template，要直接在头文件写函数的实现，
	template是一个编译时的泛型，如果把实现写到另一个.cpp文件中，是无法参与到最后的编译的。

*/
class MMQueue 
{
public:
	MMQueue()
	{


	}

	~MMQueue()
	{
		while (Size() > 0) {//清空队列
			T* t = nullptr;
			Pop(&t);
		}
	}

	/*  参数是想要塞的类型的指针  */
	int Push(T * t){
		queue.push(t);//放入队列的头位置
		return 0;
	}

	/*  参数是指针的指针  */
	int Pop(T * * t){
		int size = Size();
		if (size > 0) {
			*t = queue.front();
			queue.pop();
			return 0;
		}
		return -1;
	}

	/*  获取queue的大小*/
	int Size() {
		return queue.size();
	}



private:
	/*    本来应该只塞一个T，<T>，但是可以做一个变换
	当在template声明了比如MMAVFrame，但实际上想存的是MMAVFrame的指针
	这时就可以耍一个小花招，声明存的是T的指针<T *>,这样即使在外面声明存的是MMAVFrame，
	但实际上操作的都是MMAVFrame的指针。    
		这样的好处是 1、在MMQueue(){}中，可以指针对指针操作，不管外面传的什么类型，
	都会取它的指针。
		2、操作指针有什么好处呢？可以避免一些麻烦，比如内存拷贝，赋值运算符都不用考虑
		3、当然也有一些问题，一旦把MMAVFrame插入到queue中，MMAVFrame的生命周期就完全托管给了queue，
	*/
	std::queue<T*> queue;



};