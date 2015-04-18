#include "MyForm.h"
#include <windows.h>
using namespace GORDIAN;


int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	Application::Run(gcnew MyForm());

	return 0;
}