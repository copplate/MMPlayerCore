#include "MMAV.h"

#include "MMAVPacketPrivate.h"
#include "MMAVReaderPrivate.h"

MMAVReader::MMAVReader() {
	imp = new MMAVReaderPrivate();
	imp->formatCtx = avformat_alloc_context();
}
MMAVReader::~MMAVReader(){
	//�ͷ���Դʱͨ��Ҫ�ж���Դ�Ƿ�Ϊ�գ���Ϊnullpter���ǲ�ȥ�ͷ���,���Ϊnullptr��ûʲô���ͷŵ�
	if (imp->formatCtx != nullptr) {
		avformat_free_context(imp->formatCtx);//ͬ����������������Ҫ����free����
		imp->formatCtx = nullptr;//���ͷ�֮�󣬰�����Ϊnullptr
	}
	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
	
}

int MMAVReader::Open(const char* path) {
	if (imp->formatCtx == nullptr) {//��ֹ���⣬��һ�����
		return -1;//ֱ�ӷ���һ��ʧ��
	}
	//avformat_open_input��һ��������˫��ָ�룬������formatCtxǰ��һ��ȡ��ַ��&
	//���ĸ�������һ�������������tcp����udpȥ��һ����������nullptr��ʾĬ�ϵķ�ʽ
	int ret = avformat_open_input(&imp->formatCtx,path,nullptr,nullptr);
	if (!ret) {
		avformat_find_stream_info(imp->formatCtx,nullptr);//������ȡ���������ϸ��Ϣ

	}
	return ret;
}
int MMAVReader::Close() {
	if (imp->formatCtx == nullptr) {//��ֹ���⣬��һ�����
		return -1;//ֱ�ӷ���һ��ʧ��
	}
	avformat_close_input(&imp->formatCtx);
	return 0;
}

int MMAVReader::Read(MMAVPacket* packet) {
	if (imp->formatCtx == nullptr) {//��ֹ���⣬��һ�����
		return -1;//ֱ�ӷ���һ��ʧ��
	}
	int ret = av_read_frame(imp->formatCtx,packet ->imp->pkt);
	return ret;
}