// MMPlayer.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。
#pragma once

//#include <iostream>

// TODO: 在此处引用程序需要的其他标头。
#include <string>
#include "MMThread/MMThread.h"

class MMPlayerReaderThread :public MMThread //MMPlayer中会用到 MMPlayerReaderThread这个类，所以要放在MMPlayer类的前面
{
public:
	MMPlayerReaderThread(std::string path);
	~MMPlayerReaderThread();

	virtual void run();

private:
	std::string path;
};


class MMPlayer {
public:
	MMPlayer(std::string path);
	~MMPlayer();

	int Open();//打开这个流
	int Stop();//停止这个流

	int Play();//开始播放
	int Pause();//暂停播放

	int Seek(double time);

private:
	std::string path;
	MMPlayerReaderThread* readerThread = nullptr;
};


