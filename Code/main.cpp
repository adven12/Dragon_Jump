#include "MyFramework.h"

int main(int argc, char* argv[])
{
    try
    {
        run(new MyFramework());
    }
    catch (const std::exception& exe)
    {
        MyFramework::HandleExeption(exe);
    }
    return 0;
}