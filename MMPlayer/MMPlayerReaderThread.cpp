#include "MMPlayer.h"
#include "MMAV/MMAV.h"


MMPlayerReaderThread::MMPlayerReaderThread(std::string _path)
{
	path = _path;
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

	//TODO 拿到流之后要初始化一下解码器

	while (!stopFlag) {
		MMAVPacket* pkt = new MMAVPacket();//不希望循环一次之后，pkt就被释放掉，所以用new的形式
		int ret = reader.Read(pkt);
		if (ret) {
			delete pkt;
			pkt = nullptr;
			break;//Read出错了，或者已经读到了文件流的末尾
		}

		//将Packet放入缓存

		printf("Read Packet Success\n");
		delete pkt;
		pkt = nullptr;

	}

	reader.Close();


}