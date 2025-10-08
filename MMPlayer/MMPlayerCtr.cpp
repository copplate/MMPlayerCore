#include "MMPlayer.h"


MMPlayerCtr::MMPlayerCtr()
{

}

MMPlayerCtr::~MMPlayerCtr()
{

}

void MMPlayerCtr::run()
{
	//在程序一开始启动之后，就在PlayerCtr中，把Read线程启动
	MMPlayerReaderThread readerThread("H://250824.mp4",this);
	readerThread.start();
	
	//获取线程启动时的时间 start_time
	long long startTime = MMAVTime::GetTime();

	MMAVFrame* videoFrame = nullptr;
	MMAVFrame* audioFrame = nullptr;

	while (!stopFlag) {  //stopFlag说明这也是一个外部可终结的循环
		//在循环的开始，可以先让线程挂起一段很短的时间，如果不挂起线程不断循环，cpu很快就跑满
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//获取当前的时间now_time
		long long nowTime = MMAVTime::GetTime();

		//获取当前时间和开始时间的差值 d_time
		long long dTime = nowTime - startTime;
		//printf("DTime:%lld\n", dTime);

		//从视频缓存队列中，获取一帧视频 frame_pts
		if (videoFrame == nullptr) {
			//尝试取一帧出来
			videoQueue.Pop(&videoFrame);
		}
		//printf("Video Queue Size:%d\n", videoQueue.Size());
		
		if (videoFrame != nullptr) {//当拿到这一帧
			//如果frame_pts <= d_time , 这帧视频应该立即播放出来
			if (videoFrame->GetPts() <= dTime) {
				printf("Video Frame:%lld\n", videoFrame->GetPts());
				delete videoFrame;
				videoFrame = nullptr;
			}
			else {//否则，这帧视频还不到播放的时候，程序进行自旋，或者去处理音频

			}
			
		}
		
		//从音频缓存队列中，获取一帧音频 frame_pts
		if (audioFrame == nullptr) {
			audioQueue.Pop(&audioFrame);
		}
		if (audioFrame != nullptr) {
			//如果frame_pts <= d_time , 这帧音频应该立即播放出来
			if (audioFrame->GetPts() <= dTime) {
				//printf("Audio Frame\n");
				delete audioFrame;
				audioFrame = nullptr;
			}
			else {//否则，这帧音频还不到播放的时候，程序进行自旋

			}
		}

		/*
		这样音频帧和视频帧是分开处理的，都是用系统时间戳进行比较，相互之间比较独立，是互不影响的。
		*/


	}

	//在PlayerCtr线程结束之后，把read线程stop掉
	readerThread.Stop();
	


}

int MMPlayerCtr::GetVideoQueueSize()
{
	return videoQueue.Size();
}
int MMPlayerCtr::GetAudioQueueSize()
{
	return audioQueue.Size();
}

int MMPlayerCtr::PushFrameToVideoQueue(MMAVFrame* frame)
{
	return videoQueue.Push(frame);
}
int MMPlayerCtr::PushFrameToAudioQueue(MMAVFrame* frame)
{
	return audioQueue.Push(frame);
}