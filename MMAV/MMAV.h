


#pragma once
class MMAVStreamPrivate;

class MMAVStream;

class MMAVPacketPrivate;


class MMAVPacket {

public:
	MMAVPacket();//构造方法
	~MMAVPacket();//析构方法

	int GetIndex();
public:
	MMAVPacketPrivate* imp = nullptr;
};

class MMAVFramePrivate;

class MMAVFrame {
public:
	MMAVFrame();
	~MMAVFrame();

	int VideoPrint();
	int AudioPrint();

	//获取图像的宽高
	int GetW();
	int GetH();

	//直接传一个外部分配好的buffer进来,然后把Y分量拷贝到这个buffer上
	int GetY(unsigned char * y);
	//获取U分量
	int GetU(unsigned char* u);
	//获取V分量
	int GetV(unsigned char* v);

	long long GetPts();

public:
	MMAVFramePrivate* imp = nullptr;
};

class MMAVReaderPrivate;

class MMAVReader {
public:
	MMAVReader();//照例给一个构造方法和析构方法
	~MMAVReader();

	int Open(const char* path);//打开一个文件

	int GetStreamCount();//有多少个AVStream
	/*
	传入指针后，帮我们把MMAVStream的内容填充好；
	int Open函数可能会拿出来很多stream，要多传一个streamId
	*/
	int GetStream(MMAVStream * stream,int streamId);

	int GetVideoStreamIndex();
	int GetAudioStreamIndex();


	int Close();//既然有open，就要有close

	/*
	read时先传一个MMAVPacket进去，调用外部我们会创建一个MMAVPacket，如果Read方法从指针MMAVPacket* 读到了一个packet，
	那么Read方法会把packet里的数据填充进来，失败的话我们根据Read方法返回的错误码可以把packet直接销毁
	*/
	int Read(MMAVPacket* packet);

private:
	MMAVReaderPrivate* imp = nullptr;
};

class MMAVStreamPrivate;

class MMAVStream
{
public:
	MMAVStream();
	~MMAVStream();
public:
	int streamIndex = -1;
	int timebaseNum = 0;
	int timebaseDen = 0;
	MMAVStreamPrivate* imp = nullptr;
};

class MMAVDecoderPrivate;//仿照之前把ffmpeg的api隐藏起来的做法，声明这个MMAVDecoderPrivate的class

class MMAVDecoder 
{
public:
	MMAVDecoder();
	~MMAVDecoder();

	int Init(MMAVStream * stream);

	int SendPacket(MMAVPacket * pkt);

	int RecvFrame(MMAVFrame * frame);

	int Close();
private:
	MMAVDecoderPrivate* imp = nullptr;

	int timebaseNum = 0;
	int timebaseDen = 0;
};

class MMAVTime
{
public:
	static long long GetTime();//时间戳用一个long long类型
};