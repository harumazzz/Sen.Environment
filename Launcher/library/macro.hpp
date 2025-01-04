#pragma once

#if _WIN32
#define MAIN int wmain(int argc, wchar_t* argv[])
#else
#define MAIN int main(int argc, char* argv[])
#endif
