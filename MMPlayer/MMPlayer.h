// MMPlayer.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。
#pragma once

//#include <iostream>

// TODO: 在此处引用程序需要的其他标头。
#include <string>
#include "MMThread/MMThread.h"
#include "MMAV/MMAV.h"
#include "MMQueue/MMQueue.h"

enum MMDecoderType
{
	MMDECODER_TYPE_VIDEO = 0,
	MMDECODER_TYPE_AUDIO = 1
};

class MMPlayerCtr : public MMThread //一个播放控制线程
{
public:
	MMPlayerCtr();
	~MMPlayerCtr();

	virtual void run();

	int GetVideoQueueSize();
	int GetAudioQueueSize();

	int PushFrameToVideoQueue(MMAVFrame* frame);
	int PushFrameToAudioQueue(MMAVFrame* frame);

private:
	//videoQueue和audioQueue这两个队列是和解码线程相关的，解码线程要往这两个队列里塞数据，playerCtr再从里面拿数据
	MMQueue<MMAVFrame> videoQueue;
	MMQueue<MMAVFrame> audioQueue;
};

class MMPlayerReaderThread :public MMThread //MMPlayer中会用到 MMPlayerReaderThread这个类，所以要放在MMPlayer类的前面
{
public:
	MMPlayerReaderThread(std::string path,MMPlayerCtr* playerCtr);//先把playerCtr传给reader，再由reader传给decoder
	~MMPlayerReaderThread();

	virtual void run();

private:
	std::string path;

	MMPlayerCtr* playerCtr = nullptr;
};

class MMPlayerDecoderThread : public MMThread
{
public:
	MMPlayerDecoderThread(MMPlayerCtr* playerCtr, MMDecoderType type);//由reader来告诉decoder是音频还是视频
	~MMPlayerDecoderThread();

	virtual void run();

	int Init(MMAVStream * avStream);//因为MMAVDecoder中需要Init函数，所以在这里也暴露一个Init函数

	int PutPacket(MMAVPacket * pkt);//提供一个往Packet里存数据的方法

	int GetPacketQueueSize();//让读取线程知道，解码线程当前缓存了多少Packet

private:
	MMAVDecoder * decoder = nullptr;
	MMQueue<MMAVPacket> packetQueue;

	MMPlayerCtr* playerCtr = nullptr;

	MMDecoderType type;
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

	MMPlayerCtr* playerCtr = nullptr;
};


