#include "MMPlayer.h"

MMPlayerDecoderThread::MMPlayerDecoderThread()
{
	decoder = new MMAVDecoder();


}
MMPlayerDecoderThread::~MMPlayerDecoderThread()
{
	if (decoder != nullptr) {
		delete decoder;
		decoder = nullptr;
	}
}

int MMPlayerDecoderThread::Init(MMAVStream* avStream)//可以定义一个限制，线程只有在被Init之后，才可以start
{
	return decoder->Init(avStream);
}

int MMPlayerDecoderThread::PutPacket(MMAVPacket* pkt)
{
	return packetQueue.Push(pkt);
}

int MMPlayerDecoderThread::GetPacketQueueSize()
{
	return packetQueue.Size();
}

void MMPlayerDecoderThread::run()
{
	int frameCount = 0;
	//进行解码操作
	while (!stopFlag) {//stopFlag的功能是让 解码操作这个循环 变得外部可中止
		MMAVPacket* pkt = nullptr;
		int ret = packetQueue.Pop(&pkt);
		if (ret) {
			//队列为空，取不到Packet
			continue;
		}

		decoder->SendPacket(pkt);
		while (1) {
			MMAVFrame frame;
			ret = decoder->RecvFrame(&frame);
			if (ret) {
				break;
			}
			frameCount++;
			std::this_thread::sleep_for(std::chrono::seconds(1));//每解码成功一次，sleep一秒钟
			printf("Decoder Success:%d\n",frameCount);
		}

	}


}