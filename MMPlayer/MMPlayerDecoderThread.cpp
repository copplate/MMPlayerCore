#include "MMPlayer.h"

MMPlayerDecoderThread::MMPlayerDecoderThread(MMPlayerCtr* _playerCtr, MMDecoderType _type)
{
	decoder = new MMAVDecoder();
	playerCtr = _playerCtr;
	type = _type;

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
		std::this_thread::sleep_for(std::chrono::milliseconds(1));//延时一会儿，防止MMPlayerReaderThread线程抢不到锁
		MMAVPacket* pkt = nullptr;
		int ret = packetQueue.Pop(&pkt);
		if (ret) {
			//队列为空，取不到Packet
			continue;
		}

		decoder->SendPacket(pkt);
		while (1) {
			MMAVFrame* frame = new MMAVFrame();
			ret = decoder->RecvFrame(frame);
			if (ret) {
				break;
			}
			frameCount++;
			std::this_thread::sleep_for(std::chrono::seconds(1));//每解码成功一次，sleep一秒钟
			//printf("Decoder Success:%d\n",frameCount);

			//解码成功后，通过playerCtr的句柄，向其塞入Frame数据
			if (type == MMDecoderType::MMDECODER_TYPE_VIDEO) {
				playerCtr->PushFrameToVideoQueue(frame);
			}
			else if (type == MMDecoderType::MMDECODER_TYPE_AUDIO) {
				playerCtr->PushFrameToAudioQueue(frame);
			}

		}

	}


}