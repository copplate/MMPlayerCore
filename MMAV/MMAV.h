


#pragma once

class MMAVPacketPrivate;


class MMAVPacket {

public:
	MMAVPacket();//构造方法
	~MMAVPacket();//析构方法


public:
	MMAVPacketPrivate* imp = nullptr;
};

class MMAVReaderPrivate;

class MMAVReader {
public:
	MMAVReader();//照例给一个构造方法和析构方法
	~MMAVReader();

	int Open(const char* path);//打开一个文件
	int Close();//既然有open，就要有close

	/*
	read时先传一个MMAVPacket进去，调用外部我们会创建一个MMAVPacket，如果Read方法从指针MMAVPacket* 读到了一个packet，
	那么Read方法会把packet里的数据填充进来，失败的话我们根据Read方法返回的错误码可以把packet直接销毁
	*/
	int Read(MMAVPacket* packet);

private:
	MMAVReaderPrivate* imp = nullptr;
};