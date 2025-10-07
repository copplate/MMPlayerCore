#pragma once

extern "C"
{
#include <libavformat/avformat.h> 
}

class MMAVFramePrivate
{
public:
	AVFrame* frame = nullptr;
	double ptsSec = 0.0;//一个用秒做单位的时间戳
};