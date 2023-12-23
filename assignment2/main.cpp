#include "main.h"
#include "restaurant.cpp"

int main(int argc, char *argv[])
{
	string fileName = "test.txt";
	simulate(fileName);

	return 0;
}

// int main() {
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
//	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
//	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
//	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
//	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
//	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
//     int* arr = new int[5];
//     cout << "arr: " << arr << endl;
//	delete[] arr;
//     return 0;
// }
