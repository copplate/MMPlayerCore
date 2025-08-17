#pragma once

extern "C"
{
#include <libavformat/avformat.h> 
}

class MMAVDecoderPrivate
{
public:
	AVCodecContext* codecContext = nullptr;//所有后面流程都用到AVCodecContext * codecContext,我们就把codecContext变成成员变量
};