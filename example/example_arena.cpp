#include <stzcpp/arena.h>

int main()
{
    Arena mem = Arena(1024);
    mem.Print("Init");
    return 0;
}

