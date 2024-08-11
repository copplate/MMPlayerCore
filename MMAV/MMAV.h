


#pragma once

class MMAVPacketPrivate;


class MMAVPacket {

public:
	MMAVPacket();//���췽��
	~MMAVPacket();//��������


public:
	MMAVPacketPrivate* imp = nullptr;
};

class MMAVReaderPrivate;

class MMAVReader {
public:
	MMAVReader();//������һ�����췽������������
	~MMAVReader();

	int Open(const char* path);//��һ���ļ�
	int Close();//��Ȼ��open����Ҫ��close

	/*
	readʱ�ȴ�һ��MMAVPacket��ȥ�������ⲿ���ǻᴴ��һ��MMAVPacket�����Read������ָ��MMAVPacket* ������һ��packet��
	��ôRead�������packet���������������ʧ�ܵĻ����Ǹ���Read�������صĴ�������԰�packetֱ������
	*/
	int Read(MMAVPacket* packet);

private:
	MMAVReaderPrivate* imp = nullptr;
};