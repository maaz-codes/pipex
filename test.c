#include <unistd.h>

int main(void)
{
	write(1, "hello_stdout\n", 13);
       	write(2, "hello_stderr\n", 13);	
}
