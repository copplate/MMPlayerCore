#include "MMPlayer/MMPlayer.h"

#include <Windows.h>

/*
std::int64_t get_mill_time_stamp() {
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	return tmp.count();
}*/

int main()
{
	long long t = MMAVTime::GetTime();
	printf("t:%lld\n",t);

	MMPlayer player("H://250824.mp4");//新建一个播放器实例

	int ret = player.Open();
	if (ret) {
		printf("Player Open Fail\n");
		return -1;//程序结束
	}
	Sleep(1000 * 10000);

	player.Play();

	return 0;
}