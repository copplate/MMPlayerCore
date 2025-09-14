#include "MMPlayer/MMPlayer.h"

#include <Windows.h>

int main()
{
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