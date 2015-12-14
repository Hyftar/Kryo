#include "kryocore.h"

int main(int argc, char** argv)
{
    Kryo::Engine engine;
    engine.SetMaxFps(144);
    engine.Start("Kryo", 800, 600, false);
}
