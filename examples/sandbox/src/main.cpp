#include "sandbox.h"

int main() 
{
    Sandbox* sandbox = new Sandbox();
    sandbox->init();
    sandbox->run();
    sandbox->stop();
    delete sandbox;

    return 0;
}
