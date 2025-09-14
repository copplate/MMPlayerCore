#include "MMPlayer.h"

MMPlayer::MMPlayer(std::string _path)
{
	path = _path;
}
MMPlayer::~MMPlayer()
{

}

int MMPlayer::Open()//打开这个流
{
	if (readerThread == nullptr) {
		readerThread = new MMPlayerReaderThread(path);

		//启动线程
		readerThread->start();
		return 0;
	}
	
	return -1;
}
int MMPlayer::Stop()//停止这个流
{
	if (readerThread != nullptr) {
		readerThread->Stop();
		delete readerThread;
		readerThread = nullptr;
	}
	
	return 0;
}

int MMPlayer::Play()//开始播放
{
	return 0;
}
int MMPlayer::Pause()//暂停播放
{
	return 0;
}

int MMPlayer::Seek(double time) 
{
	return 0;
}