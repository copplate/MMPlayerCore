#include "MMAV.h"

#include "MMAVDecoderPrivate.h"
#include "MMAVStreamPrivate.h"
#include "MMAVPacketPrivate.h"
#include "MMAVFramePrivate.h"


MMAVDecoder::MMAVDecoder() {
	imp = new MMAVDecoderPrivate();
	imp->codecContext = avcodec_alloc_context3(nullptr);//const AVCodec* codec这个参数，如果不知道，可以传null
	
}

MMAVDecoder::~MMAVDecoder() {
	if (imp->codecContext != nullptr) {
		avcodec_free_context(&imp->codecContext);
		imp->codecContext = nullptr;
	}
	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}

int MMAVDecoder::Init(MMAVStream* stream)
{
	timebaseNum = stream->timebaseNum;
	timebaseDen = stream->timebaseDen;

	avcodec_parameters_to_context(imp->codecContext, stream->imp->codecpar);
	AVCodec * avcodec = avcodec_find_decoder(imp->codecContext->codec_id);
	//解码时，想解这一路流用什么参数都是视频规定好的，第三个参数AVDictionary **options 直接传nullptr

	int ret = avcodec_open2(imp->codecContext,avcodec,nullptr);
	if (ret) {
		printf("avcodec_open2 fail ret:d%\n", ret);
		return -1;
	}
	return 0;
}

int MMAVDecoder::SendPacket(MMAVPacket* pkt)
{
	int ret = 0;
	if (pkt == nullptr) {
		ret = avcodec_send_packet(imp->codecContext, nullptr);
	}
	else {
		//解码前的数据叫AVPacket，经过解码器解码的数据叫AVFrame
		ret = avcodec_send_packet(imp->codecContext, pkt->imp->pkt);
	}
	
	return ret;
}

int MMAVDecoder::RecvFrame(MMAVFrame* frame)
{
	int ret = avcodec_receive_frame(imp->codecContext,frame->imp->frame);
	if (ret) {
		//将秒级时间戳计算并且赋值
		frame->imp->ptsSec = frame->imp->frame->pts * 1.0 * timebaseNum / timebaseDen;//frame->imp->frame->pts * 1.0是为了让pts变成double类型
	}
	return ret;
}

int MMAVDecoder::Close()
{
	return avcodec_close(imp->codecContext);
}