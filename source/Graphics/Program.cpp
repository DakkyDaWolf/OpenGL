#include "pch.h"
#include "GraphicsGame.h"

using namespace Rendering;

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE /*instance*/, HINSTANCE /*previousInstance*/, LPSTR /*commandLine*/, int /*showCommand*/)
#else
int main()
#endif

{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GraphicsGame game("OpenGL Essentials");
	game.Run();

	return EXIT_SUCCESS;
}