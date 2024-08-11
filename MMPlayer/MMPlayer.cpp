// MMPlayer.cpp: 定义应用程序的入口点。
//

#include "MMPlayer.h"
#include <thread>
#include "MMThread/MMThread.h"
#include <chrono>

using namespace std;


void threadFunction(int i) {
	printf("Thread Function:%d\n", i);
}

//一个比较经典的线程的跑的实验，两个子线程争夺cpu资源...

class MyMMThread :public MMThread {
	//总结，这种仿Java的写法，对比一开始传静态函数给线程当参数的写法的好处是
	//1、首先它是一个类，类的好处是:比如下面声明了资源int a,我们就可以很直观的知道，a就是在这个线程里边被调用的
	// ，a就属于这个线程，不属于别的。然后在构造函数里传一些参数，就可以看出资源是属于谁的描述的非常清楚。
	//而不会和之前过程化的那样，在函数外边写一个变量，变量属于哪个函数也不清楚。


private:
	int a = 22;
public:
	MyMMThread(int _a) {
		a = _a;
	}

	virtual void run() {
		cout << "MyMMThread" << a << endl;

	}
};


int main_thread()//把之前的main函数做一个重命名，再写一个新的main函数
{
	//********------C++11中线程的基本用法 ---------*****************
	std::thread t(threadFunction, 10);//这样就可以启动一个线程,把threadFunction当成t的一个构造函数。
	//而且这个构造方法的长度是可变的。10这个参数就会被传到函数里。
	//一个线程要启动和销毁，只能由启动的线程来销毁。

	t.join();//join()方法可以将main函数阻塞住，当子线程结束后会退出阻塞状态
	//t.detach();//detach()可以让子线程和创建它的函数完全分离，子线程怎么跑就和创建它的线程没有关系了，
	//结束之后子线程的资源也不需要创建它的线程去回收，操作系统会来回收 
	//如果不调用detach()，main函数结束时会报一个异常，因为thread有一个状态，叫做joinable，如果在主线程结束时子线程还处于joinable，就会报一个错
	//我们通常不希望这种情况发生，所以启动线程之后，要不然就执行t.join()，要不然就执行t.detach()把它detach出来。
	//否则的话等执行完销毁的时候是会有问题的

	//********--------线程，更聪明的用法-------*****************
	//刚才已经说了C++里面线程的一些基本的实现办法，我们可以直接用这个办法来启动一个线程。
	//但还有一些问题：1、传的threadFunction是一个静态函数
	//其实我们要启动一个线程并不困难，多线程编程的麻烦之处在于，多线程之间资源的协调，多线程之间的同步。
	//有的是线程结束了资源还没释放，有的是资源释放了但是线程还在调用。
	//    静态函数其实并没有把资源的归属问题描述的很清楚。这样不是说不行，但是不太好。
	//所以我们需要另一种编程的形式，将这些资源的归属，即它属于哪个线程，它的生命周期应该是什么，
	//线程之间是否有包含关系(由一个线程启动另外一个线程），把这些东西理清楚。这样用基本用法就有点描述不清了。
	//    这时我们可以参考一下其他语言比如Java的线程
	//新建一个子模块Thread，让MMPlayer子模块引用Thread子模块，让各个模块之间有一个独立性


	MyMMThread t2(10);
	t2.start();
	//在start()方法里，调用了t.detach();方法，线程启动后和主线程是完全分离的状态。
	//这样就有可能主函数比子线程先执行完，操作系统把整个进程关闭，导致我们观察不到子线程的执行，所以我们要让主函数暂停一会
	std::this_thread::sleep_for(std::chrono::seconds(2));
	cout << "Hello CMake." << endl;
	return 0;
}

#include "MMAV/MMAV.h"
int main() {
	MMAVReader reader;
	int ret = reader.Open("H://2403161.mp4");
	if (ret) {//如果出现错误
		printf("Open File Fail!!!");
		return -1;
	}
	while (1) {//中间过程用一个死循环读取
		MMAVPacket pkt;
		ret = reader.Read(&pkt);
		if (ret) {//只要返回值不等于0，说明有问题，就应该break出去
			break;
		}
		cout << "Read Packet Success!!" << endl;
	}

	reader.Close();//在最后，把它close掉
	return 0;
}


