#include "stdafx.h"

int main(array<System::String ^> ^args)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(753);

	Game game;

	game.Play();

    return 0;

}
