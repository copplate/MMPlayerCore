#include "MMAV.h"

#include "MMAVFramePrivate.h"

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