#include <unistd.h>

int main()
{
    write(1, "stdout\n", 7);
    write(2, "stderr\n", 7);
    return (0);
}