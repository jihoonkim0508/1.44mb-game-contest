#define DLLIMPORT __declspec(dllimport)

DLLIMPORT int __stdcall MessageBoxA(void *, const char *, const char *, unsigned long);
DLLIMPORT __declspec(noreturn) void __stdcall ExitProcess(unsigned int);

void mainCRTStartup(void)
{
    MessageBoxA(0, "Hello", "1.44 MB probe", 0);
    ExitProcess(0);
}
