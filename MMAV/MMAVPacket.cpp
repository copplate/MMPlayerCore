#include "MMAV.h"
#include "MMAVPacketPrivate.h"

MMAVPacket::MMAVPacket() {
	imp = new MMAVPacketPrivate();
	//在构造里把AVPacket构造出来，在析构里把它销毁掉
	imp->pkt = av_packet_alloc();
}
MMAVPacket::~MMAVPacket() {
	if (imp->pkt != nullptr) {
		av_packet_free(&imp->pkt);
		imp->pkt = nullptr;
	}
	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
	
}