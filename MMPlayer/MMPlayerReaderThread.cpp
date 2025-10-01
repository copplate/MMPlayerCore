#include "MMPlayer.h"
#include "MMAV/MMAV.h"


MMPlayerReaderThread::MMPlayerReaderThread(std::string _path, MMPlayerCtr* _playerCtr)
{
	path = _path;
	playerCtr = _playerCtr;
}
MMPlayerReaderThread::~MMPlayerReaderThread()
{

}

void MMPlayerReaderThread::run()
{
	MMAVReader reader;
	int ret = reader.Open(path.c_str());
	if (ret) {//如果Open失败，后续就没必要执行了
		return;
	}

	int videoStreamIndex = reader.GetVideoStreamIndex();
	int audioStreamIndex = reader.GetAudioStreamIndex();

	//拿到流之后要初始化一下解码器
	//有两个streamIndex，启动两个decoder线程
	MMPlayerDecoderThread* videoDecoderThread = new MMPlayerDecoderThread(playerCtr,MMDecoderType::MMDECODER_TYPE_VIDEO);
	MMPlayerDecoderThread* audioDecoderThread = new MMPlayerDecoderThread(playerCtr,MMDecoderType::MMDECODER_TYPE_AUDIO);

	MMAVStream videoStream;
	reader.GetStream(&videoStream,videoStreamIndex);
	videoDecoderThread->Init(&videoStream);

	MMAVStream audioStream;
	reader.GetStream(&audioStream, audioStreamIndex);
	audioDecoderThread->Init(&audioStream);

	videoDecoderThread->start();
	audioDecoderThread->start();


	while (!stopFlag) {
		//处理解码线程可能比较慢的情况
		if (videoDecoderThread->GetPacketQueueSize() > 5 && audioDecoderThread->GetPacketQueueSize() > 5) {
			continue;
		}

		MMAVPacket* pkt = new MMAVPacket();//不希望循环一次之后，pkt就被释放掉，所以用new的形式
		int ret = reader.Read(pkt);
		if (ret) {
			delete pkt;
			pkt = nullptr;
			break;//Read出错了，或者已经读到了文件流的末尾
		}

		if (pkt->GetIndex() == videoStreamIndex) {//区分packet是音频流还是视频流，然后放到对应的视频流或音频流的解码器中
			videoDecoderThread->PutPacket(pkt);
		}

		if (pkt->GetIndex() == audioStreamIndex) {
			audioDecoderThread->PutPacket(pkt);
		}

		//将Packet放入缓存

		//printf("Read Packet Success\n");
		//delete pkt;
		//pkt = nullptr;

	}

	videoDecoderThread->Stop();//视频播到最后了，停止这两个线程
	audioDecoderThread->Stop();

	reader.Close();


}