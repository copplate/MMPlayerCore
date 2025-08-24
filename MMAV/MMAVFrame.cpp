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

	for (int i = 0;i < AV_NUM_DATA_POINTERS;i++) {
		printf("Linesize[%d] : %d\n", i, imp->frame->linesize[i]);
	}

	return 0;
}

int MMAVFrame::GetW()
{
	return imp->frame->width;
}

int MMAVFrame::GetH()
{
	return imp->frame->height;
}

// GetY、GetU、GetV这三个函数的作用是，从AVFrame里把yuv数据拷贝出去
int MMAVFrame::GetY(unsigned char* y)
{
	int width = GetW();
	int height = GetH();

	//逐行拷贝，高度是多少就有多少行
	for (int i = 0;i < height;i++) {
		memcpy(y + i * width, imp->frame->data[0] + i* imp->frame->linesize[0],width);
	}

	return 0;
}

int MMAVFrame::GetU(unsigned char* u)
{
	int width = GetW() / 2;//因为格式是yuv420p,宽和高都要除以二
	int height = GetH() / 2;

	//逐行拷贝，高度是多少就有多少行
	for (int i = 0;i < height;i++) {
		memcpy(u + i * width, imp->frame->data[1] + i * imp->frame->linesize[1], width);
	}

	return 0;
}

int MMAVFrame::GetV(unsigned char* v)
{
	int width = GetW() / 2;
	int height = GetH() / 2;

	//逐行拷贝，高度是多少就有多少行
	for (int i = 0;i < height;i++) {
		memcpy(v + i * width, imp->frame->data[2] + i * imp->frame->linesize[2], width);
	}

	return 0;
}