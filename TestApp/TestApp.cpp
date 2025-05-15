
#include <iostream>
#include <windows.h>

int main() {
    std::cout << "Test app running. PID: " << GetCurrentProcessId() << std::endl;
    system("pause");
    return 0;
}
