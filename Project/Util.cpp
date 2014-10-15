#include "stdafx.h"
#include "Util.h"

time_t Util::startTime = time(NULL);

void Util::Pause()
{
	char* message = "...press the enter key to continue...";
	Console::SetCursorPosition((Console::WindowWidth - strlen(message)) / 2,
		Console::WindowHeight - 1);
	cout << message;
	cin.ignore(INT_MAX, '\n');
}

void Util::FlushKeys()
{
	for (int i = 0; i < 256; ++i) GetAsyncKeyState(i);
	while (Console::KeyAvailable) Console::ReadKey(true);
}

void Util::EOLWrap(bool on)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(out, &mode);
	if (on)
		mode |= ENABLE_WRAP_AT_EOL_OUTPUT;
	else
		mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
	SetConsoleMode(out, mode);
}

void Util::WriteWChar(int x, int y, wchar_t symbol)
{
	Console::SetCursorPosition(x, y);
	Console::Write(symbol);
}

void Util::DisplayFPS(unsigned long long frame)
{
	time_t seconds = time(NULL) - Util::startTime;
	float fps = 0 ? 0 : frame / (float)seconds;

	cout.precision(1);
	cout.setf(ios_base::showpoint);
	cout.setf(ios_base::fixed, ios_base::floatfield);
	cout.width(6);
	Console::SetCursorPosition(0, Console::WindowHeight - 2);

	cout << fps;
}