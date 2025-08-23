#include "MMAV.h"

#include "MMAVFramePrivate.h"

extern "C"
{
#include <libavutil/pixdesc.h>
}

MMAVFrame::MMAVFrame()
{
	imp = new MMAVFramePrivate();
	imp->frame = av_frame_alloc();
}

MMAVFrame::~MMAVFrame()
{
	if (imp->frame != nullptr) {
		av_frame_free(&imp->frame);
		imp->frame = nullptr;
	}

	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}

int MMAVFrame::VideoPrint()
{
	printf("Width: %d\n", imp->frame->width);
	printf("Height:%d\n", imp->frame->height);
	AVPixelFormat format = (AVPixelFormat)(imp->frame->format);
	//动态分配一个string
	char* str = (char*)malloc(128);
	//ffmpeg提供了一个获取到类型字符串的函数
	str = av_get_pix_fmt_string(str,128, format);
	printf("Pixel Format:%s\n", str);
	//malloc出的东西，最后一定要free掉
	free(str);
	return 0;
}