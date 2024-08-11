#include "MMAV.h"

#include "MMAVPacketPrivate.h"
#include "MMAVReaderPrivate.h"

MMAVReader::MMAVReader() {
	imp = new MMAVReaderPrivate();
	imp->formatCtx = avformat_alloc_context();
}
MMAVReader::~MMAVReader(){
	//释放资源时通常要判断资源是否为空，不为nullpter我们才去释放它,如果为nullptr就没什么好释放的
	if (imp->formatCtx != nullptr) {
		avformat_free_context(imp->formatCtx);//同样在析构里我们需要调用free方法
		imp->formatCtx = nullptr;//在释放之后，把它置为nullptr
	}
	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
	
}

int MMAVReader::Open(const char* path) {
	if (imp->formatCtx == nullptr) {//防止意外，做一层防护
		return -1;//直接返回一个失败
	}
	//avformat_open_input第一个参数是双重指针，所以在formatCtx前加一个取地址符&
	//第四个参数是一个设置项，比如用tcp还是udp去打开一个网络流，nullptr表示默认的方式
	int ret = avformat_open_input(&imp->formatCtx,path,nullptr,nullptr);
	if (!ret) {
		avformat_find_stream_info(imp->formatCtx,nullptr);//让它获取到流里的详细信息

	}
	return ret;
}
int MMAVReader::Close() {
	if (imp->formatCtx == nullptr) {//防止意外，做一层防护
		return -1;//直接返回一个失败
	}
	avformat_close_input(&imp->formatCtx);
	return 0;
}

int MMAVReader::Read(MMAVPacket* packet) {
	if (imp->formatCtx == nullptr) {//防止意外，做一层防护
		return -1;//直接返回一个失败
	}
	int ret = av_read_frame(imp->formatCtx,packet ->imp->pkt);
	return ret;
}