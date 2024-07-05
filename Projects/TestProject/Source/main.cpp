#include "Engine.h"

#include <iostream>

// main is the entry point for cross-platform applications
int main() 
{
    Engine* vroem = new Engine();
    vroem->Run();

    printf("    DOEI!!!!    \n");
    
    delete vroem;
    system("Pause");
    return 0;
}