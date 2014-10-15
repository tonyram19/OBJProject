#pragma once
struct Util
{
	static time_t startTime;

	static void Pause();
	static void FlushKeys();
	static void EOLWrap(bool on);
	static void WriteWChar(int x, int y, wchar_t symbol);

	//FPS Calls
	static void DisplayFPS(unsigned long long frame);
};



